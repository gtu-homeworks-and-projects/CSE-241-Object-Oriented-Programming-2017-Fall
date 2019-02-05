//
// Created by cecen on 11/23/2017.
//

#ifndef HW5_CELL_H
#define HW5_CELL_H

#include <iostream>
#include <fstream>
using namespace std;
namespace Abstract {
    class Cell { /* Nothing has changed in this class. Its now just another class instead of inner */
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

    istream & operator>>(istream & input, Cell & );
    ostream & operator<<(ostream & output, Cell & );
}
#endif //HW5_CELL_H
