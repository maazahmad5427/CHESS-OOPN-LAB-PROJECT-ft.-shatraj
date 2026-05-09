#include "Chess.h"
#include <iostream>
#include <cctype>
#include <cmath>

using namespace std;

// ==========================================
// Piece  Base Class Definitions
// ==========================================

Piece::Piece() : color(NONE), symbol(' ') {}

Piece::Piece(Color c, char s) : color(c), symbol(s) {}

Piece::~Piece() {}

Color Piece::getColor() const { return color; }

char Piece::getSymbol() const { return symbol; }

bool Piece::isPathClear(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) {
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

// ==========================================
// Pawn Definitions
// ==========================================

Pawn::Pawn(Color c) : Piece(c, c == WHITE ? 'P' : 'p') {}

bool Pawn::isValidMove(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) {
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

// ==========================================
// Rook Definitions
// ==========================================

Rook::Rook(Color c) : Piece(c, c == WHITE ? 'R' : 'r'), hasMoved(false) {}

bool Rook::getHasMoved() const { return hasMoved; }

void Rook::setHasMoved(bool m) { hasMoved = m; }

bool Rook::isValidMove(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) {
    if (srcRow != destRow && srcCol != destCol) return false;
    return isPathClear(srcRow, srcCol, destRow, destCol, board);
}

// ==========================================
// Knight Definitions
// ==========================================

Knight::Knight(Color c) : Piece(c, c == WHITE ? 'N' : 'n') {}

bool Knight::isValidMove(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) {
    int rowDiff = abs(destRow - srcRow);
    int colDiff = abs(destCol - srcCol);
    return (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2);
}

// ==========================================
// Bishop Definitions
// ==========================================

Bishop::Bishop(Color c) : Piece(c, c == WHITE ? 'B' : 'b') {}

bool Bishop::isValidMove(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) {
    if (abs(destRow - srcRow) != abs(destCol - srcCol)) return false;
    return isPathClear(srcRow, srcCol, destRow, destCol, board);
}

// ==========================================
// Queen Definitions
// ==========================================

Queen::Queen(Color c) : Piece(c, c == WHITE ? 'Q' : 'q') {}

bool Queen::isValidMove(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) {
    if (srcRow == destRow || srcCol == destCol ||
        abs(destRow - srcRow) == abs(destCol - srcCol))
        return isPathClear(srcRow, srcCol, destRow, destCol, board);
    return false;
}

// ==========================================
// King Definitions
// ==========================================

King::King(Color c) : Piece(c, c == WHITE ? 'K' : 'k'), hasMoved(false) {}

bool King::getHasMoved() const { return hasMoved; }

void King::setHasMoved(bool m) { hasMoved = m; }

bool King::isValidMove(int srcRow, int srcCol, int destRow, int destCol, Piece* board[8][8]) {
    int rowDiff = abs(destRow - srcRow);
    int colDiff = abs(destCol - srcCol);
    return (rowDiff <= 1 && colDiff <= 1);
}

// ==========================================
// Board Definitions
// ==========================================

Board::Board() {
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            grid[r][c] = nullptr;
    setupBoard();
}

Board::~Board() {
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            if (grid[r][c] != nullptr) delete grid[r][c];
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
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j) {
            Piece* p = grid[i][j];
            if (p != nullptr && p->getColor() == opponentColor)
                if (p->isValidMove(i, j, r, c, grid)) return true;
        }
    return false;
}

void Board::setupBoard() {
    // Black back rank
    grid[0][0] = new Rook(BLACK);
    grid[0][1] = new Knight(BLACK);
    grid[0][2] = new Bishop(BLACK);
    grid[0][3] = new Queen(BLACK);
    grid[0][4] = new King(BLACK);
    grid[0][5] = new Bishop(BLACK);
    grid[0][6] = new Knight(BLACK);
    grid[0][7] = new Rook(BLACK);

    // Black pawns
    for (int i = 0; i < 8; ++i)
        grid[1][i] = new Pawn(BLACK);

    // White pawns
    for (int i = 0; i < 8; ++i)
        grid[6][i] = new Pawn(WHITE);

    // White back rank
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
            if (grid[r][c] == nullptr)
                cout << "   |";
            else
                cout << " " << grid[r][c]->getSymbol() << " |";
        }
        cout << " " << 8 - r << "\n";
        cout << "  +---+---+---+---+---+---+---+---+\n";
    }
    cout << "    a   b   c   d   e   f   g   h\n\n";
}

