#include "Chess.h"
#include <iostream>
#include <string>
using namespace std;

// ==========================================
// MAIN MENU HELPERS
// ==========================================

void showMainMenu() {
    cout << "\n";
    cout << "  +==================================+\n";
    cout << "  |       ***  CONSOLE CHESS  ***    |\n";
    cout << "  +==================================+\n";
    cout << "  |   1.  New Game                   |\n";
    cout << "  |   2.  How to Play                |\n";
    cout << "  |   3.  Quit                       |\n";
    cout << "  +==================================+\n";
    cout << "  Enter your choice: ";
}

void showHowToPlay() {
    cout << "\n";
    cout << "  +============================================+\n";
    cout << "  |              HOW TO PLAY                  |\n";
    cout << "  +============================================+\n";
    cout << "  PIECES  (Uppercase = White, Lowercase = Black)\n";
    cout << "    K/k = King    Q/q = Queen   R/r = Rook\n";
    cout << "    B/b = Bishop  N/n = Knight  P/p = Pawn\n\n";
    cout << "  ENTERING MOVES\n";
    cout << "    Type: <from> <to>  (e.g., e2 e4)\n";
    cout << "    Columns: a-h   |   Rows: 1-8\n\n";
    cout << "  SPECIAL MOVES\n";
    cout << "    Castling   : Move king two squares\n";
    cout << "                 Kingside  -> e1 g1  (white)\n";
    cout << "                 Queenside -> e1 c1  (white)\n";
    cout << "    Promotion  : Pawn reaching last rank;\n";
    cout << "                 choose Q / R / B / N\n\n";
    cout << "  COMMANDS DURING GAME\n";
    cout << "    resign     : Forfeit the current game\n\n";
    cout << "  RULES\n";
    cout << "    - You cannot leave your King in check.\n";
    cout << "    - Checkmate ends the game.\n";
    cout << "  +============================================+\n";
    cout << "\n  Press Enter to return to the menu...";
    cin.ignore(1000, '\n');
    cin.get();
}

// ==========================================
// MAIN — Menu Loop
// ==========================================

int main() {
    string choice;

    while (true) {
        showMainMenu();
        cin >> choice;

        if (choice == "1") {
            Game chessGame;
            chessGame.play();
            cout << "\n  Press Enter to return to the menu...";
            cin.ignore(1000, '\n');
            cin.get();

        }
        else if (choice == "2") {
            cin.ignore(1000, '\n');
            showHowToPlay();

        }
        else if (choice == "3") {
            cout << "\n  Thanks for playing! Goodbye.\n\n";
            break;

        }
        else {
            cout << "  Invalid choice. Please enter 1, 2, or 3.\n";
        }
    }

    return 0;
}