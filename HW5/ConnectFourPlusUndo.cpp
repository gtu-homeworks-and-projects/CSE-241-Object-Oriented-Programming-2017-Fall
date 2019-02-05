//
// Created by cecen on 11/25/2017.
//

#include "ConnectFourPlusUndo.h"
namespace Abstract{
    ConnectFourPlusUndo::ConnectFourPlusUndo () {
        pastMoves = new int[maxNumOfMoves];
        undoInit = true;
    }

    ConnectFourPlusUndo::ConnectFourPlusUndo (const ConnectFourPlusUndo &other) {
        *this = other;
    }

    ConnectFourPlusUndo::~ConnectFourPlusUndo () {
        delete pastMoves;
    }

    ConnectFourPlusUndo & ConnectFourPlusUndo::operator= (const ConnectFourPlusUndo &other) {
        if(this != &other){
            ConnectFourAbstract::operator=(other);
            if(undoInit) delete [] pastMoves;
            pastMoves = new int[other.maxNumOfMoves];
            undoInit = other.undoInit;
        }
        return *this;
    }

    void ConnectFourPlusUndo::playGame () {
        int playerMoveCol = 0;
        int currentPlayer = 1;
        isItWin = false;

        maxNumOfMoves = 0; /* max num of moves initialization for draw state */
        for (int i = 0; i < boardSize.height; ++i)
            for (int j = 0; j < boardSize.width; ++j)
                if(gameCells[i][j] == 3) maxNumOfMoves++;

        while (!endGame) {

            cout << (1 == currentPlayer ? "-Player 1's turn-" : (15 == currentPlayer) ? "-Player 2's turn-" : "Computers move:")
                 << endl;


            do {
                display(); /* Display the board when player needs */
                playerMoveCol = aiEnable && currentPlayer == 10 ? play(currentPlayer) : getMove(); //

                if (playerMoveCol == 35)
                    saveManagement(LOAD, saveFileName, currentPlayer);
                else if (playerMoveCol == 45)
                    saveManagement(SAVE, saveFileName, currentPlayer);
                else if (playerMoveCol == 55)
                    if(livingCellCount == 0) cerr << "Can't UNDO more!" << endl;
                    else undoMove(currentPlayer, pastMoves[livingCellCount]);
            } while (35 == playerMoveCol || 45 == playerMoveCol || 55 == playerMoveCol);

            if(currentPlayer == 1)  play(currentPlayer,playerMoveCol);

            currentPlayer =
                    1 == currentPlayer ? aiEnable ? 10 : 15 : 1; /* AI is 10, Second Player is 15 and First Player is 1 */
            livingCellCount++;
            pastMoves[livingCellCount] = playerMoveCol;

            if (maxNumOfMoves == livingCellCount) endGame = true; /* DRAW Check */

            isItTheEnd(playerMoveCol); /* If its a win, ends loop and makes sure it doesnt registers it as DRAW */
        }
        display();
        if (maxNumOfMoves == livingCellCount && !isItWin) cout << "DRAW" << endl;
        else {
            if (gameCells[winningCoords.startingC[0]][winningCoords.startingC[1]] == 4 ||
                gameCells[winningCoords.startingC[0]][winningCoords.startingC[1]] == 1)
                cout << "Player 1 Wins!!!" << endl;
            else if (aiEnable) cout << "Computer Wins!!!" << endl;
            else cout << "Player 2 Wins!!!" << endl;
        }
    }

    void ConnectFourPlusUndo::initGame () {
        ConnectFourAbstract::initGame();
        if(undoInit) delete [] pastMoves;

        pastMoves = new int[maxNumOfMoves];
        for (int i = 0; i < maxNumOfMoves; ++i) pastMoves[i] = 0;

        undoInit = true;
    }

    int ConnectFourPlusUndo::getMove () {
        auto isInputValid = false;
        char move = '0';
        auto lastCol = 'A';
        string command;
        string line;

        for (int i = 1; i < boardSize.width; ++i) ++lastCol; /* For displaying Last column */
        while (!isInputValid) {
            cout << "Select the column you want to play in (A to " << lastCol
                 << ") or LOAD/SAVE/UNDO: ";
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
                } else if (command == "UNDO") {                         /* Added in this derived class */
                    return 55;
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

    void ConnectFourPlusUndo::saveManagement (Abstract::commandType command, const string &fileName, int &player) {
        if(command == LOAD){
            ifstream saveFile(saveFileName); /* Opens file as input stream */
            int isColFull;
            if (!saveFile.fail()) {
                saveFile >> boardSize.height
                         >> boardSize.width;
                initGame();
                for (int i = 0; i < boardSize.width; ++i) {
                    saveFile >> isColFull;
                    boardSize.isColFull[i] = isColFull != 0;
                    for (int j = 0; j < boardSize.height; ++j) saveFile >> gameCells[j][i]; // Stream extraction overload for Cell class
                }
                saveFile >> aiEnable
                         >> livingCellCount
                         >> player;
                undoInit;
                for (int k = 0; k < maxNumOfMoves; ++k) saveFile >> pastMoves[k]; /* Added in this derived class */

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
                         << livingCellCount << endl
                         << player << endl;
                for (int k = 0; k < maxNumOfMoves; ++k)
                    saveFile << pastMoves[k] << endl;   /* Added in this derived class */

                saveFile.close();
            } else cerr << "File couldn't be opened." << endl;
        }
    }

    void ConnectFourPlusUndo::undoMove (int &player, int column) {
        if (column == -1) column = getAiMove();

        int i,k;
        for (k = 0; k < boardSize.height && gameCells[k][column] == 0; ++k);
        for (i = k; i < boardSize.height && gameCells[i][column] == 3; ++i); /* Gets last filled row */

        gameCells[i][column] = 3;   /* Empties last filled cell */
        boardSize.isColFull[column] = false; /* Makes sure that column is marked as not full */
        livingCellCount--; /* Living Cell Count Also works as index of Derived Class' pastMoves array */
        player = player == 1 ? (aiEnable? 10 : 15) : 1;  /* Player switch */
    }
}