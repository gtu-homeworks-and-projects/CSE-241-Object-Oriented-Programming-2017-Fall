//
// Created by cecen on 12.10.2017.
//

#include "ConnectFour.h"

int ConnectFour::livingCellCount = 0;

void upperCase(char &letter) {
    if (letter <= 'z' && letter >= 'a') letter += 'A' - 'a';
}

ConnectFour::ConnectFour(): gameInitialized(false){}

ConnectFour::ConnectFour(bool init) {
    if(init){
        takeGameInfo();
        initGame();
    }
}

ConnectFour::ConnectFour(const Size &size, bool isCompPlaying) : boardSize(size), aiEnable(isCompPlaying) {
    /* If given parameters out of printable or playable game size, resets it to their minimum of maxmum values */
    if (boardSize.height < 4) boardSize.height = 4;
    if (boardSize.width < 4) boardSize.width = 4;
    if (boardSize.height > 26) boardSize.height = 26;
    if (boardSize.width > 26) boardSize.width = 26;

    initGame();
}

ConnectFour::ConnectFour(int height, int width, bool isCompPlaying) {
    boardSize.height = height;
    boardSize.width = width;
    aiEnable = isCompPlaying;

    /* If given parameters out of printable or playable game size, resets it to their minimum of maxmum values */
    if (boardSize.height < 4) boardSize.height = 4;
    if (boardSize.width < 4) boardSize.width = 4;
    if (boardSize.height > 26) boardSize.height = 26;
    if (boardSize.width > 26) boardSize.width = 26;

    initGame();
}

commandType ConnectFour::saveManagement(commandType command, string saveFileName, int moveCount, int player) {
    if(command == LOAD){
        ifstream saveFile(saveFileName); /* Opens file as input stream */
        int isColFull, cellVal;
        if (!saveFile.fail()) {
            saveFile >> boardSize.height
                     >> boardSize.width;
            initGame();
            livingCellCount -= moveCount;
            for (int i = 0; i < boardSize.width; ++i) {
                saveFile >> isColFull;
                boardSize.isColFull[i] = isColFull != 0;
                for (int j = 0; j < boardSize.height; ++j){
                    saveFile >> cellVal;
                    gameCells[i][j].setCellVal(cellVal);
                }
            }
            saveFile >> aiEnable
                     >> moveCount
                     >> player;
            livingCellCount += moveCount;
        } else cerr << "File couldn't be opened." << endl;
    }
    else if(command == SAVE){
        ofstream saveFile(saveFileName); /* Opens file as output stream */
        int isColFull;
        if (!saveFile.fail()) {
            saveFile << boardSize.height << endl
                     << boardSize.width << endl;

            for (int i = 0; i < boardSize.width; ++i) {
                isColFull = (boardSize.isColFull[i])? 1: 0;
                saveFile << isColFull << endl;

                for (int j = 0; j < boardSize.height; ++j)
                    saveFile << gameCells[j][i].getCellVal() << endl;
            }
            saveFile << aiEnable << endl
                     << moveCount << endl
                     << player << endl;
        } else cerr << "File couldn't be opened." << endl;
    }
    return command;
}

int ConnectFour::livingCells() {
    return livingCellCount;
}

void ConnectFour::display() const{
    auto Letter = 'A';
    for (int k = 0; k < boardSize.width; ++k, ++Letter) cout << Letter << ' ';
    cout << endl;

    for (int i = 0; i < boardSize.height; ++i) {
        for (int j = 0; j < boardSize.width; ++j)
            switch (gameCells[i][j].getCellVal()) {
                case 3: /* Empty Cell */
                    cout << ". ";
                    break;
                case 1: /* Player 1 Cell */
                    cout << "X ";
                    break;
                case 2: /* Player 2/Computer Cell */
                    cout << "O ";
                    break;
                case 4: /* Player 1 Won line Cell */
                    cout << "x ";
                    break;
                case 5: /* Player 2/Computer Won Line Cell */
                    cout << "o ";
                    break;
                default:
                    break;
            }
        cout << endl;
    }
    cout << "Living cells: " << livingCells() << endl;
}

