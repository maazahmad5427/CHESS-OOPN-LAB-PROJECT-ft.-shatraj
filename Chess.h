#ifndef CHESS_H
#define CHESS_H

#include <iostream>
#include <string>
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

    bool isPathClear(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]);

public:
    Piece();
    Piece(Color c, char s);
    virtual ~Piece();

    Color getColor() const;
    char getSymbol() const;

    virtual bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) = 0;
};

// ==========================================
// INHERITANCE: Derived Piece Classes
// ==========================================
class Pawn : public Piece {
public:
    Pawn(Color c);
    bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) override;
};

class Rook : public Piece {
private:
    bool hasMoved;
public:
    Rook(Color c);
    bool getHasMoved() const;
    void setHasMoved(bool m);
    bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) override;
};

class Knight : public Piece {
public:
    Knight(Color c);
    bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) override;
};

class Bishop : public Piece {
public:
    Bishop(Color c);
    bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) override;
};

class Queen : public Piece {
public:
    Queen(Color c);
    bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) override;
};

class King : public Piece {
private:
    bool hasMoved;
public:
    King(Color c);
    bool getHasMoved() const;
    void setHasMoved(bool m);
    bool isValidMove(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) override;
};

// ==========================================
// COMPOSITION: Board contains Pieces
// ==========================================
class Board {
private:
    Piece* grid[8][8];

    void findKing(Color c, int& kingR, int& kingC) const;
    bool isSquareUnderAttack(int r, int c, Color opponentColor);

    bool canCastleKingside(Color c);
    bool canCastleQueenside(Color c);
    bool tryCastle(Color c, bool kingside);

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