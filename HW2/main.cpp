#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>

using namespace std;

const int MAX_TABLE_SIZE = 20;

// STRUCTS START HERE -------------------------------------------------------------------------------------------------
struct board {
    int table[MAX_TABLE_SIZE][MAX_TABLE_SIZE];
    int size;
    int isColFull[MAX_TABLE_SIZE];
};
/* Variable type for game board:
 * table: is 2d board array
 * size: Keeps size of the board array
 * isColFull: column is full or not
 */

struct coords {
    int startingC[2];
    int endingC[2];
};
/* Variable type for winning four coordinates */
// STRUCTS END HERE ---------------------------------------------------------------------------------------------------


// COMPUTER MOVE RELATED FUNCTIONS -START- ----------------------------------------------------------------------------
int getAiMove(const struct board &place, struct coords &aiC);

/* According to board situation returns a move for Computer play as an integer */

int evaluateBoard(const struct board &place, struct coords &aiC);

/* Scans the board and returns the state of it as an integer (State numbers explained in function body) */

int aiScanVertical(const struct board &place, struct coords &aiC);   /* These functions checks lines (until four   */
int aiScanHorizantal(const struct board &place, struct coords &aiC); /* unit) and returns their lengths. This helps*/
int aiScanDiagonal1(const struct board &place, struct coords &aiC);  /* to determine a danger level. Also they     */
int aiScanDiagonal2(const struct board &place, struct coords &aiC);  /* for possible win situations                */
// COMPUTER MOVE RELATED FUNCTIONS -END- ------------------------------------------------------------------------------

int takeBoardSize(); /* Asks user which board size they want to be playing according to rules */
int getMove(const struct board &place, string &filename); /* Gets a legal user move or SAVE/LOAD command each turn  */
void registerMove(struct board &place, int column, int player = 1);

/* Registers moves to the board according to given column. WARNING!: This function doesnt check if given column is
 * legal or not. It depends on other functions which gets moves to check their validity. Also assumes player is 1
 * if no provided */

int checkFour(struct board &place, const int &lastPlayedCol, struct coords &winCoords);

/* If there is a line longer than 4 units, it marks those 4 units and returns a positive integer */


int checkFourVertical(const struct board &place, const int &lastPlayedCol, struct coords &winCoords);

int checkFourHorizantal(const struct board &place, const int &lastPlayedCol, struct coords &winCoords);

int checkFourDiagonal1(const struct board &place, const int &lastPlayedCol, struct coords &winCoords);

int checkFourDiagonal2(const struct board &place, const int &lastPlayedCol, struct coords &winCoords);

/* These 4 functions looks around last played cell and returns
 * maximum line lengths with their starting and ending coords */

bool isComputerPlaying(); /* Sets game mode as PvP or PvC */
void initBoard(struct board &place); /* Initializes the game board */
void displayBoard(const struct board &place); /* Displays game boards current state */
void upperCase(char &letter); /* to accept lower case column choices a small implementation of toUpper function */


void loadState(const string &fileName, board &place, bool &ai, int &maxNumOfMoves, int &numOfMoves, int &whichPlayer);

void saveState(const string &fileName, const board &place, bool ai, int maxNumOfMoves, int numOfMoves, int whichPlayer);

/* Loads and Saves given parameters */