bool Board::isInCheck(Color c) {
    int kingR = -1, kingC = -1;
    findKing(c, kingR, kingC);
    if (kingR == -1) return false;
    Color opp = (c == WHITE) ? BLACK : WHITE;
    return isSquareUnderAttack(kingR, kingC, opp);
}

bool Board::isCheckmate(Color c) {
    if (!isInCheck(c)) return false;

    for (int r = 0; r < 8; ++r) {
        for (int col = 0; col < 8; ++col) {
            Piece* p = grid[r][col];
            if (p == nullptr || p->getColor() != c) continue;

            for (int dr = 0; dr < 8; ++dr) {
                for (int dc = 0; dc < 8; ++dc) {
                    if (grid[dr][dc] != nullptr && grid[dr][dc]->getColor() == c) continue;
                    if (!p->isValidMove(r, col, dr, dc, grid)) continue;

                    // Simulate move
                    Piece* captured = grid[dr][dc];
                    grid[dr][dc] = p;
                    grid[r][col] = nullptr;

                    bool stillInCheck = isInCheck(c);

                    // Undo
                    grid[r][col] = p;
                    grid[dr][dc] = captured;

                    if (!stillInCheck) return false;
                }
            }
        }
    }
    return true;
}

// ==========================================
// Castling Definitions
// ==========================================

bool Board::canCastleKingside(Color c) {
    int row = (c == WHITE) ? 7 : 0;

    Piece* king = grid[row][4];
    if (king == nullptr || tolower(king->getSymbol()) != 'k') return false;
    if (static_cast<King*>(king)->getHasMoved()) return false;

    Piece* rook = grid[row][7];
    if (rook == nullptr || tolower(rook->getSymbol()) != 'r') return false;
    if (static_cast<Rook*>(rook)->getHasMoved()) return false;

    if (grid[row][5] != nullptr || grid[row][6] != nullptr) return false;

    Color opp = (c == WHITE) ? BLACK : WHITE;
    if (isSquareUnderAttack(row, 4, opp)) return false;
    if (isSquareUnderAttack(row, 5, opp)) return false;
    if (isSquareUnderAttack(row, 6, opp)) return false;

    return true;
}

bool Board::canCastleQueenside(Color c) {
    int row = (c == WHITE) ? 7 : 0;

    Piece* king = grid[row][4];
    if (king == nullptr || tolower(king->getSymbol()) != 'k') return false;
    if (static_cast<King*>(king)->getHasMoved()) return false;

    Piece* rook = grid[row][0];
    if (rook == nullptr || tolower(rook->getSymbol()) != 'r') return false;
    if (static_cast<Rook*>(rook)->getHasMoved()) return false;

    if (grid[row][1] != nullptr || grid[row][2] != nullptr || grid[row][3] != nullptr) return false;

    Color opp = (c == WHITE) ? BLACK : WHITE;
    if (isSquareUnderAttack(row, 4, opp)) return false;
    if (isSquareUnderAttack(row, 3, opp)) return false;
    if (isSquareUnderAttack(row, 2, opp)) return false;

    return true;
}

bool Board::tryCastle(Color c, bool kingside) {
    if (kingside && !canCastleKingside(c)) { cout << "Kingside castling not available!\n";  return false; }
    if (!kingside && !canCastleQueenside(c)) { cout << "Queenside castling not available!\n"; return false; }

    int row = (c == WHITE) ? 7 : 0;
    int kingCol = 4;
    int rookFrom = kingside ? 7 : 0;
    int kingTo = kingside ? 6 : 2;
    int rookTo = kingside ? 5 : 3;

    King* king = static_cast<King*>(grid[row][kingCol]);
    Rook* rook = static_cast<Rook*>(grid[row][rookFrom]);

    grid[row][kingTo] = king;
    grid[row][rookTo] = rook;
    grid[row][kingCol] = nullptr;
    grid[row][rookFrom] = nullptr;

    king->setHasMoved(true);
    rook->setHasMoved(true);

    cout << (kingside ? "Kingside" : "Queenside") << " castling performed!\n";
    return true;
}