void ConnectFour::initGame() {
    gameCells.clear();
    boardSize.isColFull.clear();
    for (int i = 0; i < boardSize.height; ++i)
        gameCells.emplace_back(vector<Cell>(boardSize.width));

    for (int k = 0; k < boardSize.width; ++k)
        boardSize.isColFull.emplace_back(false);

    /*for (int j = 0; j < boardSize.height; ++j) {
        gameCells[boardSize.height][j].setCellVal(0);
        gameCells[j][boardSize.width].setCellVal(0);
    }*/
    maxNumOfMoves = boardSize.height * boardSize.width;
    endGame = false;
    isItWin = false;
    gameInitialized = true;
}

void ConnectFour::takeGameInfo() {
    int height = 0, width = 0;
    /*Reading height */
    while (height < 4 || height > 'Z' - 'A' + 1) {
        cout << "Enter height of the board (Must be over or equal to 4 and under or equal to 26): " << endl;

        cin >> height;
        if (cin.fail()) {
            cin.clear(); // Clears any error flags if cin failed
            while (cin.get() != '\n'); // If user enters a char, cin.get() empty buffer
        }
        if (height < 4 || height > 'Z' - 'A' + 1) cerr << "Please enter a valid number!" << endl;
    }
    boardSize.height = height;
    /* Reading width */
    while (width < 4 || width > 'Z' - 'A' + 1) {
        cout << "Enter width of the board (Must be over or equal to 4 and under or equal to 26): " << endl;

        cin >> width;
        if (cin.fail()) {
            cin.clear(); // Clears any error flags if cin failed
            while (cin.get() != '\n'); // If user enters a char, cin.get() empty buffer
        }
            if (width < 4 || width > 'Z' - 'A' + 1) cerr << "Please enter a valid number!" << endl;
    }
    boardSize.width = width;
    cout << "Board size will be " << height << "x" << width << endl;

    /* Reading game mode */
    auto gameModeChoice = '0';
    auto isInputValid = false;
    while (!isInputValid) {
        cout << "Choose game mode:\n1.Player versus Player (P)\n2.Player versus Computer (C)\n";
        cin >> gameModeChoice;
        cin.clear();
        cin.ignore(255, '\n');
        if ('P' == gameModeChoice || 'C' == gameModeChoice || 'p' == gameModeChoice || 'c' == gameModeChoice)
            isInputValid = true; /* Input validation */
        else
            cerr << "Please choose a valid game mode! (1 or 2)"
                 << endl; /*Gives an error message in order to notify player(s) */

    }
    aiEnable = 'C' == gameModeChoice || 'c' == gameModeChoice;

    if (!aiEnable) cout << "Player 1 is 'X', Player 2 is 'O'" << endl; /* Prints players characters */
    else cout << "Player 1 is 'X', Computer is 'O'" << endl;
}

bool ConnectFour::isItTheEnd(int col) {
    if (checkFour(col)) {

        isItWin = true;
        endGame = true;
        return true;
    }
    return false;
}

int ConnectFour::checkFourVertical(int col) {
    int i;
    int j;
    int k;

    for (i = 0; 3 == gameCells[i][col].getCellVal() && i < boardSize.height; ++i);
    int lineCheck = 1; //Counts last played cell as one

    /* Scans to the up */
    for (k = col, j = i; j < boardSize.height - 1 && gameCells[j][k].getCellVal() == gameCells[j + 1][k].getCellVal();
         j++, lineCheck++);
    winningCoords.startingC[0] = j;
    winningCoords.startingC[1] = k;

    /* Scans to the down */
    for (j = i; j > 1 && gameCells[j][k].getCellVal() == gameCells[j - 1][k].getCellVal(); j--, lineCheck++);
    winningCoords.endingC[0] = j;
    winningCoords.endingC[1] = k;

    return lineCheck;
}