int main() {
    board gameBoard{}; /* Main game board */
    coords winningCoords{}; /* Coordinates of winning line */
    decltype(winningCoords) aiCalcCoords{}; /* Coordinates of ai move lines */

    string file;

    auto isItWin = false; /* Draw or Win? */
    auto endGame = false; /* Main switch for game loop */
    bool aiEnable; /* Computer is active or not */

    gameBoard.size = takeBoardSize();
    initBoard(gameBoard);

    aiEnable = isComputerPlaying();

    int maxNumOfMoves = gameBoard.size * gameBoard.size; /* Maximum possible moves */
    int numOfMoves = 0; /* Keeps track of how many moves have been made */
    int whichPlayer = 1; /* Which player is playing this turn? */
    int playerMoveCol; /* Which column to play on? */

    while (!endGame) {

        cout << (1 == whichPlayer ? "-Player 1's turn-" : (15 == whichPlayer) ? "-Player 2's turn-" : "Computers move:")
             << endl;


        if (aiEnable && whichPlayer == 10)
            registerMove(gameBoard, playerMoveCol = getAiMove(gameBoard, aiCalcCoords),
                         whichPlayer); /* Computer Move */
        else {
            do {
                displayBoard(gameBoard); /* Display the board when player needs */
                playerMoveCol = getMove(gameBoard, file);

                if (playerMoveCol == 35)
                    loadState(file, gameBoard, aiEnable, maxNumOfMoves, numOfMoves, whichPlayer);

                else if (playerMoveCol == 45)
                    saveState(file, gameBoard, aiEnable, maxNumOfMoves, numOfMoves, whichPlayer);

            } while (35 == playerMoveCol || 45 == playerMoveCol);

            if (whichPlayer == 1)
                registerMove(gameBoard, playerMoveCol); /* Player Move */
            else registerMove(gameBoard, playerMoveCol, whichPlayer); /* Player Move */

        }
        if (checkFour(gameBoard, playerMoveCol, winningCoords)) {
            isItWin = true;
            endGame = true;
        } /* If its a win, ends loop and makes sure it doesnt registers it as DRAW */

        whichPlayer =
                1 == whichPlayer ? aiEnable ? 10 : 15 : 1; /* AI is 10, Second Player is 15 and First Player is 1 */
        numOfMoves++;
        if (maxNumOfMoves == numOfMoves) endGame = true; /* DRAW Check */
    }

    displayBoard(gameBoard);
    if (maxNumOfMoves == numOfMoves && !isItWin) cout << "DRAW" << endl;
    else {
        if (gameBoard.table[winningCoords.startingC[0]][winningCoords.startingC[1]] == 4 ||
            gameBoard.table[winningCoords.startingC[0]][winningCoords.startingC[1]] == 1)
            cout << "Player 1 Wins!!!" << endl;
        else if (aiEnable) cout << "Computer Wins!!!" << endl;
        else cout << "Player 2 Wins!!!" << endl;
    }
    return 0;
}

void displayBoard(const struct board &place) {
    cout << endl;
    auto Letter = 'A';
    for (int k = 0; k < place.size; ++k, ++Letter) cout << Letter << ' ';
    cout << endl;

    for (int i = 0; i < place.size; ++i) {
        for (int j = 0; j < place.size; ++j)
            switch (place.table[i][j]) {
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
}

void initBoard(struct board &place) {
    for (int i = 0; i < MAX_TABLE_SIZE; ++i)
        for (int j = 0; j < MAX_TABLE_SIZE; ++j) {
            place.table[i][j] = 0;
            place.isColFull[j] = 0;
        } /* Places 0 in all cells */


    for (int k = 0; k < place.size; ++k)
        for (int l = 0; l < place.size; ++l)
            place.table[k][l] = 3;
    /* Places 3 in empty valid cells in order to not conflict with non table cells */

}

bool isComputerPlaying() {
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

    if (1 == gameModeChoice) cout << "Player 1 is 'X', Player 2 is 'O'" << endl;
    else cout << "Player 1 is 'X', Computer is 'O'" << endl;

    return ('C' == gameModeChoice || 'c' == gameModeChoice);
}

void registerMove(struct board &place, int column, int player) {
    int i;

    for (i = 0; 3 == place.table[i][column] && i < place.size; ++i); /* Gets last filled row */

    place.table[i - 1][column] = (1 == player) ? 1 : 2;
    if (i == 1) place.isColFull[column] = 1;
}

int takeBoardSize() {
    int size = 0;
    cout << "Welcome to the game of \"Connect Four\" " << endl;
    while (size < 4 || size > MAX_TABLE_SIZE || size % 2 != 0) {
        cout << "Enter length of one side of the board (Between 4 - 20 and it must be an even number): " << endl;

        cin >> size;
        if (cin.fail()) {
            cin.clear(); // Clears any error flags if cin failed
            while (cin.get() != '\n'); // If user enters a char, cin.get() empty buffer
        }
        if (size < 4 || size > MAX_TABLE_SIZE || size % 2 != 0) cerr << "Please enter a valid number!" << endl;
    } /* To not deal with cin buffer, takes char and converts into integer */
    cout << "Board size will be " << size << "x" << size << endl;
    return size;
}

int getMove(const struct board &place, string &filename) {
    auto isInputValid = false;
    char move = 0;
    auto lastCol = 'A';
    string command;
    string line;

    for (int i = 1; i < place.size; ++i) ++lastCol; /* For displaying Last column */

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
                filename = line.substr(5, line.size() + 1);
                return 35;
            } else if (command == "SAVE") {
                filename = line.substr(5, line.size() + 1);
                return 45;
            }

        }
        isInputValid = (move >= 'A' && move <= lastCol);
        if (!isInputValid) cerr << "Please select a valid column! (A to " << lastCol << ")" << endl;
        else {

            if (place.isColFull[move - 'A']) {
                isInputValid = false;
                cerr << "The column you choose is full. Please try another column." << endl;
            }
        }
    }
    return int(move - 'A');
}

