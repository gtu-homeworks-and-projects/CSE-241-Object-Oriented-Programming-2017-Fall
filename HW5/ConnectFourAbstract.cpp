//
// Created by cecen on 11/23/2017.
//

#include "ConnectFourAbstract.h"
namespace Abstract{
    /* ConnectFourAbstract class Constructors, Copy Constructor, overloads and destructor (Big three implementation included) */
    ConnectFourAbstract::ConnectFourAbstract(){ // Default 5x5 game initialization
        takeGameInfo(false);
        initGame();
    }

    ConnectFourAbstract::ConnectFourAbstract(int height, int width, bool isCompPlaying) { // Classic 3 parameter constructor
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

    ConnectFourAbstract::ConnectFourAbstract (const ConnectFourAbstract &other) {
        *this = other;
    }

    ConnectFourAbstract::~ConnectFourAbstract () {
        for (int i = 0; i < boardSize.height; ++i) delete []gameCells[i];
        delete []gameCells;

        delete []boardSize.isColFull;
    }

    ConnectFourAbstract &ConnectFourAbstract::operator= (const ConnectFourAbstract &other) {
        cout << "Abstract assignment" << endl;
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

    void ConnectFourAbstract::saveManagement (commandType command, const string &saveFileName, int &player) {
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
                cout << saveFile.tellg() << endl;
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
                cout << saveFile.tellp() << endl;
                saveFile.close();
            } else cerr << "File couldn't be opened." << endl;
        }
    }

    void ConnectFourAbstract::display() const{
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
        cout << "Living cells: " << livingCellCount << endl;
    }

    void ConnectFourAbstract::initGame() {
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

    void ConnectFourAbstract::takeGameInfo (bool justAI) {
        if(!justAI) {
            /* Takes height and width instead of board */
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

    bool ConnectFourAbstract::isItTheEnd(int col) {
        if (checkFour(col)) {

            isItWin = true;
            endGame = true;
            return true;
        }
        return false;
    }

    void ConnectFourAbstract::playGame () {
        int playerMoveCol = 0;
        int currentPlayer = 1;
        isItWin = false;
        if(!gameInitialized){
            cout << "Enter board file name: ";
            cin >> boardFile;
            if(!cin.fail()) takeGameInfo(false); /* If file name successfully read initializes board according to it */
            else{
                cin.clear(); // Clears bad state flag
                takeGameInfo(false);
                initGame();
            }
        }

        maxNumOfMoves = 0; /* max num of moves initialization for draw state */
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
                        saveManagement(LOAD, saveFileName, currentPlayer);

                    else if (playerMoveCol == 45)
                        saveManagement(SAVE, saveFileName, currentPlayer);
                } while (35 == playerMoveCol || 45 == playerMoveCol);

                play(currentPlayer,playerMoveCol);
            }
            currentPlayer =
                    1 == currentPlayer ? aiEnable ? 10 : 15 : 1; /* AI is 10, Second Player is 15 and First Player is 1 */
            livingCellCount++;
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

    int ConnectFourAbstract::getMove() {
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

    int ConnectFourAbstract::play (int player, int column) {
        if (column == -1) column = getAiMove();

        int i,k;
        for (k = 0; k < boardSize.height && gameCells[k][column] == 0; ++k);
        for (i = k; i < boardSize.height && gameCells[i][column] == 3; ++i); /* Gets last filled row */

        gameCells[i - 1][column] = (1 == player) ? 1 : 2;
        if (i == k + 1) boardSize.isColFull[column] = true;

        return column; // Will  be used for ai move win check

    }

    void upperCase(char &letter) {
        if (letter <= 'z' && letter >= 'a') letter += 'A' - 'a';
    }
}