int ConnectFour::checkFourHorizantal(int col) {
    int i;
    int j;
    int k;

    for (i = 0; 3 == gameCells[i][col].getCellVal() && i < boardSize.height; ++i);
    int lineCheck = 1; // Counts last played cell as one

    /* Scans to the right */
    for (j = i, k = col;
         j < boardSize.height && k < boardSize.width - 1 && gameCells[j][k].getCellVal() == gameCells[j][k + 1].getCellVal();
         k++, lineCheck++);
    winningCoords.startingC[0] = j;
    winningCoords.startingC[1] = k;

    /* Scans to the left */
    for (j = i, k = col; k > 1 && gameCells[j][k].getCellVal() == gameCells[j][k - 1].getCellVal();
         k--, lineCheck++);
    winningCoords.endingC[0] = j;
    winningCoords.endingC[1] = k;

    return lineCheck;
}

int ConnectFour::checkFourDiagonal1(int col) {
    int i;
    int j;
    int k;

    for (i = 0; 3 == gameCells[i][col].getCellVal() && i < boardSize.height - 1; ++i);
    int lineCheck = 1; // Counts last played cell as one

    /* Scans to the down right */
    for (j = i, k = col;
         j < boardSize.height - 1 && k < boardSize.width - 1 && gameCells[j][k].getCellVal() == gameCells[j + 1][k + 1].getCellVal();
         k++, j++, lineCheck++);
    winningCoords.startingC[0] = j;
    winningCoords.startingC[1] = k;

    /* Scans to the up left */
    for (j = i, k = col;
         k > 1 && j > 1 && gameCells[j][k].getCellVal() == gameCells[j - 1][k - 1].getCellVal();
         k--, j--, lineCheck++);
    winningCoords.endingC[0] = j;
    winningCoords.endingC[1] = k;

    return lineCheck;
}

int ConnectFour::checkFourDiagonal2(int col) {
    int i;
    int j;
    int k;

    for (i = 0; 3 == gameCells[i][col].getCellVal() && i < boardSize.height; ++i);
    int lineCheck = 1; // Counts last played cell as one

    /* Scans to the upper right */
    for (j = i, k = col;
         j > 1 && k < boardSize.width - 1 && gameCells[j][k].getCellVal() == gameCells[j - 1][k + 1].getCellVal();
         k++, j--, lineCheck++);
    winningCoords.startingC[0] = j;
    winningCoords.startingC[1] = k;

    /* Scans to the down left */
    for (j = i, k = col;
         k > 1 && j < boardSize.height - 1 && gameCells[j][k].getCellVal() == gameCells[j + 1][k - 1].getCellVal();
         k--, j++, lineCheck++);
    winningCoords.endingC[0] = j;
    winningCoords.endingC[1] = k;

    return lineCheck;
}

int ConnectFour::checkFour(int col) {
    if (checkFourVertical(col) >= 4) {
        for (int i = winningCoords.endingC[0], count = 0; i <= winningCoords.startingC[0] && count < 4; ++i, ++count) {
            gameCells[i][col].setCellVal(gameCells[i][col].getCellVal() == 1 ? 4 : 5);
        }
        return 1;
    }

    if (checkFourHorizantal(col) >= 4) {
        for (int i = winningCoords.endingC[1], count = 0; i <= winningCoords.startingC[1] && count < 4; ++i, ++count) {
            gameCells[winningCoords.startingC[0]][i].setCellVal(gameCells[winningCoords.startingC[0]][i].getCellVal() == 1 ? 4 : 5);
        }
        return 2;
    }

    if (checkFourDiagonal1(col) >= 4) {
        for (int i = winningCoords.startingC[0], count = 0, j = winningCoords.startingC[1];
             i >= winningCoords.endingC[0] && j >= winningCoords.endingC[1] && count < 4;
             --i, --j, ++count) {
            gameCells[i][j].setCellVal(gameCells[i][j].getCellVal() == 1 ? 4 : 5);
        }
        return 3;
    }

    if (checkFourDiagonal2(col) >= 4) {
        for (int i = winningCoords.startingC[0], j = winningCoords.startingC[1], count = 0;
             i <= winningCoords.endingC[0] && j >= winningCoords.endingC[1] && count < 4;
             ++i, --j, ++count) {
            gameCells[i][j].setCellVal(gameCells[i][j].getCellVal() == 1 ? 4 : 5);
        }
        return 4;
    }
    return 0;
}