void upperCase(char &letter) {
    if (letter <= 'z' && letter >= 'a') letter += 'A' - 'a';
}

// Vertical 4 dots Check
int checkFourVertical(const struct board &place, const int &lastPlayedCol, struct coords &winCoords) {
    int i;
    int j;
    int k;

    for (i = 0; 3 == place.table[i][lastPlayedCol] && i < place.size; ++i);
    int lineCheck = 1; // Counts last played cell as one

    /* Scans to the up */
    for (k = lastPlayedCol, j = i; j < place.size && place.table[j][k] == place.table[j + 1][k]; j++, lineCheck++);
    winCoords.startingC[0] = j;
    winCoords.startingC[1] = k;

    /* Scans to the down */
    for (j = i; j > 0 && place.table[j][k] == place.table[j - 1][k]; j--, lineCheck++);
    winCoords.endingC[0] = j;
    winCoords.endingC[1] = k;

    return lineCheck;
}

// Horizantal 4 dots Check
int checkFourHorizantal(const struct board &place, const int &lastPlayedCol, struct coords &winCoords) {
    int i;
    int j;
    int k;

    for (i = 0; 3 == place.table[i][lastPlayedCol] && i < place.size; ++i);
    int lineCheck = 1; // Counts last played cell as one

    /* Scans to the right */
    for (j = i, k = lastPlayedCol;
         j < place.size && k < place.size && place.table[j][k] == place.table[j][k + 1]; k++, lineCheck++);
    winCoords.startingC[0] = j;
    winCoords.startingC[1] = k;

    /* Scans to the left */
    for (j = i, k = lastPlayedCol; k > 0 && place.table[j][k] == place.table[j][k - 1]; k--, lineCheck++);
    winCoords.endingC[0] = j;
    winCoords.endingC[1] = k;

    return lineCheck;

}

// Diagonal '\' 4 dots Check
int checkFourDiagonal1(const struct board &place, const int &lastPlayedCol, struct coords &winCoords) {
    int i;
    int j;
    int k;

    for (i = 0; 3 == place.table[i][lastPlayedCol] && i < place.size; ++i);
    int lineCheck = 1; // Counts last played cell as one

    /* Scans to the down right */
    for (j = i, k = lastPlayedCol;
         j < place.size && k < place.size && place.table[j][k] == place.table[j + 1][k + 1]; k++, j++, lineCheck++);
    winCoords.startingC[0] = j;
    winCoords.startingC[1] = k;

    /* Scans to the up left */
    for (j = i, k = lastPlayedCol;
         k > 0 && j > 0 && place.table[j][k] == place.table[j - 1][k - 1]; k--, j--, lineCheck++);
    winCoords.endingC[0] = j;
    winCoords.endingC[1] = k;

    return lineCheck;
}

// Diagonal '/' 4 dots Check
int checkFourDiagonal2(const struct board &place, const int &lastPlayedCol, struct coords &winCoords) {
    int i;
    int j;
    int k;

    for (i = 0; 3 == place.table[i][lastPlayedCol] && i < place.size; ++i);
    int lineCheck = 1; // Counts last played cell as one

    /* Scans to the upper right */
    for (j = i, k = lastPlayedCol;
         j > 0 && k < place.size && place.table[j][k] == place.table[j - 1][k + 1]; k++, j--, lineCheck++);
    winCoords.startingC[0] = j;
    winCoords.startingC[1] = k;

    /* Scans to the down left */
    for (j = i, k = lastPlayedCol;
         k > 0 && j < place.size && place.table[j][k] == place.table[j + 1][k - 1]; k--, j++, lineCheck++);
    winCoords.endingC[0] = j;
    winCoords.endingC[1] = k;

    return lineCheck;
}

