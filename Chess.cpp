#include "Chess.h"
#include <iostream>
#include <cctype>

using namespace std;


// Board Methods


Board::Board() {
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            grid[r][c] = nullptr;
    setupBoard();
}

Board::~Board() {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (grid[r][c] != nullptr) delete grid[r][c];
        }
    }
}

void Board::findKing(Color c, int& kingR, int& kingC) const {
    for (int r = 0; r < 8; ++r) {
        for (int col = 0; col < 8; ++col) {
            Piece* p = grid[r][col];
            if (p != nullptr && p->getColor() == c && tolower(p->getSymbol()) == 'k') {
                kingR = r;
                kingC = col;
                return;
            }
        }
    }
}

bool Board::isSquareUnderAttack(int r, int c, Color opponentColor) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Piece* p = grid[i][j];
            if (p != nullptr && p->getColor() == opponentColor) {
                if (p->isValidMove(i, j, r, c, grid)) return true;
            }
        }
    }
    return false;
}

void Board::setupBoard() {
    grid[0][0] = new Rook(BLACK);
    grid[0][1] = new Knight(BLACK);
    grid[0][2] = new Bishop(BLACK);
    grid[0][3] = new Queen(BLACK);
    grid[0][4] = new King(BLACK);
    grid[0][5] = new Bishop(BLACK);
    grid[0][6] = new Knight(BLACK);
    grid[0][7] = new Rook(BLACK);

    for (int i = 0; i < 8; ++i) 
        grid[1][i] = new Pawn(BLACK);

    for (int i = 0; i < 8; ++i) 
        grid[6][i] = new Pawn(WHITE);
        grid[7][0] = new Rook(WHITE);
        grid[7][1] = new Knight(WHITE);
        grid[7][2] = new Bishop(WHITE);
        grid[7][3] = new Queen(WHITE);
        grid[7][4] = new King(WHITE);
        grid[7][5] = new Bishop(WHITE);
        grid[7][6] = new Knight(WHITE);
        grid[7][7] = new Rook(WHITE);
}

void Board::printBoard() const {
    cout << "\n    a   b   c   d   e   f   g   h\n";
    cout << "  +---+---+---+---+---+---+---+---+\n";
    for (int r = 0; r < 8; ++r) {
        cout << 8 - r << " |";
        for (int c = 0; c < 8; ++c) {
            if (grid[r][c] == nullptr) {
                cout << "   |";
            }
            else {
                cout << " " << grid[r][c]->getSymbol() << " |";
            }
        }
        cout << " " << 8 - r << "\n";
        cout << "  +---+---+---+---+---+---+---+---+\n";
    }
    cout << "    a   b   c   d   e   f   g   h\n\n";
}

bool Board::isInCheck(Color c) {
    int kingR = -1, kingC = -1;
    findKing(c, kingR, kingC);

    if (kingR == -1 || kingC == -1) { return false; }

    Color oppColor = (c == WHITE) ? BLACK : WHITE;
    return isSquareUnderAttack(kingR, kingC, oppColor);
}

bool Board::isCheckmate(Color c) {
    if (!isInCheck(c)) return false;

    for (int r = 0; r < 8; ++r) {
        for (int col = 0; col < 8; ++col) {
            Piece* p = grid[r][col];
            if (p != nullptr && p->getColor() == c) {
                for (int destR = 0; destR < 8; ++destR) {
                    for (int destC = 0; destC < 8; ++destC) {
                        if (grid[destR][destC] != nullptr && grid[destR][destC]->getColor() == c) continue;

                        if (p->isValidMove(r, col, destR, destC, grid)) {
                            Piece* captured = grid[destR][destC];
                            grid[destR][destC] = p;
                            grid[r][col] = nullptr;

                            bool stillInCheck = isInCheck(c);

                            grid[r][col] = p;
                            grid[destR][destC] = captured;

                            if (!stillInCheck) return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

bool Board::movePiece(Color currentTurn, int srcR, int srcC, int destR, int destC) {
    Piece* p = grid[srcR][srcC];

    if (p == nullptr) {
        cout << "No piece at the starting position!\n";
        return false;
    }
    if (p->getColor() != currentTurn) {
        cout << "You can only move your own pieces!\n";
        return false;
    }
    if (grid[destR][destC] != nullptr && grid[destR][destC]->getColor() == currentTurn) {
        cout << "You cannot capture your own piece!\n";
        return false;
    }
    if (!p->isValidMove(srcR, srcC, destR, destC, grid)) {
        cout << "Invalid move for this piece!\n";
        return false;
    }

    Piece* capturedPiece = grid[destR][destC];
    grid[destR][destC] = p;
    grid[srcR][srcC] = nullptr;

    bool selfCheck = isInCheck(currentTurn);

    grid[srcR][srcC] = p;
    grid[destR][destC] = capturedPiece;

    if (selfCheck) {
        cout << "Invalid move! You cannot put or leave your King in check.\n";
        return false;
    }

    if (capturedPiece != nullptr) {
        delete capturedPiece;
    }
    grid[destR][destC] = p;
    grid[srcR][srcC] = nullptr;

    return true;
}

// ==========================================
// Game Methods
// ==========================================

Game::Game() : currentTurn(WHITE) {}

bool Game::parseInput(string input, int& r, int& c) {
    if (input.length() != 2) return false;
    c = input[0] - 'a';
    r = 8 - (input[1] - '0');
    return (c >= 0 && c < 8 && r >= 0 && r < 8);
}

void Game::play() {
    cout << "=== Console Chess ===\n";
    cout << "Uppercase letters (P, R, N...) are WHITE.\n";
    cout << "Lowercase letters (p, r, n...) are BLACK.\n";
    cout << "Enter moves using standard coordinates (e.g., e2 e4).\n";

    string srcInput, destInput;
    bool gameOver = false;

    while (!gameOver) {
        board.printBoard();

        if (board.isCheckmate(currentTurn)) {
            Color winner = (currentTurn == WHITE) ? BLACK : WHITE;
            cout << "\nCHECKMATE! " << (winner == WHITE ? "White" : "Black") << " wins!\n";
            break;
        }
        else if (board.isInCheck(currentTurn)) {
            cout << "\n*** CHECK! ***\n";
        }

        cout << (currentTurn == WHITE ? "White's Turn" : "Black's Turn") << "\n";
        cout << "Enter move (start end): ";
        cin >> srcInput >> destInput;

        int srcR, srcC, destR, destC;
        if (parseInput(srcInput, srcR, srcC) && parseInput(destInput, destR, destC)) {
            if (board.movePiece(currentTurn, srcR, srcC, destR, destC)) {
                currentTurn = (currentTurn == WHITE) ? BLACK : WHITE;
            }
        }
        else {
            cout << "Invalid coordinate format! Use a-h and 1-8 (e.g., e2 e4).\n";
        }
    }
}
