//
// Created by cecen on 1.11.2017.
//

#include "ConnectFour.h"

int ConnectFour::livingCellCount = 0;
void upperCase(char &letter);

/* ConnectFour class Constructors, Copy Constructor, overloads and destructor (Big three implementation included) */
ConnectFour::ConnectFour(){ // Default 5x5 game initialization
    boardSize.height = 5;
    boardSize.width = 5;

    initGame();
}

ConnectFour::ConnectFour(bool askFile) {
    /* This constructor relies on calling playGame function so that playgame function can ask board name */
}

ConnectFour::ConnectFour (string fileName): boardFile(fileName) { // board name paramter constructor
    takeGameInfo(true, false); // Also initializes for this particular constructor
}

ConnectFour::ConnectFour(int height, int width, bool isCompPlaying) { // Classic 3 parameter constructor
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

ConnectFour::~ConnectFour () {
    for (int i = 0; i < boardSize.height; ++i) delete []gameCells[i];
    delete []gameCells;

    delete []boardSize.isColFull;
}

ConnectFour &ConnectFour::operator= (const ConnectFour &other) {
    if( this != &other ){
        if(gameInitialized){
            for (int i = 0; i < boardSize.height + 1; ++i) delete []gameCells[i];
            delete []gameCells;
            delete []boardSize.isColFull;
            gameInitialized = false;
        }
        boardSize.height = other.boardSize.height;
        boardSize.width = other.boardSize.width;
        boardSize.isColFull = new bool[boardSize.width];
        for (int j = 0; j < boardSize.width; ++j) boardSize.isColFull[j] = other.boardSize.isColFull[j];

        initGame();

        for (int k = 0; k < boardSize.height; ++k)
            for (int i = 0; i < boardSize.width; ++i)
                gameCells[k][i] = other.gameCells[k][i];

        winningCoords = other.winningCoords;
        aiWinningCoords = other.aiWinningCoords;
        maxNumOfMoves = other.maxNumOfMoves;
        isItWin = other.isItWin;
        endGame = other.endGame;
        aiEnable = other.aiEnable;
        gameInitialized = other.gameInitialized;
        saveFileName = other.saveFileName;
        boardFile = other.boardFile;
    }
    return *this;
}

bool ConnectFour::operator==(const ConnectFour & other) const{
	if(boardSize.width == other.boardSize.width && boardSize.height == other.boardSize.height){
        for (int i = 0; i < boardSize.width ; ++i)
            if(boardSize.isColFull[i] != other.boardSize.isColFull[i])
                return false;

        if(isItWin == other.isItWin && endGame == other.endGame && aiEnable == other.aiEnable &&
           gameInitialized == other.gameInitialized)
            for (int i = 0; i < boardSize.height; ++i)
                for (int j = 0; j < boardSize.width; ++j)
                    if (gameCells[i][j] != other.gameCells[i][j])
                        return false;
        return true;
    }
    return false;
}

ConnectFour::ConnectFour (const ConnectFour &other) {
    *this = other;
}

bool ConnectFour::operator!= (const ConnectFour & other) const {
    return !(*this == other);
}
/* - */

/* Cell class Constructors, overloads. Default Big Three works normally */
ConnectFour::Cell::Cell() {
    setCell(0,0,0);
}

ConnectFour::Cell::Cell(int col, int row, int p) {
    setCell(col, row, p);
}

ConnectFour::Cell::Cell (int val) {
    value = val;
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

ConnectFour::Cell & ConnectFour::Cell::operator= (int val) {
    value = val;
    return *this;
}

bool ConnectFour::Cell::operator== (const ConnectFour::Cell & other) const{
    return value == other.value;
}

bool ConnectFour::Cell::operator!= (const ConnectFour::Cell & other) const{
    return *this == other;
}

ConnectFour::Cell ConnectFour::Cell::operator++ () {
    switch (value){
        case 1:
            value++;
            break;
        case 2:
            value++;
            break;
        case 3:
            value = 1;
            break;
        default:
            break;
    }
    return *this;
}

ConnectFour::Cell ConnectFour::Cell::operator++ (int) {
    Cell tmp = value;
    ++*this;
    return tmp;
}

ConnectFour::Cell ConnectFour::Cell::operator-- () {
    switch (value){
        case 1:
            value = 3;
            break;
        case 2:
            value--;
            break;
        case 3:
            value--;
            break;
        default:
            break;
    }
    return *this;
}

ConnectFour::Cell ConnectFour::Cell::operator-- (int) {
    Cell tmp = value;
    --*this;
    return tmp;
}

istream & operator>> (istream &input, ConnectFour::Cell & other) {
    input >> other.value;
    return input;
}

ostream & operator<< (ostream &output, ConnectFour::Cell & other) {
    output << other.value;
    return output;
}
/* - */

commandType ConnectFour::saveManagement(commandType command, string saveFileName, int moveCount, int player) {
    if(command == LOAD){
        ifstream saveFile(saveFileName); /* Opens file as input stream */
        int isColFull;
        if (!saveFile.fail()) {
            saveFile >> boardSize.height
                     >> boardSize.width;
            initGame();
            livingCellCount -= moveCount;
            for (int i = 0; i < boardSize.width; ++i) {
                saveFile >> isColFull;
                boardSize.isColFull[i] = isColFull != 0;
                for (int j = 0; j < boardSize.height; ++j) saveFile >> gameCells[j][i]; // Stream extraction overload for Cell class
            }
            saveFile >> aiEnable
                     >> moveCount
                     >> player;
            livingCellCount += moveCount;
            saveFile.close();
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
                    saveFile << gameCells[j][i] << endl; // Stream insertion overload for Cell class
            }
            saveFile << aiEnable << endl
                     << moveCount << endl
                     << player << endl;
            saveFile.close();
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
                default: /* Really empty cell */
                    cout << "  ";
                    break;
            }
        cout << endl;
    }
    cout << "Living cells: " << livingCells() << endl;
}

void ConnectFour::initGame() {
    if(gameInitialized){ /* This deleting process works for reinitialization of the object */
        for (int i = 0; i < boardSize.height + 1; ++i)
            delete [] gameCells[i];
        delete [] gameCells;
        gameCells = nullptr;

        delete [] boardSize.isColFull;
        boardSize.isColFull = nullptr;
    }

    /* dynamic memory allocations for initial state of object */
    gameCells = new Cell*[boardSize.height + 1];
    for (int i = 0; i < boardSize.height + 1; ++i){
        gameCells[i] = new Cell[boardSize.width + 1];
    }

    for (int l = 0; l < boardSize.height; ++l) gameCells[l][boardSize.width] = 0;
    for (int l = 0; l < boardSize.width; ++l) gameCells[boardSize.height][l] = 0;

    boardSize.isColFull = new bool[boardSize.width];
    for (int k = 0; k < boardSize.width; ++k)
        boardSize.isColFull[k] = false;
    /* - */

    maxNumOfMoves = boardSize.height * boardSize.width; // This one will be reinitialized in playGame functions
    endGame = false;
    isItWin = false;
    gameInitialized = true; // This switch is also important for all dynamic memory allocations. DONT FORGET TO HANDLE THIS!!
}

void ConnectFour::takeGameInfo (bool shapeBoard, bool justAI) {
    if(!justAI) {
        if (shapeBoard) {  /* Board initialization goes here for reading from file specifically */
            int height = 0, width = 0;
            char ** tempArray = nullptr; // Expands for every line read. Temporary array. Later will be transferred to actual Cell array
            char ** tmpArr = nullptr; // Help expanding other temporary array
            ifstream inputFile(boardFile);
            if (!inputFile.fail()){ // Reading as char dynamic array
                while(!inputFile.eof()){
                    if(tempArray != nullptr){ // Clears actual temporary array holds it temporarily.
                        tmpArr = new char* [height];
                        for (int i = 0; i < height; ++i) {
                            tmpArr[i] = new char[width];
                            for (int j = 0; j < width; ++j) tmpArr[i][j] = tempArray[i][j];
                            delete [] tempArray[i];
                        }
                        delete [] tempArray;
                    }

                    height++;

                    string line;
                    getline(inputFile, line);
                    if (height == 1) width = line.size();
                    tempArray = new char*[height];
                    for (int j = 0; j < height; ++j) tempArray[j] = new char[width];

                    if(tmpArr != nullptr){
                        for (int j = 0; j < height - 1; ++j) {
                            for (int i = 0; i < width && j < height - 1; ++i) tempArray[j][i] = tmpArr[j][i];
                            delete [] tmpArr[j];
                        }
                        delete [] tmpArr;
                    }
                    for (int i = 0; i < width; ++i) tempArray[height - 1][i] = line[i];
                }
                inputFile.close();
                // Converting characters to Cell type dynamic array (initializing cell array)
                boardSize.height = --height;
                boardSize.width = width;
                gameCells = new Cell*[height + 1];
                for (int k = 0; k < height; ++k) {
                    gameCells[k] = new Cell[width + 1];
                    for (int i = 0; i < width; ++i) {
                        gameCells[k][i] = tempArray[k][i] == '*' ? 3 : 0;
                    }
                }
                gameCells[height] = new Cell[width + 1];
                boardSize.isColFull = new bool[width];
                for (int l = 0; l < width + 1; ++l) boardSize.isColFull[l] = false;

                /* maxNumOfMoves will initialized in playGame func for this config */
                endGame = false;
                isItWin = false;
                gameInitialized = true;

                for (int i = 0; i < height; ++i) delete []tempArray[i]; 
                delete []tempArray; /* Temp array is cleared */
            }
        } else { /* Takes height and width instead of board */
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
        }
    }
    /* Reading game mode (AI or PvP) */
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
    int i,j,k,l;

    for (l = 0; l < boardSize.height && gameCells[l][col] == 0; ++l);
    for (i = l; gameCells[i][col] == 3 && i < boardSize.height; ++i);
    int lineCheck = 1; //Counts last played cell as one

    /* Scans to the up */
    for (k = col, j = i; j < boardSize.height - 1 && gameCells[j][k] == gameCells[j + 1][k];
         j++, lineCheck++);
    winningCoords.startingC[0] = j;
    winningCoords.startingC[1] = k;

    /* Scans to the down */
    for (j = i; j > 0 && gameCells[j][k] == gameCells[j - 1][k]; j--, lineCheck++);
    winningCoords.endingC[0] = j;
    winningCoords.endingC[1] = k;

    return lineCheck;
}

int ConnectFour::checkFourHorizontal (int col) {
    int i,j,k,l;

    for (l = 0; l < boardSize.height && gameCells[l][col] == 0; ++l);
    for (i = l; gameCells[i][col] == 3 && i < boardSize.height; ++i);
    int lineCheck = 1; // Counts last played cell as one

    /* Scans to the right */
    for (j = i, k = col;
         j < boardSize.height && k < boardSize.width - 1 && gameCells[j][k] == gameCells[j][k + 1];
         k++, lineCheck++);
    winningCoords.startingC[0] = j;
    winningCoords.startingC[1] = k;

    /* Scans to the left */
    for (j = i, k = col; k > 0 && gameCells[j][k] == gameCells[j][k - 1];
         k--, lineCheck++);
    winningCoords.endingC[0] = j;
    winningCoords.endingC[1] = k;

    return lineCheck;
}

int ConnectFour::checkFourDiagonal1(int col) {
    int i,j,k,l;

    for (l = 0; l < boardSize.height && gameCells[l][col] == 0; ++l);
    for (i = l; gameCells[i][col] == 3 && i < boardSize.height - 1; ++i);
    int lineCheck = 1; // Counts last played cell as one

    /* Scans to the down right */
    for (j = i, k = col;
         j < boardSize.height - 1 && k < boardSize.width - 1 && gameCells[j][k] == gameCells[j + 1][k + 1];
         k++, j++, lineCheck++);
    winningCoords.startingC[0] = j;
    winningCoords.startingC[1] = k;

    /* Scans to the up left */
    for (j = i, k = col;
         k > 0 && j > 0 && gameCells[j][k] == gameCells[j - 1][k - 1];
         k--, j--, lineCheck++);
    winningCoords.endingC[0] = j;
    winningCoords.endingC[1] = k;

    return lineCheck;
}

int ConnectFour::checkFourDiagonal2(int col) {
    int i,j,k,l;

    for (l = 0; l < boardSize.height && gameCells[l][col] == 0; ++l);
    for (i = l; gameCells[i][col] == 3 && i < boardSize.height; ++i);
    int lineCheck = 1; // Counts last played cell as one

    /* Scans to the upper right */
    for (j = i, k = col;
         j > 0 && k < boardSize.width - 1 && gameCells[j][k] == gameCells[j - 1][k + 1];
         k++, j--, lineCheck++);
    winningCoords.startingC[0] = j;
    winningCoords.startingC[1] = k;

    /* Scans to the down left */
    for (j = i, k = col;
         k > 0 && j < boardSize.height - 1 && gameCells[j][k] == gameCells[j + 1][k - 1];
         k--, j++, lineCheck++);
    winningCoords.endingC[0] = j;
    winningCoords.endingC[1] = k;

    return lineCheck;
}

int ConnectFour::checkFour(int col) {
    if (checkFourVertical(col) >= 4) {
        for (int i = winningCoords.endingC[0], count = 0; i <= winningCoords.startingC[0] && count < 4; ++i, ++count) {
            gameCells[i][col] = gameCells[i][col] == 1 ? 4 : 5;
        }
        return 1;
    }

    if (checkFourHorizontal(col) >= 4) {
        for (int i = winningCoords.endingC[1], count = 0; i <= winningCoords.startingC[1] && count < 4; ++i, ++count) {
            gameCells[winningCoords.startingC[0]][i] = gameCells[winningCoords.startingC[0]][i] == 1 ? 4 : 5;
        }
        return 2;
    }

    if (checkFourDiagonal1(col) >= 4) {
        for (int i = winningCoords.startingC[0], count = 0, j = winningCoords.startingC[1];
             i >= winningCoords.endingC[0] && j >= winningCoords.endingC[1] && count < 4;
             --i, --j, ++count) {
            gameCells[i][j] = gameCells[i][j] == 1 ? 4 : 5;
        }
        return 3;
    }

    if (checkFourDiagonal2(col) >= 4) {
        for (int i = winningCoords.startingC[0], j = winningCoords.startingC[1], count = 0;
             i <= winningCoords.endingC[0] && j >= winningCoords.endingC[1] && count < 4;
             ++i, --j, ++count) {
            gameCells[i][j] = gameCells[i][j] == 1 ? 4 : 5;
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
        cout << "Enter board file name: ";
        cin >> boardFile;
        if(!cin.fail()) takeGameInfo(true,false); /* If file name successfully read initializes board according to it */ 
        else{
            cin.clear(); // Clears bad state flag
            takeGameInfo(false, false);
            initGame();
        }

    }

	maxNumOfMoves = 0; /* maxnumofmoves initialization for draw state */
    for (int i = 0; i < boardSize.height; ++i)
        for (int j = 0; j < boardSize.width; ++j)
            if(gameCells[i][j] == 3) maxNumOfMoves++;

    while (!endGame) {

        cout << (1 == currentPlayer ? "-Player 1's turn-" : (15 == currentPlayer) ? "-Player 2's turn-" : "Computers move:")
             << endl;


        if (aiEnable && currentPlayer == 10){
            playerMoveCol = play(currentPlayer); 
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
        if (gameCells[winningCoords.startingC[0]][winningCoords.startingC[1]] == 4 ||
            gameCells[winningCoords.startingC[0]][winningCoords.startingC[1]] == 1)
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
        if(cin.eof()){ /* Moodle da belirtildiği üzere sonsuz döngüye girmemesi için programın bitimi */
        	cerr << "Reached end of inputs!" << endl; 
        	exit(1); 
        } 
    }
    return move - 'A';

}

int ConnectFour::play (int player, int column) {
    if (column == -1) column = getAiMove();

    int i,k;
    for (k = 0; k < boardSize.height && gameCells[k][column] == 0; ++k);
    for (i = k; i < boardSize.height && gameCells[i][column] == 3; ++i); /* Gets last filled row */

    gameCells[i - 1][column] = (1 == player) ? 1 : 2;
    if (i == k + 1) boardSize.isColFull[column] = true;

    return column; // Will  be used for ai move win check

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
                && gameCells[aiWinningCoords.endingC[0]][aiWinningCoords.endingC[1] + 1] == 3 &&
                gameCells[aiWinningCoords.endingC[0]][aiWinningCoords.endingC[1] + 1] == 0 &&
                gameCells[aiWinningCoords.endingC[0] + 1][aiWinningCoords.endingC[1] + 1] != 3 &&
                gameCells[aiWinningCoords.endingC[0] + 1][aiWinningCoords.endingC[1] + 1] != 0
                    )
                column = aiWinningCoords.endingC[1] + 1;
            else if (aiWinningCoords.startingC[1] > 1 && !boardSize.isColFull[aiWinningCoords.startingC[1] - 1] &&
                     gameCells[aiWinningCoords.startingC[0]][aiWinningCoords.startingC[1] - 1] == 3 &&
                     gameCells[aiWinningCoords.startingC[0]][aiWinningCoords.startingC[1] - 1] == 0 &&
                     gameCells[aiWinningCoords.startingC[0] + 1][aiWinningCoords.startingC[1] - 1] != 3 &&
                     gameCells[aiWinningCoords.startingC[0] + 1][aiWinningCoords.startingC[1] - 1] != 0)
                column = aiWinningCoords.startingC[1] - 1;

            break;
        case 3: /* Player or computer is winning diagonal down left to the upper right */
            if (aiWinningCoords.endingC[1] + 1 < boardSize.width - 1 && !boardSize.isColFull[aiWinningCoords.endingC[1] + 1] &&
                gameCells[aiWinningCoords.endingC[0] + 1][aiWinningCoords.endingC[1] + 1] == 3 &&
                gameCells[aiWinningCoords.endingC[0] + 1][aiWinningCoords.endingC[1] + 1] == 0 &&
                gameCells[aiWinningCoords.endingC[0] + 2][aiWinningCoords.endingC[1] + 1] != 3 &&
                gameCells[aiWinningCoords.startingC[0] + 2][aiWinningCoords.startingC[1] + 1] != 0)
                column = aiWinningCoords.endingC[1] + 1;
            else if (aiWinningCoords.startingC[1] > 1 && !boardSize.isColFull[aiWinningCoords.startingC[1] - 1] &&
                     gameCells[aiWinningCoords.startingC[0] - 1][aiWinningCoords.startingC[1] - 1] == 3 &&
                     gameCells[aiWinningCoords.startingC[0] - 1][aiWinningCoords.startingC[1] - 1] == 0 &&
                     gameCells[aiWinningCoords.startingC[0] - 2][aiWinningCoords.startingC[1] - 1] != 3 &&
                     gameCells[aiWinningCoords.startingC[0] - 2][aiWinningCoords.startingC[1] - 1] != 0)
                column = aiWinningCoords.startingC[1] - 1;
            break;
        case 4: /* Player or computer is winning diagonal up left to the down right */
            if (aiWinningCoords.endingC[1] > 1 && !boardSize.isColFull[aiWinningCoords.endingC[1] + 1]
                && gameCells[aiWinningCoords.endingC[0] + 1][aiWinningCoords.endingC[1] - 1] == 3 &&
                gameCells[aiWinningCoords.endingC[0] + 1][aiWinningCoords.endingC[1] - 1] == 0 &&
                gameCells[aiWinningCoords.endingC[0] + 2][aiWinningCoords.endingC[1] - 1] != 3 &&
                gameCells[aiWinningCoords.endingC[0] + 2][aiWinningCoords.endingC[1] - 1] != 0)
                column = aiWinningCoords.endingC[1] + 1;
            else if (aiWinningCoords.startingC[1] + 1 < boardSize.width - 1 && !boardSize.isColFull[aiWinningCoords.startingC[1] - 1]
                     && gameCells[aiWinningCoords.startingC[0] - 1][aiWinningCoords.startingC[1] + 1] == 3 &&
                     gameCells[aiWinningCoords.startingC[0] - 1][aiWinningCoords.startingC[1] + 1] == 0 &&
                     gameCells[aiWinningCoords.startingC[0] - 2][aiWinningCoords.startingC[1] + 1] != 3 &&
                     gameCells[aiWinningCoords.startingC[0] - 2][aiWinningCoords.startingC[1] + 1] != 0)
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
    else if (aiScanHorizontal() >= 2) lineState = 2;
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
            while (gameCells[j][i] == gameCells[j + 1][i] &&
                   gameCells[j][i] == 1 && !boardSize.isColFull[i]) {
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
            while (gameCells[j][i] == gameCells[j + 1][i] &&
                   gameCells[j][i] == 2 && !boardSize.isColFull[i]) {
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

int ConnectFour::aiScanHorizontal() {
    int lineCheck = 1; // Counts last played cell as one
    int longestLine = 0, tempBlockLine = 0;
    int endingTempBlockCoords[2];

    /* Block attempt */
    for (int i = 0; i < boardSize.height; ++i) {
        for (int j = 0; j < boardSize.width - 1; ++j) {
            while (gameCells[i][j] == gameCells[i][j + 1] &&
                   gameCells[i][j] == 1) {
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
            while (gameCells[i][j] == gameCells[i][j + 1] &&
                   gameCells[i][j] == 1) {
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
            while (gameCells[j][i] == gameCells[j + 1][i + 1] &&
                   gameCells[j][i] == 1) {
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
            while (gameCells[j][i] == gameCells[j + 1][i + 1] &&
                   gameCells[j][i] == 1) {
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

            while (gameCells[j][i] == gameCells[j + 1][i - 1] &&
                   gameCells[j][i] == 1) {
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

            while (gameCells[j][i] == gameCells[j + 1][i - 1] &&
                   gameCells[j][i] == 1) {
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

    maxNumOfMoves = 0; /* maxnumofmoves initialization for draw state */
    for (int i = 0; i < boardSize.height; ++i)
        for (int j = 0; j < boardSize.width; ++j)
            if(gameCells[i][j] == 3) maxNumOfMoves++;

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
        if (gameCells[winningCoords.startingC[0]][winningCoords.startingC[1]] == 4 ||
            gameCells[winningCoords.startingC[0]][winningCoords.startingC[1]] == 1)
            cout << "Player 1 Wins!!!" << endl;
        else if (aiEnable) cout << "Computer Wins!!!" << endl;
        else cout << "Player 2 Wins!!!" << endl;
    }
    return endGame;
}

void playFiveCF() {
    ConnectFour *game = new ConnectFour[5]; /* All arrays are dynamic */
    int *moveCount = new int[5];
    bool *gameDone = new bool[5];
    for (int i = 0; i < 5; ++i)
    {
    	moveCount[i] = 0;
    	gameDone[i] = false;
    }
    string boardName;
    for (int i = 0; i < 5; ++i) {
        cout << "Game " << i + 1 << ": " << "Enter board shape file: ";
        cin >> boardName;
        game[i] = ConnectFour(boardName);
    }

    int endGame = 0;
    while(endGame < 5){
        int i = multipleGameChoice();
        if (!gameDone[i]){
            cout << "Game " << i + 1 << ":" << endl;
            gameDone[i] = game[i].playTurn(moveCount[i]);
            if(gameDone[i]) {
                endGame++;
                cout << "Game " << i + 1 << " is ended." << endl;
            }
        }
        else cout << "Game " << i + 1 << " is ended. Please try another object!" << endl;
    }
    cout << "All games are ended!" << endl;
    delete [] game; /* All dynamic arrays have been deleted */
    delete [] moveCount; 
    delete [] gameDone;
}

int multipleGameChoice() {
    int gameChoice = -1;
    do {
        if (cin.fail()) {
            cin.clear(); // Clears any error flags if cin failed
            while (cin.get() != '\n'); // If user enters a char, cin.get() empty buffer
            cerr << "Please enter between 1 and 5" << endl;
        }
        cout << "Choose game: " << endl;
        cin >> gameChoice;
        gameChoice--;
    }
    while(gameChoice < 0 || gameChoice >= 5 || cin.fail());
    return gameChoice;
}

void upperCase(char &letter) {
    if (letter <= 'z' && letter >= 'a') letter += 'A' - 'a';
}