int ConnectFour::playGame() {
    int numOfMoves = 0;
    int playerMoveCol = 0;
    int currentPlayer = 1;
    isItWin = false;
    if(!gameInitialized){
        takeGameInfo();
        initGame();
    }
    while (!endGame) {

        cout << (1 == currentPlayer ? "-Player 1's turn-" : (15 == currentPlayer) ? "-Player 2's turn-" : "Computers move:")
             << endl;


        if (aiEnable && currentPlayer == 10){
            play(currentPlayer);
        }
        else {
            do {
                display(); /* Display the board when player needs */
                playerMoveCol = getMove();

                if (playerMoveCol == 35)
                    saveManagement(LOAD, saveFileName, numOfMoves, currentPlayer);

                else if (playerMoveCol == 45)
                    saveManagement(SAVE, saveFileName, numOfMoves, currentPlayer);
            } while (35 == playerMoveCol || 45 == playerMoveCol);

            play(currentPlayer,playerMoveCol);
        }
        currentPlayer =
                1 == currentPlayer ? aiEnable ? 10 : 15 : 1; /* AI is 10, Second Player is 15 and First Player is 1 */
        numOfMoves++;
        livingCellCount++;
        if (maxNumOfMoves == numOfMoves) endGame = true; /* DRAW Check */

        isItTheEnd(playerMoveCol); /* If its a win, ends loop and makes sure it doesnt registers it as DRAW */
    }
    display();
    if (maxNumOfMoves == numOfMoves && !isItWin) cout << "DRAW" << endl;
    else {
        if (gameCells[winningCoords.startingC[0]][winningCoords.startingC[1]].getCellVal() == 4 ||
            gameCells[winningCoords.startingC[0]][winningCoords.startingC[1]].getCellVal() == 1)
            cout << "Player 1 Wins!!!" << endl;
        else if (aiEnable) cout << "Computer Wins!!!" << endl;
        else cout << "Player 2 Wins!!!" << endl;
    }

}

int ConnectFour::getMove() {
    auto isInputValid = false;
    char move = '0';
    auto lastCol = 'A';
    string command;
    string line;

    for (int i = 1; i < boardSize.width; ++i) ++lastCol; /* For displaying Last column */
    while (!isInputValid) {
        cout << "Select the column you want to play in (A to " << lastCol
             << ") or LOAD/SAVE a game save file (LOAD/SAVE save.txt): ";
        getline(cin, line);
        if (line.size() <= 1) {
            move = line[0];
            upperCase(move); /* Input is not case sensitive */
        } else {
            command = line.substr(0, 4);
            if (command == "LOAD") {
                saveFileName = line.substr(5, line.size() + 1);
                return 35;
            } else if (command == "SAVE") {
                saveFileName = line.substr(5, line.size() + 1);
                return 45;
            }

        }
        isInputValid = (move >= 'A' && move <= lastCol);
        if (!isInputValid) cerr << "Please select a valid column! (A to " << lastCol << ")" << endl;
        else {

             if (boardSize.isColFull[move - 'A']) {
                 isInputValid = false;
                 cerr << "The column you choose is full. Please try another column." << endl;
             }
        }
    }
    return move - 'A';

}

bool ConnectFour::compareGames(const ConnectFour &other) const {
    int numOfFilledCells1 = 0;
    int numOfFilledCells2 = 0;
    for (int i = 0; i < boardSize.height; ++i) {
        for (int j = 0; j < boardSize.width; ++j) {
            if (gameCells[i][j].getCellVal() != 3)
                numOfFilledCells1++;
        }
    }

    for (int i = 0; i < other.boardSize.height; ++i) {
        for (int j = 0; j < other.boardSize.width; ++j) {
            if (other.gameCells[i][j].getCellVal() != 3)
                numOfFilledCells2++;
        }
    }

    return maxNumOfMoves - numOfFilledCells1 < other.maxNumOfMoves - numOfFilledCells2;
}