// Checks any 4 unit length dots check four win conditions
int checkFour(struct board &place, const int &lastPlayedCol, struct coords &winCoords) {
    if (checkFourVertical(place, lastPlayedCol, winCoords) >= 4) {
        for (int i = winCoords.endingC[0], count = 0; i <= winCoords.startingC[0] && count < 4; ++i, ++count) {
            place.table[i][lastPlayedCol] = place.table[i][lastPlayedCol] == 1 ? 4 : 5;
        }
        return 1;
    }

    if (checkFourHorizantal(place, lastPlayedCol, winCoords) >= 4) {
        for (int i = winCoords.endingC[1], count = 0; i <= winCoords.startingC[1] && count < 4; ++i, ++count) {
            place.table[winCoords.startingC[0]][i] = place.table[winCoords.startingC[0]][i] == 1 ? 4 : 5;
        }
        return 2;
    }

    if (checkFourDiagonal1(place, lastPlayedCol, winCoords) >= 4) {
        for (int i = winCoords.startingC[0], count = 0, j = winCoords.startingC[1];
             i >= winCoords.endingC[0] && j >= winCoords.endingC[1] && count < 4;
             --i, --j, ++count) {
            place.table[i][j] = place.table[i][j] == 1 ? 4 : 5;
        }
        return 3;
    }

    if (checkFourDiagonal2(place, lastPlayedCol, winCoords) >= 4) {
        for (int i = winCoords.startingC[0], j = winCoords.startingC[1], count = 0;
             i <= winCoords.endingC[0] && j >= winCoords.endingC[1] && count < 4;
             ++i, --j, ++count) {
            place.table[i][j] = place.table[i][j] == 1 ? 4 : 5;
        }
        return 4;
    }


    return 0;
}

int getAiMove(const struct board &place, struct coords &aiC) {
    int column;
    int state = 0;

    while (place.isColFull[column = rand() % place.size]);
    /* Random computer move in case of states don't evaluate a value */

    state = evaluateBoard(place, aiC);

    switch (state) {
        case 1: /* Player or computer is winning vertically */
            column = aiC.endingC[1];
            break;
        case 2: /* Player or computer is winning horizontally */
            if (place.table[aiC.endingC[0]][aiC.endingC[1] + 1] == 3 &&
                place.table[aiC.endingC[0] + 1][aiC.endingC[1] + 1] != 3 &&
                aiC.endingC[1] + 1 < place.size && !place.isColFull[aiC.endingC[1] + 1])
                column = aiC.endingC[1] + 1;
            else if (place.table[aiC.startingC[0]][aiC.startingC[1] - 1] == 3 &&
                     place.table[aiC.startingC[0] + 1][aiC.startingC[1] - 1] != 3 &&
                     aiC.startingC[1] > 0 && !place.isColFull[aiC.startingC[1] - 1])
                column = aiC.startingC[1] - 1;

            break;
        case 3: /* Player or computer is winning diagonal down left to the upper right */
            if (place.table[aiC.endingC[0] + 1][aiC.endingC[1] + 1] == 3 &&
                place.table[aiC.endingC[0] + 2][aiC.endingC[1] + 1] != 3 &&
                aiC.endingC[1] + 1 < place.size && !place.isColFull[aiC.endingC[1] + 1])
                column = aiC.endingC[1] + 1;
            else if (place.table[aiC.startingC[0] - 1][aiC.startingC[1] - 1] == 3 &&
                     place.table[aiC.startingC[0] - 2][aiC.startingC[1] - 1] != 3 &&
                     aiC.startingC[1] > 0 && !place.isColFull[aiC.startingC[1] - 1])
                column = aiC.startingC[1] - 1;
            break;
        case 4: /* Player or computer is winning diagonal up left to the down right */
            if (place.table[aiC.endingC[0] + 1][aiC.endingC[1] - 1] == 3 &&
                place.table[aiC.endingC[0] + 2][aiC.endingC[1] - 1] != 3 &&
                aiC.endingC[1] > 0 && !place.isColFull[aiC.endingC[1] + 1])
                column = aiC.endingC[1] + 1;
            else if (place.table[aiC.startingC[0] - 1][aiC.startingC[1] + 1] == 3 &&
                     place.table[aiC.startingC[0] - 2][aiC.startingC[1] + 1] != 3 &&
                     aiC.startingC[1] + 1 < place.size && !place.isColFull[aiC.startingC[1] - 1])
                column = aiC.startingC[1] - 1;
            break;
        default:
            break;
    }

    return column;
}