// ==========================================
// Pawn Promotion Definition
// ==========================================

void Board::handlePromotion(int r, int c, Color pieceColor) {
    cout << "Pawn promotion! Choose a piece:\n";
    cout << "  Q = Queen   R = Rook   B = Bishop   N = Knight\n";
    cout << "Your choice: ";

    char choice;
    while (true) {
        cin >> choice;
        choice = toupper(choice);
        if (choice == 'Q' || choice == 'R' || choice == 'B' || choice == 'N') break;
        cout << "Invalid choice. Enter Q, R, B, or N: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }

    delete grid[r][c];
    switch (choice) {
    case 'Q': grid[r][c] = new Queen(pieceColor);  break;
    case 'R': grid[r][c] = new Rook(pieceColor);   break;
    case 'B': grid[r][c] = new Bishop(pieceColor); break;
    case 'N': grid[r][c] = new Knight(pieceColor); break;
    }
    cout << "Promoted to "
        << (choice == 'Q' ? "Queen" : choice == 'R' ? "Rook" : choice == 'B' ? "Bishop" : "Knight")
        << "!\n";
}

// ==========================================
// movePiece Definition
// ==========================================

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

    // Castling detection
    if (tolower(p->getSymbol()) == 'k' && srcR == destR && abs(destC - srcC) == 2) {
        bool kingside = (destC > srcC);
        return tryCastle(currentTurn, kingside);
    }

    // Normal move validation
    if (grid[destR][destC] != nullptr && grid[destR][destC]->getColor() == currentTurn) {
        cout << "You cannot capture your own piece!\n";
        return false;
    }
    if (!p->isValidMove(srcR, srcC, destR, destC, grid)) {
        cout << "Invalid move for this piece!\n";
        return false;
    }

    // Simulate move to check for self-check
    Piece* capturedPiece = grid[destR][destC];
    grid[destR][destC] = p;
    grid[srcR][srcC] = nullptr;

    bool selfCheck = isInCheck(currentTurn);

    // Undo simulation
    grid[srcR][srcC] = p;
    grid[destR][destC] = capturedPiece;

    if (selfCheck) {
        cout << "Invalid move! You cannot put or leave your King in check.\n";
        return false;
    }

    // Commit move
    if (capturedPiece != nullptr) delete capturedPiece;
    grid[destR][destC] = p;
    grid[srcR][srcC] = nullptr;

    // Mark King/Rook as moved
    if (tolower(p->getSymbol()) == 'k')
        static_cast<King*>(p)->setHasMoved(true);
    else if (tolower(p->getSymbol()) == 'r')
        static_cast<Rook*>(p)->setHasMoved(true);

    // Pawn promotion check
    if (tolower(p->getSymbol()) == 'p') {
        int promRow = (p->getColor() == WHITE) ? 0 : 7;
        if (destR == promRow)
            handlePromotion(destR, destC, p->getColor());
    }

    return true;
}

// ==========================================
// Game Definitions
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
    cout << "To castle, move your king two squares: e1 g1 (kingside) or e1 c1 (queenside).\n\n";

    string srcInput, destInput;

    while (true) {
        board.printBoard();

        if (board.isCheckmate(currentTurn)) {
            Color winner = (currentTurn == WHITE) ? BLACK : WHITE;
            cout << "\nCHECKMATE! " << (winner == WHITE ? "White" : "Black") << " wins!\n";
            break;
        }
        if (board.isInCheck(currentTurn))
            cout << "\n*** CHECK! ***\n";

        cout << (currentTurn == WHITE ? "White's Turn" : "Black's Turn") << "\n";
        cout << "Enter move (start end): ";
        cin >> srcInput >> destInput;

        int srcR, srcC, destR, destC;
        if (parseInput(srcInput, srcR, srcC) && parseInput(destInput, destR, destC)) {
            if (board.movePiece(currentTurn, srcR, srcC, destR, destC))
                currentTurn = (currentTurn == WHITE) ? BLACK : WHITE;
        }
        else {
            cout << "Invalid coordinate format! Use a-h and 1-8 (e.g., e2 e4).\n";
        }
    }
}