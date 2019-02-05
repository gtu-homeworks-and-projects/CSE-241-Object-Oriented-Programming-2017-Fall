//
// Created by cecen on 11/25/2017.
//

#ifndef HW5_CONNECTFOURPLUS_H
#define HW5_CONNECTFOURPLUS_H

#include "ConnectFourAbstract.h"
namespace Abstract{
    class ConnectFourPlus: public ConnectFourAbstract { /* This derived class of ConnectFourAbstract only detects horizontal wins */
    public:
        ConnectFourPlus() = default;
        ConnectFourPlus(int, int , bool);

    protected:
        int checkFour(int col) override;    // Checks win situations
        int checkFourVertical(int col);     //
        int checkFourHorizontal(int col);   //

        int getAiMove() override;       // Gets legal ai move
        int evaluateBoard() override;   // Ai move calculation
        int aiScanVertical();           //
        int aiScanHorizontal();         //
    };
}

#endif //HW5_CONNECTFOURPLUS_H