int evaluateBoard(const struct board &place, struct coords &aiC) {
    int lineState = 0;
    if (aiScanVertical(place, aiC) >= 2) lineState = 1;
    else if (aiScanHorizantal(place, aiC) >= 2) lineState = 2;
    else if (aiScanDiagonal1(place, aiC) >= 2) lineState = 3;
    else if (aiScanDiagonal2(place, aiC) >= 2) lineState = 4;
    return lineState;
}

int aiScanVertical(const struct board &place, struct coords &aiC) {

    int lineCheck = 1; // Counts last played cell as one
    int longestLine = 0, tempBlockLine = 0;
    int endingTempBlockCoords[2];

    /* Block attempt */
    for (int i = 0; i < place.size; ++i) {
        for (int j = 0; j < place.size; ++j) {
            while (place.table[j][i] == place.table[j + 1][i] && place.table[j][i] == 1 && !place.isColFull[i]) {
                ++lineCheck;
                if (lineCheck >= tempBlockLine) {
                    tempBlockLine = lineCheck;
                    endingTempBlockCoords[0] = j + 1;
                    endingTempBlockCoords[1] = i;
                }
                ++j;
            }
            lineCheck = 1;
        }
    }

    /* Win attempt */
    for (int i = 0; i < place.size; ++i) {
        for (int j = 0; j < place.size; ++j) {
            while (place.table[j][i] == place.table[j + 1][i] && place.table[j][i] == 2 && !place.isColFull[i]) {
                ++lineCheck;
                if (lineCheck >= longestLine) {
                    longestLine = lineCheck;
                    aiC.endingC[0] = j + 1;
                    aiC.endingC[1] = i;
                }
                ++j;
            }
            lineCheck = 1;
        }
    }

    if (tempBlockLine > 2) {
        aiC.endingC[0] = endingTempBlockCoords[0];
        aiC.endingC[1] = endingTempBlockCoords[1];
        longestLine = tempBlockLine;
    }

    aiC.startingC[0] = aiC.endingC[0] - (lineCheck - 1);
    aiC.startingC[1] = aiC.endingC[1];

    return longestLine;
}

int aiScanHorizantal(const struct board &place, struct coords &aiC) {
    int lineCheck = 1; // Counts last played cell as one
    int longestLine = 0, tempBlockLine = 0;
    int endingTempBlockCoords[2];

    /* Block attempt */
    for (int i = 0; i < place.size; ++i) {
        for (int j = 0; j < place.size; ++j) {
            while (place.table[i][j] == place.table[i][j + 1] && place.table[i][j] == 1) {
                ++lineCheck;
                if (lineCheck >= tempBlockLine) {
                    tempBlockLine = lineCheck;
                    endingTempBlockCoords[0] = i;
                    endingTempBlockCoords[1] = j + 1;
                }
                ++j;
            }
        }
    }

    /* Win attempt */
    for (int i = 0; i < place.size; ++i) {
        for (int j = 0; j < place.size; ++j) {
            while (place.table[i][j] == place.table[i][j + 1] && place.table[i][j] == 1) {
                ++lineCheck;
                if (lineCheck >= longestLine) {
                    longestLine = lineCheck;
                    aiC.endingC[0] = i;
                    aiC.endingC[1] = j + 1;
                }
                ++j;
            }
        }
    }

    if (tempBlockLine > 2) {
        aiC.endingC[0] = endingTempBlockCoords[0];
        aiC.endingC[1] = endingTempBlockCoords[1];
        longestLine = tempBlockLine;
    }

    aiC.startingC[0] = aiC.endingC[0];
    aiC.startingC[1] = aiC.endingC[1] - (lineCheck - 1);

    return longestLine;
}

