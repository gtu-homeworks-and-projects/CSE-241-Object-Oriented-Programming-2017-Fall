//
// Created by cecen on 12.10.2017.
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

enum commandType{
    SAVE,LOAD
};
struct Size {
    int width = 0, height = 0;
    vector<bool> isColFull;
};

struct coords {
    int startingC[2] = {0,0};
    int endingC[2] = {0,0};
};

void playFiveCF();

class ConnectFour {
public:
    ConnectFour();
    ConnectFour(int, int, bool);
    ConnectFour(const Size &, bool);
    explicit ConnectFour(bool);

    static int livingCells();
    Size getSize() const {return boardSize;}

    int playGame(); //plays whole game
    void play(int player, int column = -1); // plays one player move
    bool playTurn(int &); // Plays one round of game
    bool isItTheEnd(int col); // checks if its an end
    bool compareGames(const ConnectFour & other) const; // Compares which game has least empty cells
    bool isGameInit() const {return gameInitialized;}


private:
    void initGame(); // Initializes game
    void display() const; // Displays game
    void takeGameInfo(); // Takes width height and comp selection
    int getMove(); // Gets user move choice
    commandType saveManagement(commandType command, string fileName, int moveCount, int player); // Handles save management

    int checkFour(int col);             // Checks win situations
    int checkFourVertical(int col);     //
    int checkFourHorizantal(int col);   //
    int checkFourDiagonal1(int col);    //
    int checkFourDiagonal2(int col);    //

    int getAiMove();    // Gets legal ai move
    int evaluateBoard();
    int aiScanVertical();   /* These functions checks lines (until four   */
    int aiScanHorizantal(); /* unit) and returns their lengths. This helps*/
    int aiScanDiagonal1();  /* to determine a danger level. Also they     */
    int aiScanDiagonal2();  /* for possible win situations                */

    class Cell {
    public:
        Cell();
        Cell(int col, int row, int p);

        inline void setCellVal(int val);
        int getCellVal() const {return value;}
    private:
        void setCell(int col, int row, int p);

        int colNum, rowNum, player;
        int value;
    };

    Size boardSize;
    coords winningCoords;
    decltype(winningCoords) aiWinningCoords;
    vector<vector <Cell>> gameCells;

    int maxNumOfMoves;
    bool isItWin; /* Draw or Win? */
    bool endGame; /* Main switch for game loop */
    bool aiEnable; /* Computer is active or not */
    bool gameInitialized; // Game board initialize switch
    string saveFileName;
    static int livingCellCount;
};
