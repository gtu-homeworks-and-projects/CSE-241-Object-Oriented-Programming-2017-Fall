//
// Created by cecen on 11/25/2017.
//

#ifndef HW5_CONNECTFOURDIAG_H
#define HW5_CONNECTFOURDIAG_H

#include "ConnectFourAbstract.h"
namespace Abstract {
    class ConnectFourDiag: public ConnectFourAbstract  { /* This derived class of ConnectFourAbstract only detects diagonal wins */
    public:
        ConnectFourDiag() = default;
        ConnectFourDiag(int , int, bool);

    protected:
        int checkFour(int col) override;    // Checks win situations
        int checkFourDiagonal1(int col);
        int checkFourDiagonal2(int col);

        int getAiMove() override;        // Gets legal ai move
        int evaluateBoard() override;    // Ai move calculation
        int aiScanDiagonal1();           //
        int aiScanDiagonal2();           //
    };
}

#endif //HW5_CONNECTFOURDIAG_H