int aiScanDiagonal1(const struct board &place, struct coords &aiC) {
    int lineCheck = 1; // Counts last played cell as one
    int longestLine = 0, tempBlockLine = 0;
    int endingTempBlockCoords[2];

    /* Block attempt */
    for (int i = 0; i < place.size; ++i) {
        for (int j = 0; j < place.size; ++j) {
            while (place.table[j][i] == place.table[j + 1][i + 1] && place.table[j][i] == 1) {
                ++lineCheck;
                if (lineCheck >= tempBlockLine) {
                    tempBlockLine = lineCheck;
                    endingTempBlockCoords[0] = j + 1;
                    endingTempBlockCoords[1] = i + 1;
                }
                ++i;
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
    for (int i = 0; i < place.size; ++i) {
        for (int j = 0; j < place.size; ++j) {
            while (place.table[j][i] == place.table[j + 1][i + 1] && place.table[j][i] == 1) {
                ++lineCheck;
                if (lineCheck >= longestLine) {
                    longestLine = lineCheck;
                    aiC.endingC[0] = j + 1;
                    aiC.endingC[1] = i + 1;
                }
                ++i;
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
        aiC.endingC[0] = endingTempBlockCoords[0];
        aiC.endingC[1] = endingTempBlockCoords[1];
        longestLine = tempBlockLine;
    }

    aiC.startingC[0] = aiC.endingC[0] - (lineCheck - 1);
    aiC.startingC[1] = aiC.endingC[1] - (lineCheck - 1);

    return longestLine;
}

int aiScanDiagonal2(const struct board &place, struct coords &aiC) {
    int lineCheck = 1; // Counts last played cell as one
    int longestLine = 0, tempBlockLine = 0;
    int endingTempBlockCoords[2];

    /* Block attempt */
    for (int i = 0; i < place.size; ++i) {
        for (int j = 0; j < place.size; ++j) {

            while (place.table[j][i] == place.table[j + 1][i - 1] && place.table[j][i] == 1) {
                ++lineCheck;
                if (lineCheck >= tempBlockLine) {
                    tempBlockLine = lineCheck;
                    endingTempBlockCoords[0] = j + 1;
                    endingTempBlockCoords[1] = i - 1;
                }
                --i;
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
    for (int i = 0; i < place.size; ++i) {
        for (int j = 0; j < place.size; ++j) {

            while (place.table[j][i] == place.table[j + 1][i - 1] && place.table[j][i] == 1) {
                ++lineCheck;
                if (lineCheck >= longestLine) {
                    longestLine = lineCheck;
                    aiC.endingC[0] = j + 1;
                    aiC.endingC[1] = i - 1;
                }
                --i;
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
        aiC.endingC[0] = endingTempBlockCoords[0];
        aiC.endingC[1] = endingTempBlockCoords[1];
        longestLine = tempBlockLine;
    }

    aiC.startingC[0] = aiC.endingC[0] - (lineCheck - 1);
    aiC.startingC[1] = aiC.endingC[1] + (lineCheck - 1);
    return longestLine;
}

void
saveState(const string &fileName, const board &place, bool ai, int maxNumOfMoves, int numOfMoves, int whichPlayer) {
    ofstream saveFile(fileName); /* Opens file as output stream */

    if (!saveFile.fail()) {
        for (int i = 0; i < MAX_TABLE_SIZE; ++i) {
            saveFile << place.isColFull[i] << endl;
            for (int j = 0; j < MAX_TABLE_SIZE; ++j)
                saveFile << place.table[i][j] << endl;
        }
        saveFile << place.size << endl
                 << ai << endl
                 << maxNumOfMoves << endl
                 << numOfMoves << endl
                 << whichPlayer << endl;
    } else cerr << "File couldn't be opened." << endl;
}

void loadState(const string &fileName, board &place, bool &ai, int &maxNumOfMoves, int &numOfMoves, int &whichPlayer) {
    ifstream saveFile(fileName); /* Opens file as input stream */

    if (!saveFile.fail()) {
        for (int i = 0; i < MAX_TABLE_SIZE; ++i) {
            saveFile >> place.isColFull[i];
            for (int j = 0; j < MAX_TABLE_SIZE; ++j)
                saveFile >> place.table[i][j];
        }
        saveFile >> place.size
                 >> ai
                 >> maxNumOfMoves
                 >> numOfMoves
                 >> whichPlayer;
    } else cerr << "File couldn't be opened." << endl;
}
