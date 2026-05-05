#ifndef CHESS_H
#define CHESS_H

#include <string>
#include <cmath>
using namespace std;

// Enum for piece colors
enum Color { WHITE, BLACK, NONE };

// ==========================================
// ENCAPSULATION & ABSTRACTION: Base Class
// ==========================================
class Piece {
protected:
    Color color;
    char symbol;

    bool isPathClear(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) {
        int rowStep = (destRow - srcRow) == 0 ? 0 : (destRow - srcRow) / abs(destRow - srcRow);
        int colStep = (destCol - srcCol) == 0 ? 0 : (destCol - srcCol) / abs(destCol - srcCol);

        int r = srcRow + rowStep;
        int c = srcCol + colStep;

        while (r != destRow || c != destCol) {
            if (board[r][c] != nullptr) return false;
            r += rowStep;
            c += colStep;
        }
        return true;
    }

public:
    Piece(Color c, char s) : color(c), symbol(s) {}
    virtual ~Piece() {}

    Color getColor() const { 
        return color;
    }

    char getSymbol() const { 
        return symbol;
    }

    virtual bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) = 0;
};

// ==========================================
// INHERITANCE: Derived Piece Classes
// ==========================================
class Pawn : public Piece {
public:
    Pawn(Color c) : Piece(c, c == WHITE ? 'P' : 'p') {}

    bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) override {
        int direction = (color == WHITE) ? -1 : 1;
        int startRow = (color == WHITE) ? 6 : 1;

        if (destCol == srcCol && destRow == srcRow + direction && board[destRow][destCol] == nullptr) {
            return true;
        }
        
        if (destCol == srcCol && srcRow == startRow && destRow == srcRow + 2 * direction &&
            board[srcRow + direction][srcCol] == nullptr && board[destRow][destCol] == nullptr) {
            return true;
        }
        if (std::abs(destCol - srcCol) == 1 && destRow == srcRow + direction && board[destRow][destCol] != nullptr) {
            return board[destRow][destCol]->getColor() != this->color;
        }
        return false;
    }
};

class Rook : public Piece {
public:
    Rook(Color c) : Piece(c, c == WHITE ? 'R' : 'r') {}
    bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) override {
        if (srcRow != destRow && srcCol != destCol) {
            return false;
        }
        return isPathClear(srcRow, srcCol, destRow, destCol, board);
    }
};

class Knight : public Piece {
public:
    Knight(Color c) : Piece(c, c == WHITE ? 'N' : 'n') {}
    bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) override {
        int rowDiff = std::abs(destRow - srcRow);
        int colDiff = std::abs(destCol - srcCol);
        return (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2);
    }
};

class Bishop : public Piece {
public:
    Bishop(Color c) : Piece(c, c == WHITE ? 'B' : 'b') {}
    bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) override {
        if (std::abs(destRow - srcRow) != std::abs(destCol - srcCol)) {
            return false;
        }
        return isPathClear(srcRow, srcCol, destRow, destCol, board);
    }
};

class Queen : public Piece {
public:
    Queen(Color c) : Piece(c, c == WHITE ? 'Q' : 'q') {}
    bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) override {
        if (srcRow == destRow || srcCol == destCol || std::abs(destRow - srcRow) == std::abs(destCol - srcCol)) {
            return isPathClear(srcRow, srcCol, destRow, destCol, board);
        }
        return false;
    }
};

class King : public Piece {
public:
    King(Color c) : Piece(c, c == WHITE ? 'K' : 'k') {}
    bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) override {
        int rowDiff = std::abs(destRow - srcRow);
        int colDiff = std::abs(destCol - srcCol);
        return (rowDiff <= 1 && colDiff <= 1);
    }
};

// ==========================================
// COMPOSITION: Board contains Pieces
// ==========================================
class Board {
private:
    Piece* grid[8][8];
    void findKing(Color c, int& kingR, int& kingC) const;
    bool isSquareUnderAttack(int r, int c, Color opponentColor);

public:
    Board();
    ~Board();
    void setupBoard();
    void printBoard() const;
    bool isInCheck(Color c);
    bool isCheckmate(Color c);
    bool movePiece(Color currentTurn, int srcR, int srcC, int destR, int destC);
};

// ==========================================
// GAME ENGINE
// ==========================================
class Game {
private:
    Board board;
    Color currentTurn;
    bool parseInput(std::string input, int& r, int& c);

public:
    Game();
    void play();
};

#endif // CHESS_H#pragma once
