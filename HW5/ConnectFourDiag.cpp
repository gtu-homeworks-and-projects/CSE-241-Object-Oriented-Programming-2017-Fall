//
// Created by cecen on 11/25/2017.
//

#include "ConnectFourDiag.h"
namespace Abstract {
    ConnectFourDiag::ConnectFourDiag (int height, int width, bool isCompPlaying) : ConnectFourAbstract(height,width,isCompPlaying) {
        /* Same constructor works for this type of constructor */
    }

    int ConnectFourDiag::checkFour(int col) {
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

    int ConnectFourDiag::checkFourDiagonal1(int col) {
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

    int ConnectFourDiag::checkFourDiagonal2(int col) {
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

    int ConnectFourDiag::getAiMove() {
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

    int ConnectFourDiag::evaluateBoard() {
        int lineState = 0;
        if (aiScanDiagonal1() >= 2) lineState = 3;
        else if (aiScanDiagonal2() >= 2) lineState = 4;
        return lineState;
    }

    int ConnectFourDiag::aiScanDiagonal1() {
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

    int ConnectFourDiag::aiScanDiagonal2() {
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

}