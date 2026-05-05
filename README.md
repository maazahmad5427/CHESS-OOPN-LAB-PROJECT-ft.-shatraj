# ♟️ Chess Game using Object-Oriented Programming (OOP)

## 📌 Project Overview

This project is a **Chess Game** developed using the core principles of **Object-Oriented Programming (OOP)**. The aim is to simulate a real chess match while demonstrating concepts such as **encapsulation, inheritance, polymorphism, and abstraction**.

The game follows standard chess rules and allows two players to play against each other in a structured and modular program design.

---

## 🎯 Objectives

* Implement a fully functional chess game.
* Apply OOP principles in a practical project.
* Create a clean and maintainable code structure.
* Simulate real-world chess mechanics using classes and objects.

---

## 🧱 OOP Concepts Used

### 1. Encapsulation

* Data members like piece position, type, and color are kept private.
* Access is controlled through public methods.

### 2. Inheritance

* A base class `Piece` is used.
* Derived classes include:

  * `Pawn`
  * `Rook`
  * `Knight`
  * `Bishop`
  * `Queen`
  * `King`

### 3. Polymorphism

* Function overriding is used for movement rules.
* Each piece has its own implementation of `move()`.

### 4. Abstraction

* Complex logic (like move validation) is hidden behind simple interfaces.
* Users interact with the game without needing to understand internal logic.

---

## 🧩 Features

* Two-player gameplay
* Valid move checking
* Turn-based system (White vs Black)
* Piece-specific movement rules
* Check and checkmate detection (optional if implemented)
* Board display in console/GUI

---

## 🏗️ Project Structure

```
ChessGame/
│
├── main.cpp
├── Board.cpp / Board.h
├── Piece.cpp / Piece.h
├── Pawn.cpp / Pawn.h
├── Rook.cpp / Rook.h
├── Knight.cpp / Knight.h
├── Bishop.cpp / Bishop.h
├── Queen.cpp / Queen.h
├── King.cpp / King.h
└── README.md
```

---

## ⚙️ How to Run

### Step 1: Compile the Program

```bash
g++ main.cpp Board.cpp Piece.cpp Pawn.cpp Rook.cpp Knight.cpp Bishop.cpp Queen.cpp King.cpp -o chess
```

### Step 2: Run the Program

```bash
./chess
```

*(On Windows, use `chess.exe`)*

---

## 🎮 How to Play

* The game is played between two players.
* Players enter moves in a coordinate format (e.g., `e2 e4`).
* The system validates the move and updates the board.
* Turns alternate automatically.

---

## 🚧 Future Improvements

* Add GUI (using libraries like SFML or Qt)
* Implement AI opponent
* Save/load game feature
* Add timers for players
* Improve check/checkmate detection

---

## 👨‍💻 Contributors

* Abeerah Amin / Maaz Ahmad

---

## 📄 License

This project is for educational purposes and can be modified or extended freely.

---

## 📌 Conclusion

This project demonstrates how OOP concepts can be used to model a real-world game like chess. It provides a strong foundation for building more advanced applications with clean and reusable code.

---