void ConnectFour::play(int player, int column) {
    if (column == -1) column = getAiMove();

    int i;
    for (i = 0; i < boardSize.height && 3 == gameCells[i][column].getCellVal(); ++i); /* Gets last filled row */

    gameCells[i - 1][column].setCellVal((1 == player) ? 1 : 2);
    if (i == 1) boardSize.isColFull[column] = true;

}

int ConnectFour::getAiMove() {
    int column;
    int state = 0;

    while (boardSize.isColFull[column = rand() % boardSize.width]);
    /* Random computer move in case of states don't evaluate a value */

    state = evaluateBoard();

    switch (state) {
        case 1: /* Player or computer is winning vertically */
            column = aiWinningCoords.endingC[1];
            break;
        case 2: /* Player or computer is winning horizontally */
            if (aiWinningCoords.endingC[1] + 1 < boardSize.width - 1 && !boardSize.isColFull[aiWinningCoords.endingC[1] + 1]
                && gameCells[aiWinningCoords.endingC[0]][aiWinningCoords.endingC[1] + 1].getCellVal() == 3 &&
                gameCells[aiWinningCoords.endingC[0] + 1][aiWinningCoords.endingC[1] + 1].getCellVal() != 3
                )
                column = aiWinningCoords.endingC[1] + 1;
            else if (aiWinningCoords.startingC[1] > 1 && !boardSize.isColFull[aiWinningCoords.startingC[1] - 1] &&
                    gameCells[aiWinningCoords.startingC[0]][aiWinningCoords.startingC[1] - 1].getCellVal() == 3 &&
                     gameCells[aiWinningCoords.startingC[0] + 1][aiWinningCoords.startingC[1] - 1].getCellVal() != 3)
                column = aiWinningCoords.startingC[1] - 1;

            break;
        case 3: /* Player or computer is winning diagonal down left to the upper right */
            if (aiWinningCoords.endingC[1] + 1 < boardSize.width - 1 && !boardSize.isColFull[aiWinningCoords.endingC[1] + 1] &&
                    gameCells[aiWinningCoords.endingC[0] + 1][aiWinningCoords.endingC[1] + 1].getCellVal() == 3 &&
                gameCells[aiWinningCoords.endingC[0] + 2][aiWinningCoords.endingC[1] + 1].getCellVal() != 3)
                column = aiWinningCoords.endingC[1] + 1;
            else if (aiWinningCoords.startingC[1] > 1 && !boardSize.isColFull[aiWinningCoords.startingC[1] - 1] &&
                    gameCells[aiWinningCoords.startingC[0] - 1][aiWinningCoords.startingC[1] - 1].getCellVal() == 3 &&
                     gameCells[aiWinningCoords.startingC[0] - 2][aiWinningCoords.startingC[1] - 1].getCellVal() != 3)
                column = aiWinningCoords.startingC[1] - 1;
            break;
        case 4: /* Player or computer is winning diagonal up left to the down right */
            if (aiWinningCoords.endingC[1] > 1 && !boardSize.isColFull[aiWinningCoords.endingC[1] + 1]
                && gameCells[aiWinningCoords.endingC[0] + 1][aiWinningCoords.endingC[1] - 1].getCellVal() == 3 &&
                gameCells[aiWinningCoords.endingC[0] + 2][aiWinningCoords.endingC[1] - 1].getCellVal() != 3)
                column = aiWinningCoords.endingC[1] + 1;
            else if (aiWinningCoords.startingC[1] + 1 < boardSize.width - 1 && !boardSize.isColFull[aiWinningCoords.startingC[1] - 1]
                    && gameCells[aiWinningCoords.startingC[0] - 1][aiWinningCoords.startingC[1] + 1].getCellVal() == 3 &&
                     gameCells[aiWinningCoords.startingC[0] - 2][aiWinningCoords.startingC[1] + 1].getCellVal() != 3)
                column = aiWinningCoords.startingC[1] - 1;
            break;
        default:
            break;
    }

    return column;
}

