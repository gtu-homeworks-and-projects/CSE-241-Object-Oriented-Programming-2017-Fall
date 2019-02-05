//
// Created by cecen on 11/25/2017.
//

#include "ConnectFourPlus.h"
namespace Abstract{
    ConnectFourPlus::ConnectFourPlus (int height, int width, bool isCompPlaying) : ConnectFourAbstract(height,width,isCompPlaying) {
        /* Same constructor works for this type of constructor */
    }

    int ConnectFourPlus::checkFour(int col) {
        if (checkFourVertical(col) >= 4) {
            for (int i = winningCoords.endingC[0], count = 0; i <= winningCoords.startingC[0] && count < 4; ++i, ++count) {
                gameCells[i][col] = gameCells[i][col] == 1 ? 4 : 5;
            }
            return 1;
        }

        if (checkFourHorizontal(col) >= 4) {
            for (int i = winningCoords.endingC[1], count = 0; i <= winningCoords.startingC[1] && count < 4; ++i, ++count) {
                gameCells[winningCoords.startingC[0]][i] = gameCells[winningCoords.startingC[0]][i] == 1 ? 4 : 5;
            }
            return 2;
        }

        return 0;
    }

    int ConnectFourPlus::checkFourVertical(int col) {
        int i,j,k,l;

        for (l = 0; l < boardSize.height && gameCells[l][col] == 0; ++l);
        for (i = l; gameCells[i][col] == 3 && i < boardSize.height; ++i);
        int lineCheck = 1; //Counts last played cell as one

        /* Scans to the up */
        for (k = col, j = i; j < boardSize.height - 1 && gameCells[j][k] == gameCells[j + 1][k];
             j++, lineCheck++);
        winningCoords.startingC[0] = j;
        winningCoords.startingC[1] = k;

        /* Scans to the down */
        for (j = i; j > 0 && gameCells[j][k] == gameCells[j - 1][k]; j--, lineCheck++);
        winningCoords.endingC[0] = j;
        winningCoords.endingC[1] = k;

        return lineCheck;
    }

    int ConnectFourPlus::checkFourHorizontal (int col) {
        int i,j,k,l;

        for (l = 0; l < boardSize.height && gameCells[l][col] == 0; ++l);
        for (i = l; gameCells[i][col] == 3 && i < boardSize.height; ++i);
        int lineCheck = 1; // Counts last played cell as one

        /* Scans to the right */
        for (j = i, k = col;
             j < boardSize.height && k < boardSize.width - 1 && gameCells[j][k] == gameCells[j][k + 1];
             k++, lineCheck++);
        winningCoords.startingC[0] = j;
        winningCoords.startingC[1] = k;

        /* Scans to the left */
        for (j = i, k = col; k > 0 && gameCells[j][k] == gameCells[j][k - 1];
             k--, lineCheck++);
        winningCoords.endingC[0] = j;
        winningCoords.endingC[1] = k;

        return lineCheck;
    }

    int ConnectFourPlus::getAiMove() {
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

    int ConnectFourPlus::evaluateBoard() {
        int lineState = 0;
        if (aiScanVertical() >= 2) lineState = 1;
        else if (aiScanHorizontal() >= 2) lineState = 2;
        return lineState;
    }

    int ConnectFourPlus::aiScanVertical() {
        int lineCheck = 1; // Counts last played cell as one
        int longestLine = 0, tempBlockLine = 0;
        int endingTempBlockCoords[2];

        /* Block attempt */
        for (int i = 0; i < boardSize.width; ++i) {
            for (int j = 0; j < boardSize.height - 1; ++j) {
                while (gameCells[j][i] == gameCells[j + 1][i] &&
                       gameCells[j][i] == 1 && !boardSize.isColFull[i]) {
                    ++lineCheck;
                    if (lineCheck >= tempBlockLine) {
                        tempBlockLine = lineCheck;
                        endingTempBlockCoords[0] = j + 1;
                        endingTempBlockCoords[1] = i;
                    }
                    if(j < boardSize.height - 1)
                        ++j;
                }
                lineCheck = 1;
            }
        }

        /* Win attempt */
        for (int i = 0; i < boardSize.width; ++i) {
            for (int j = 0; j < boardSize.height - 1; ++j) {
                while (gameCells[j][i] == gameCells[j + 1][i] &&
                       gameCells[j][i] == 2 && !boardSize.isColFull[i]) {
                    ++lineCheck;
                    if (lineCheck >= longestLine) {
                        longestLine = lineCheck;
                        aiWinningCoords.endingC[0] = j + 1;
                        aiWinningCoords.endingC[1] = i;
                    }
                    if(j < boardSize.height - 1)
                        ++j;
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
        aiWinningCoords.startingC[1] = aiWinningCoords.endingC[1];


        return longestLine;
    }

    int ConnectFourPlus::aiScanHorizontal() {
        int lineCheck = 1; // Counts last played cell as one
        int longestLine = 0, tempBlockLine = 0;
        int endingTempBlockCoords[2];

        /* Block attempt */
        for (int i = 0; i < boardSize.height; ++i) {
            for (int j = 0; j < boardSize.width - 1; ++j) {
                while (gameCells[i][j] == gameCells[i][j + 1] &&
                       gameCells[i][j] == 1) {
                    ++lineCheck;
                    if (lineCheck >= tempBlockLine) {
                        tempBlockLine = lineCheck;
                        endingTempBlockCoords[0] = i;
                        endingTempBlockCoords[1] = j + 1;
                    }
                    if(j < boardSize.width - 1)
                        ++j;
                }
            }
        }
        /* Win attempt */
        for (int i = 0; i < boardSize.height; ++i) {
            for (int j = 0; j < boardSize.width - 1; ++j) {
                while (gameCells[i][j] == gameCells[i][j + 1] &&
                       gameCells[i][j] == 1) {
                    ++lineCheck;
                    if (lineCheck >= longestLine) {
                        longestLine = lineCheck;
                        aiWinningCoords.endingC[0] = i;
                        aiWinningCoords.endingC[1] = j + 1;
                    }
                    if(j < boardSize.width - 1)
                        ++j;
                }
            }
        }

        if (tempBlockLine > 2) {
            aiWinningCoords.endingC[0] = endingTempBlockCoords[0];
            aiWinningCoords.endingC[1] = endingTempBlockCoords[1];
            longestLine = tempBlockLine;
        }

        aiWinningCoords.startingC[0] = aiWinningCoords.endingC[0];
        aiWinningCoords.startingC[1] = aiWinningCoords.endingC[1] - (lineCheck - 1);

        return longestLine;
    }

}