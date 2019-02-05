#include <iostream>
#include "ConnectFourDiag.h"
#include "ConnectFourPlus.h"
#include "ConnectFourPlusUndo.h"

using namespace Abstract;

char choiceInput(); // Takes legal game mode choice


int main() {
    char choice = choiceInput();
    ConnectFourAbstract * a = nullptr;
    switch (choice){
        case 'D':
            a = new ConnectFourDiag;
            break;
        case 'P':
            a = new ConnectFourPlus;
            break;
        case 'U':
            a = new ConnectFourPlusUndo;
            break;
        default:
            break;
    }
    if (a != nullptr) {
        a->playGame();
        delete a;
    }
    return 0;
}

char choiceInput() {
    bool isInputValid = false;
    char choice = '0';
    do{
        cout << "Choose game mode (D)iag, (P)lus or (U)ndo: " << endl;
        cin >> choice;
        if(choice == 'U' || choice == 'P' || choice == 'D'){
            isInputValid = true;
        } else cerr << "Please enter a valid choice!" << endl;
    } while(!isInputValid);

    return choice;
}