//
// Created by cecen on 1.11.2017.
//

#include <iostream>
#include "ConnectFour.h"
#include "ctime"
using namespace std;

char choiceInput(); // Takes legal game mode choice

int main() {
    srand(time(NULL));

    char choice = choiceInput();
    if('M' == choice)
        playFiveCF();
    if('S' == choice){
        ConnectFour a(true);
        a.playGame();
    }

    return 0;
}

char choiceInput() {
    bool isInputValid = false;
    char choice = '0';
    do{
        cout << "Choose game mode (S or M): " << endl;
        cin >> choice;

        if(choice == 'S' || choice == 'M'){
            isInputValid = true;
        } else cerr << "Please enter a valid choice!" << endl;
    } while(!isInputValid);

    return choice;
}