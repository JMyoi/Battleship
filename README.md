Battleship Game

CSC 710 – Software Engineering Course Project

This project implements the classic two-player Battleship game using C++ with a graphical user interface built using the Raylib library. The system follows the traditional Battleship game rules where players place ships on a 10×10 board and take turns firing at the opponent’s board until all ships are destroyed.

The project demonstrates modular software design, separation of game logic and user interface, and collaborative development using GitHub.

Game Features

Two-player Battleship gameplay

10×10 game board (columns A–J, rows 1–10)

Selectable number of ships (1–5)

Ship placement with rotation support

Turn-based firing system

Hit and miss detection

Ship destruction detection

Game over and winner announcement

Graphical user interface built with Raylib

System Architecture

The system is designed using a modular architecture that separates the core game logic from the graphical user interface.

Main components include:

Game Controller
Manages the overall game flow and turn switching between players.

Player
Represents each player and maintains their boards.

Board
Stores the 10×10 grid, ship placements, and shot results.

Ship
Represents ships with different sizes and tracks their hit status.

This design allows the core logic to be tested independently before integrating the graphical interface.

Technologies Used

C++

Raylib (Graphics Library)

Visual Studio Code

MSYS2 / UCRT64

GitHub (Version Control)

How to Run the Project
Requirements

C++ compiler (g++)

Raylib installed

MSYS2 / UCRT64 environment

Steps

Clone the repository

git clone https://github.com/JMyoi/Battleship.git

Open the project in Visual Studio Code

Compile the project (example)

g++ main.cpp -o battleship -lraylib -lopengl32 -lgdi32 -lwinmm

Run the program

<img width="865" height="641" alt="image" src="GameDemo/Screenshot 2026-03-04 at 5.24.30 PM.png" />

Team Members

Liu, Zhuxin

Core game logic implementation

System architecture design

Console-based testing

Project documentation and final report

Chen, Jay

Raylib graphical user interface implementation

UI interaction and gameplay visualization

UI debugging and interface improvements
