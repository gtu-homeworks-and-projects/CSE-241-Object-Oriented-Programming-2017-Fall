//
// Created by cecen on 11/25/2017.
//

#ifndef HW5_CONNECTFOURPLUSUNDO_H
#define HW5_CONNECTFOURPLUSUNDO_H

#include "ConnectFourPlus.h"
namespace Abstract{
    class ConnectFourPlusUndo: public ConnectFourPlus { /* This derived class of ConnectFourPlus able to UNDO moves */
    public:
        ConnectFourPlusUndo(); // Inits pastMoves
        ConnectFourPlusUndo(const ConnectFourPlusUndo & other);
        ~ConnectFourPlusUndo () override; // Deletes created dynamic pastMoves array

        ConnectFourPlusUndo &operator=(const ConnectFourPlusUndo& other);
        void playGame () override; // Adds ability to call undoMove
        void undoMove(int &player, int column); /* This function works like play function but it removes from cell*/
                                                /* Also swaps player and makes sure that column isnt marked as full */
    protected:
        void initGame () override;  // Calls abstract init and inits the rest
        int getMove () override;    // Mostly Same as abstract class' getmove. But adds undo command
        void saveManagement (commandType command, const string &fileName, int &player) override ;
        // Manages UNDO command additionally

        int * pastMoves = nullptr;
        bool undoInit = false;
    };
}

#endif //HW5_CONNECTFOURPLUSUNDO_H
