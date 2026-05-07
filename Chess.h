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

    Color getColor() const { return color; }
    char getSymbol() const { return symbol; }

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

        // Single step forward
        if (destCol == srcCol && destRow == srcRow + direction && board[destRow][destCol] == nullptr)
            return true;

        // Double step from starting row
        if (destCol == srcCol && srcRow == startRow && destRow == srcRow + 2 * direction &&
            board[srcRow + direction][srcCol] == nullptr && board[destRow][destCol] == nullptr)
            return true;

        // Diagonal capture
        if (abs(destCol - srcCol) == 1 && destRow == srcRow + direction && board[destRow][destCol] != nullptr)
            return board[destRow][destCol]->getColor() != this->color;

        return false;
    }
};

// ==========================================
// Rook — tracks whether it has ever moved (needed for castling)
// ==========================================
class Rook : public Piece {
private:
    bool hasMoved;
public:
    Rook(Color c) : Piece(c, c == WHITE ? 'R' : 'r'), hasMoved(false) {}

    bool getHasMoved() const { return hasMoved; }
    void setHasMoved(bool m) { hasMoved = m; }

    bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) override {
        if (srcRow != destRow && srcCol != destCol) return false;
        return isPathClear(srcRow, srcCol, destRow, destCol, board);
    }
};

class Knight : public Piece {
public:
    Knight(Color c) : Piece(c, c == WHITE ? 'N' : 'n') {}
    bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) override {
        int rowDiff = abs(destRow - srcRow);
        int colDiff = abs(destCol - srcCol);
        return (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2);
    }
};

class Bishop : public Piece {
public:
    Bishop(Color c) : Piece(c, c == WHITE ? 'B' : 'b') {}
    bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) override {
        if (abs(destRow - srcRow) != abs(destCol - srcCol)) return false;
        return isPathClear(srcRow, srcCol, destRow, destCol, board);
    }
};

class Queen : public Piece {
public:
    Queen(Color c) : Piece(c, c == WHITE ? 'Q' : 'q') {}
    bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) override {
        if (srcRow == destRow || srcCol == destCol ||
            abs(destRow - srcRow) == abs(destCol - srcCol))
            return isPathClear(srcRow, srcCol, destRow, destCol, board);
        return false;
    }
};

// ==========================================
// King — tracks whether it has ever moved (needed for castling)
// ==========================================
class King : public Piece {
private:
    bool hasMoved;
public:
    King(Color c) : Piece(c, c == WHITE ? 'K' : 'k'), hasMoved(false) {}

    bool getHasMoved() const { return hasMoved; }
    void setHasMoved(bool m) { hasMoved = m; }

    bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) override {
        int rowDiff = abs(destRow - srcRow);
        int colDiff = abs(destCol - srcCol);
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

    // Castling helpers
    bool canCastleKingside(Color c);
    bool canCastleQueenside(Color c);
    bool tryCastle(Color c, bool kingside);

    // Pawn promotion helper
    void handlePromotion(int r, int c, Color c_color);

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
    bool parseInput(string input, int& r, int& c);

public:
    Game();
    void play();
};

#endif // CHESS_H