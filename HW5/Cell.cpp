//
// Created by cecen on 11/23/2017.
//

#include "Cell.h"
namespace Abstract {
    Cell::Cell() {
        setCell(0,0,0);
    }

    Cell::Cell(int col, int row, int p) {
        setCell(col, row, p);
    }

    Cell::Cell (int val) {
        value = val;
    }

    void Cell::setCell(int col, int row, int p) {
        colNum = col;
        rowNum = row;
        player = p;

        switch (p){
            case 1:
                value = 1;
                break;
            case 10:
                value = 2;
                break;
            case 15:
                value = 2;
                break;
            default:
                value = 3;
                break;
        }
    }

    Cell & Cell::operator= (int val) {
        value = val;
        return *this;
    }

    bool Cell::operator== (const Cell & other) const{
        return value == other.value;
    }

    bool Cell::operator!= (const Cell & other) const{
        return *this == other;
    }

    Cell Cell::operator++ () {
        switch (value){
            case 1:
                value++;
                break;
            case 2:
                value++;
                break;
            case 3:
                value = 1;
                break;
            default:
                break;
        }
        return *this;
    }

    Cell Cell::operator++ (int) {
        Cell tmp = value;
        ++*this;
        return tmp;
    }

    Cell Cell::operator-- () {
        switch (value){
            case 1:
                value = 3;
                break;
            case 2:
                value--;
                break;
            case 3:
                value--;
                break;
            default:
                break;
        }
        return *this;
    }

    Cell Cell::operator-- (int) {
        Cell tmp = value;
        --*this;
        return tmp;
    }

    istream & operator>> (istream &input, Cell & other) {
        input >> other.value;
        return input;
    }

    ostream & operator<< (ostream &output, Cell & other) {
        output << other.value;
        return output;
    }
}