int ConnectFour::evaluateBoard() {
    int lineState = 0;
    if (aiScanVertical() >= 2) lineState = 1;
    else if (aiScanHorizantal() >= 2) lineState = 2;
    else if (aiScanDiagonal1() >= 2) lineState = 3;
    else if (aiScanDiagonal2() >= 2) lineState = 4;
    return lineState;
}

int ConnectFour::aiScanVertical() {
    int lineCheck = 1; // Counts last played cell as one
    int longestLine = 0, tempBlockLine = 0;
    int endingTempBlockCoords[2];

    /* Block attempt */
    for (int i = 0; i < boardSize.width; ++i) {
        for (int j = 0; j < boardSize.height - 1; ++j) {
            while (gameCells[j][i].getCellVal() == gameCells[j + 1][i].getCellVal() &&
                    gameCells[j][i].getCellVal() == 1 && !boardSize.isColFull[i]) {
                ++lineCheck;
                if (lineCheck >= tempBlockLine) {
                    tempBlockLine = lineCheck;
                    endingTempBlockCoords[0] = j + 1;
                    endingTempBlockCoords[1] = i;
                }
                if(j < boardSize.height - 1)
                    ++j;
            }
            lineCheck = 1;
        }
    }

    /* Win attempt */
    for (int i = 0; i < boardSize.width; ++i) {
        for (int j = 0; j < boardSize.height - 1; ++j) {
            while (gameCells[j][i].getCellVal() == gameCells[j + 1][i].getCellVal() &&
                    gameCells[j][i].getCellVal() == 2 && !boardSize.isColFull[i]) {
                ++lineCheck;
                if (lineCheck >= longestLine) {
                    longestLine = lineCheck;
                    aiWinningCoords.endingC[0] = j + 1;
                    aiWinningCoords.endingC[1] = i;
                }
                if(j < boardSize.height - 1)
                    ++j;
            }
            lineCheck = 1;
        }
    }

    if (tempBlockLine > 2) {
        aiWinningCoords.endingC[0] = endingTempBlockCoords[0];
        aiWinningCoords.endingC[1] = endingTempBlockCoords[1];
        longestLine = tempBlockLine;
    }

    aiWinningCoords.startingC[0] = aiWinningCoords.endingC[0] - (lineCheck - 1);
    aiWinningCoords.startingC[1] = aiWinningCoords.endingC[1];


    return longestLine;
}

int ConnectFour::aiScanHorizantal() {
    int lineCheck = 1; // Counts last played cell as one
    int longestLine = 0, tempBlockLine = 0;
    int endingTempBlockCoords[2];

    /* Block attempt */
    for (int i = 0; i < boardSize.height; ++i) {
        for (int j = 0; j < boardSize.width - 1; ++j) {
            while (gameCells[i][j].getCellVal() == gameCells[i][j + 1].getCellVal() &&
                    gameCells[i][j].getCellVal() == 1) {
                ++lineCheck;
                if (lineCheck >= tempBlockLine) {
                    tempBlockLine = lineCheck;
                    endingTempBlockCoords[0] = i;
                    endingTempBlockCoords[1] = j + 1;
                }
                if(j < boardSize.width - 1)
                    ++j;
            }
        }
    }

    /* Win attempt */
    for (int i = 0; i < boardSize.height; ++i) {
        for (int j = 0; j < boardSize.width - 1; ++j) {
            while (gameCells[i][j].getCellVal() == gameCells[i][j + 1].getCellVal() &&
                    gameCells[i][j].getCellVal() == 1) {
                ++lineCheck;
                if (lineCheck >= longestLine) {
                    longestLine = lineCheck;
                    aiWinningCoords.endingC[0] = i;
                    aiWinningCoords.endingC[1] = j + 1;
                }
                if(j < boardSize.width - 1)
                    ++j;
            }
        }
    }

    if (tempBlockLine > 2) {
        aiWinningCoords.endingC[0] = endingTempBlockCoords[0];
        aiWinningCoords.endingC[1] = endingTempBlockCoords[1];
        longestLine = tempBlockLine;
    }

    aiWinningCoords.startingC[0] = aiWinningCoords.endingC[0];
    aiWinningCoords.startingC[1] = aiWinningCoords.endingC[1] - (lineCheck - 1);

    return longestLine;
}

