//
// Created by cecen on 11/23/2017.
//

#ifndef HW5_CONNECTFOURABSTRACT_H
#define HW5_CONNECTFOURABSTRACT_H

#include <string>
#include <fstream>
#include "Cell.h"

using namespace std;

namespace Abstract{
    enum commandType{
        SAVE,LOAD
    };
    struct Size {
        int width = 0, height = 0;
        bool * isColFull = nullptr;
    };

    struct coords {
        int startingC[2] = {0,0};
        int endingC[2] = {0,0};
    };

    void upperCase(char &letter);

    class ConnectFourAbstract {
    public:
        ConnectFourAbstract();
        ConnectFourAbstract(int, int, bool);
        ConnectFourAbstract(const ConnectFourAbstract & other);
        virtual ~ConnectFourAbstract ();

        ConnectFourAbstract& operator=(const ConnectFourAbstract & other);

        virtual void playGame (); //plays whole game

    protected:
        virtual void initGame(); // Initializes game
        void display() const; // Displays game
        void takeGameInfo (bool justAI); // Takes width height and comp selection
        virtual int getMove(); // Gets user move choice
        int play (int player, int column = -1); // plays one player move
        bool isItTheEnd(int col); // checks if its an end
        virtual void saveManagement (commandType command, const string &fileName, int &player); // Handles save management

        /* Pure Virtual Functions */
        virtual int checkFour(int col) = 0; /* Win check functions */
        virtual int getAiMove() = 0;        /* Ai functions */
        virtual int evaluateBoard() = 0;
        /* --- */

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
        int livingCellCount = 0;

    };

}

#endif //HW5_CONNECTFOURABSTRACT_H
