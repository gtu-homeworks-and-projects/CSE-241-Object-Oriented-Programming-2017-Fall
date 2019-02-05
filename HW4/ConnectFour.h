//
// Created by cecen on 1.11.2017.
//

#ifndef HW4_CONNECTFOUR_H
#define HW4_CONNECTFOUR_H

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
    bool * isColFull;
};

struct coords {
    int startingC[2] = {0,0};
    int endingC[2] = {0,0};
};

void playFiveCF();
int multipleGameChoice();

class ConnectFour {
public:
    ConnectFour();
    ConnectFour(const ConnectFour & other);
    ConnectFour(int, int, bool);
    explicit ConnectFour(string fileName);
    explicit ConnectFour(bool);
    ~ConnectFour ();

    ConnectFour& operator=(const ConnectFour & other);
    bool operator==(const ConnectFour &) const;
    bool operator!=(const ConnectFour &) const;
    static int livingCells();

    int playGame(); //plays whole game
    int play (int player, int column = -1); // plays one player move
    bool playTurn(int &); // Plays one round of game
    bool isItTheEnd(int col); // checks if its an end

private:
    void initGame(); // Initializes game
    void display() const; // Displays game
    void takeGameInfo (bool shapeBoard, bool justAI); // Takes width height and comp selection
    int getMove(); // Gets user move choice
    commandType saveManagement(commandType command, string fileName, int moveCount, int player); // Handles save management

    int checkFour(int col);             // Checks win situations
    int checkFourVertical(int col);     //
    int checkFourHorizontal (int col);   //
    int checkFourDiagonal1(int col);    //
    int checkFourDiagonal2(int col);    //

    int getAiMove();    // Gets legal ai move
    int evaluateBoard();
    int aiScanVertical();   /* These functions checks lines (until four   */
    int aiScanHorizontal(); /* unit) and returns their lengths. This helps*/
    int aiScanDiagonal1();  /* to determine a danger level. Also they     */
    int aiScanDiagonal2();  /* for possible win situations                */

    class Cell {
    public:
        Cell();
        Cell(int); // For auto type conversion
        Cell(int col, int row, int p);

        inline int getCellVal() const {return value;}

        /* Operator Overloads */
        Cell& operator=(int);
        bool operator==(const Cell &) const;
        bool operator!=(const Cell &) const;
        friend istream & operator>>(istream & input, Cell & );
        friend ostream & operator<<(ostream & output, Cell & );
        Cell operator++();
        Cell operator++(int);
        Cell operator--();
        Cell operator--(int);
        /* */

    private:
        void setCell(int col, int row, int p);

        int colNum, rowNum, player;
        int value;
    };

    Size boardSize;
    coords winningCoords;
    decltype(winningCoords) aiWinningCoords;
    Cell ** gameCells;

    int maxNumOfMoves;
    bool isItWin; /* Draw or Win? */
    bool endGame; /* Main switch for game loop */
    bool aiEnable; /* Computer is active or not */
    bool gameInitialized = false; // Game board initialize switch
    string saveFileName;
    string boardFile;
    static int livingCellCount;

    friend istream & operator>>(istream & input, ConnectFour::Cell & );  /* Also there's need to make these overloads friend */
    friend ostream & operator<<(ostream & output, ConnectFour::Cell & ); /* cause of Cell class being private inside ConnectFour */
};


#endif //HW4_CONNECTFOUR_H