int ConnectFour::aiScanDiagonal1() {
    int lineCheck = 1; // Counts last played cell as one
    int longestLine = 0, tempBlockLine = 0;
    int endingTempBlockCoords[2];

    /* Block attempt */
    for (int i = 0; i < boardSize.width - 1; ++i) {
        for (int j = 0; j < boardSize.height - 1; ++j) {
            while (gameCells[j][i].getCellVal() == gameCells[j + 1][i + 1].getCellVal() &&
                    gameCells[j][i].getCellVal() == 1) {
                ++lineCheck;
                if (lineCheck >= tempBlockLine) {
                    tempBlockLine = lineCheck;
                    endingTempBlockCoords[0] = j + 1;
                    endingTempBlockCoords[1] = i + 1;
                }
                if(j < boardSize.width - 1)
                    ++i;
                if(j < boardSize.height - 1)
                    ++j;
            }
            if (lineCheck >= 2) {
                j = j - (lineCheck - 1);
                i = i - (lineCheck - 1);
            }
            lineCheck = 1;
        }
    }

    /* Win attempt */
    for (int i = 0; i < boardSize.width - 1; ++i) {
        for (int j = 0; j < boardSize.height - 1; ++j) {
            while (gameCells[j][i].getCellVal() == gameCells[j + 1][i + 1].getCellVal() &&
                    gameCells[j][i].getCellVal() == 1) {
                ++lineCheck;
                if (lineCheck >= longestLine) {
                    longestLine = lineCheck;
                    aiWinningCoords.endingC[0] = j + 1;
                    aiWinningCoords.endingC[1] = i + 1;
                }
                if(j < boardSize.width - 1)
                    ++i;
                if(j < boardSize.height - 1)
                    ++j;
            }
            if (lineCheck >= 2) {
                j = j - (lineCheck - 1);
                i = i - (lineCheck - 1);
            }
            lineCheck = 1;
        }
    }

    if (tempBlockLine > 2) {
        aiWinningCoords.endingC[0] = endingTempBlockCoords[0];
        aiWinningCoords.endingC[1] = endingTempBlockCoords[1];
        longestLine = tempBlockLine;
    }

    aiWinningCoords.startingC[0] = aiWinningCoords.endingC[0] - (lineCheck - 1);
    aiWinningCoords.startingC[1] = aiWinningCoords.endingC[1] - (lineCheck - 1);

    return longestLine;
}

int ConnectFour::aiScanDiagonal2() {
    int lineCheck = 1; // Counts last played cell as one
    int longestLine = 0, tempBlockLine = 0;
    int endingTempBlockCoords[2];

    /* Block attempt */
    for (int i = 1; i < boardSize.width; ++i) {
        for (int j = 0; j < boardSize.height - 1; ++j) {

            while (gameCells[j][i].getCellVal() == gameCells[j + 1][i - 1].getCellVal() &&
                    gameCells[j][i].getCellVal() == 1) {
                ++lineCheck;
                if (lineCheck >= tempBlockLine) {
                    tempBlockLine = lineCheck;
                    endingTempBlockCoords[0] = j + 1;
                    endingTempBlockCoords[1] = i - 1;
                }
                if(i > 0)
                    --i;
                if(j < boardSize.height - 1)
                    ++j;
            }
            if (lineCheck >= 2) {
                j = j - (lineCheck - 1);
                i = i + (lineCheck - 1);
            }
            lineCheck = 1;
        }
    }

    /* Win attempt */
    for (int i = 1; i < boardSize.width; ++i) {
        for (int j = 0; j < boardSize.height - 1; ++j) {

            while (gameCells[j][i].getCellVal() == gameCells[j + 1][i - 1].getCellVal() &&
                    gameCells[j][i].getCellVal() == 1) {
                ++lineCheck;
                if (lineCheck >= longestLine) {
                    longestLine = lineCheck;
                    aiWinningCoords.endingC[0] = j + 1;
                    aiWinningCoords.endingC[1] = i - 1;
                }
                if(i > 0)
                    --i;
                if(j < boardSize.height - 1)
                    ++j;
            }
            if (lineCheck >= 2) {
                j = j - (lineCheck - 1);
                i = i + (lineCheck - 1);
            }
            lineCheck = 1;
        }
    }

    if (tempBlockLine > 2) {
        aiWinningCoords.endingC[0] = endingTempBlockCoords[0];
        aiWinningCoords.endingC[1] = endingTempBlockCoords[1];
        longestLine = tempBlockLine;
    }

    aiWinningCoords.startingC[0] = aiWinningCoords.endingC[0] - (lineCheck - 1);
    aiWinningCoords.startingC[1] = aiWinningCoords.endingC[1] + (lineCheck - 1);
    return longestLine;
}

bool ConnectFour::playTurn(int &numOfMoves) {
    int playerMoveCol = 0;
    int currentPlayer = 1;
    int turn = 0;
    isItWin = false;
    while (!endGame && turn < 2) {
        cout << (1 == currentPlayer ? "-Player 1's turn-" : (15 == currentPlayer) ? "-Player 2's turn-" : "Computers move:")
             << endl;
        if (aiEnable && currentPlayer == 10){
            play(currentPlayer);
        }
        else {
            do {
                display(); /* Display the board when player needs */
                playerMoveCol = getMove();

                if (playerMoveCol == 35)
                    saveManagement(LOAD, saveFileName, numOfMoves, currentPlayer);

                else if (playerMoveCol == 45)
                    saveManagement(SAVE, saveFileName, numOfMoves, currentPlayer);
            } while (35 == playerMoveCol || 45 == playerMoveCol);

            play(currentPlayer,playerMoveCol);
        }
        currentPlayer =
                1 == currentPlayer ? aiEnable ? 10 : 15 : 1; /* AI is 10, Second Player is 15 and First Player is 1 */
        numOfMoves++;
        turn++;
        livingCellCount++;
        if (maxNumOfMoves == numOfMoves) endGame = true; /* DRAW Check */

        isItTheEnd(playerMoveCol); /* If its a win, ends loop and makes sure it doesnt registers it as DRAW */
    }
    display();
    if (maxNumOfMoves == numOfMoves && !isItWin) cout << "DRAW" << endl;
    else if (isItWin){
        if (gameCells[winningCoords.startingC[0]][winningCoords.startingC[1]].getCellVal() == 4 ||
            gameCells[winningCoords.startingC[0]][winningCoords.startingC[1]].getCellVal() == 1)
            cout << "Player 1 Wins!!!" << endl;
        else if (aiEnable) cout << "Computer Wins!!!" << endl;
        else cout << "Player 2 Wins!!!" << endl;
    }
    return endGame;
}


ConnectFour::Cell::Cell() {
    setCell(0,0,0);
}


ConnectFour::Cell::Cell(int col, int row, int p) {
    setCell(col, row, p);
}

void ConnectFour::Cell::setCell(int col, int row, int p) {
    colNum = col;
    rowNum = row;
    player = p;

    switch (p){
        case 1:
            value = 1;
            break;
        case 10:
            value = 2;
            break;
        case 15:
            value = 2;
            break;
        default:
            value = 3;
            break;
    }
}

void ConnectFour::Cell::setCellVal(int val) {
    value = val;
}

void playFiveCF() {
    ConnectFour game[5];
    int moveCount[5] = {0, 0, 0, 0, 0};
    bool gameDone[5] = {false, false, false, false, false};
    for (int i = 0; i < 5; ++i)
        if(!game[i].isGameInit()){
            cout << "Game " << i + 1 << ": ";
            game[i] = ConnectFour(true);
        }

    int endGame = 0;
    while(endGame < 5){
        for (int i = 0; i < 5; ++i)
            if (!gameDone[i]){
                cout << "Game " << i + 1 << ":" << endl;
                gameDone[i] = game[i].playTurn(moveCount[i]);
                if(gameDone[i]) {
                    endGame++;
                    cout << "Game " << i + 1 << " is ended." << endl;
                }
            }
    }
    cout << "All games are ended!" << endl;
}
