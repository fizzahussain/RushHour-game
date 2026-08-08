<div align="center">

<h1>🚕 Rush Hour</h1>

<h3>OOP-driven C++ arcade game with dynamic traffic, two playable roles, fuel management, and persistent scores</h3>

<p>
  <img src="https://img.shields.io/badge/C%2B%2B-OOP-00599C?logo=cplusplus&logoColor=white" alt="C++" />
  <img src="https://img.shields.io/badge/OpenGL-Graphics-5586A4?logo=opengl&logoColor=white" alt="OpenGL" />
  <img src="https://img.shields.io/badge/SDL2-Audio-5C93C4?logo=sdl&logoColor=white" alt="SDL2" />
  <img src="https://img.shields.io/badge/Platform-Linux-lightgrey" alt="Platform" />
  <img src="https://img.shields.io/badge/Project-OOP%20Coursework-success" alt="Project" />
</p>

<p>
Navigate a procedurally arranged city as a <strong>Taxi Driver</strong> or <strong>Delivery Driver</strong>, complete jobs, manage fuel and money, avoid traffic, and build a high score before time runs out.
</p>

</div>

🎮 Gameplay

Rush Hour is a graphical driving game played on a 26 × 26 tile city grid. Roads, buildings, obstacles, fuel stations, passengers, packages, and moving cars are generated across the board to create a different traffic layout each run.

You can start as either a Taxi Driver or Delivery Driver and complete role-specific jobs for points and fare money.

🚖 Taxi Driver Mode

Pick up passengers from road tiles

Drive them to their highlighted destination

Earn 10 score points for each successful drop-off

Receive a randomly generated fare in your wallet

Lose fewer points than the Delivery Driver when colliding with traffic or obstacles

📦 Delivery Driver Mode

Pick up packages placed near buildings

Deliver them to their destination

Earn 20 score points for each completed delivery

Receive a randomly generated fare in your wallet

Take larger collision penalties in exchange for the higher job reward

🔄 Game Flow

flowchart LR
    A[Choose role] --> B[Drive through city]
    B --> C[Pick up passenger or package]
    C --> D[Reach destination]
    D --> E[Earn score + fare]
    E --> F{Every 2 jobs?}
    F -- Yes --> G[Add traffic + increase car speed]
    F -- No --> H[Continue]
    G --> H
    H --> I{Time / fuel / score ended?}
    I -- No --> B
    I -- Yes --> J[Leaderboard + result]

🎮 Controls

Input

Action

↑ ↓ ← →

Move the vehicle one road tile

Space / S

Pick up or drop off a passenger/package

R

Refuel when positioned at a fuel station

P

Request a role switch while at the board origin

Enter

Confirm menu selections, name entry, refueling, or role switch

W / S

Navigate the theme and main menus

B

Return from the leaderboard

Space

Restart after game over

🧠 How It Works

A look at the main systems behind the game: the object model, city generation, traffic, fuel, and score persistence.

The project is centered around an object-oriented hierarchy of game entities. A common abstract base class stores position and defines the drawing interface, while specialized classes implement their own behavior.

🧱 Object Model

classDiagram
    class itemsonboard {
        -int x
        -int y
        +draw()*
        +update(double)
    }

    class Obstacle
    class BoxObstacle
    class TreeObstacle
    class OtherCar
    class Passenger
    class Package
    class FuelStation
    class Player
    class TaxiDriver
    class DeliveryDriver

    itemsonboard <|-- Obstacle
    Obstacle <|-- BoxObstacle
    Obstacle <|-- TreeObstacle
    itemsonboard <|-- OtherCar
    itemsonboard <|-- Passenger
    itemsonboard <|-- Package
    itemsonboard <|-- FuelStation
    itemsonboard <|-- Player
    Player <|-- TaxiDriver
    Player <|-- DeliveryDriver

The Board class manages the active player and collections of cars, passengers, packages, fuel stations, and obstacles. The Game class manages higher-level state such as menus, timing, themes, refueling, the leaderboard, and game-over logic.

🗺️ City Generation & Reachability

The board reserves regular road lines and fills part of the remaining grid with buildings. Game entities are then placed on valid road positions.

Before some passenger/package positions are accepted, the game runs a depth-first search (DFS) over the road grid to make sure the location is reachable rather than placing a task in an isolated part of the map.

🚗 Traffic & Difficulty Scaling

The game begins with moving traffic cars that update independently from the player. Cars follow valid road tiles and change direction when their current route becomes blocked.

Every two completed jobs:

another traffic car can be added, up to the configured maximum

existing traffic cars have their movement speed increased

This makes the city progressively harder as the player succeeds.

⛽ Fuel & Wallet System

The player starts with 500 fuel units, and moving one tile consumes one unit.

Fuel stations are placed around the board. When the player reaches one, pressing R opens a refueling prompt where they choose how much fuel to purchase. The amount is limited by both:

the remaining capacity of the fuel tank

the amount of money currently available in the wallet

The HUD shows the current fuel numerically and through a visual tank that changes from green to orange and red as fuel falls.

🏆 Leaderboard & Persistence

The game stores up to 10 leaderboard entries in highscores.txt using binary file operations. New scores are inserted into descending order before the leaderboard is saved again.

🏁 Scoring & Game Rules

A game lasts up to 180 seconds (3 minutes).

Event

Taxi Driver

Delivery Driver

Successful job

+10

+20

Hit an obstacle

-2

-4

Hit another car

-3

-5

The session ends when any of these conditions is reached:

the 3-minute timer expires

fuel reaches zero

score falls below zero

A final score of 100 or more is treated as a win on the game-over screen.

📊 Algorithmic Complexity

The game uses bounded collections for most entities, while DFS-based reachability is the main grid-search operation.

Most entity collections have small fixed maximum sizes, so their scans are effectively inexpensive during normal gameplay. The grid traversal used for reachability is the main algorithmic operation worth calling out.

Operation

Approx. Complexity

Move validation

O(O + C)

DFS reachability check

O(R × C)

Maintain passenger/package availability

O(P + K + F + O) per maintenance pass

Insert a leaderboard score

O(H)

Where:

R × C — board dimensions

O — number of obstacles

C — number of traffic cars

P — number of passengers

K — number of packages

F — number of fuel stations

H — leaderboard size, capped at 10

Because these counts are bounded in the current implementation, most gameplay checks behave like constant-time operations in practice.

🛠️ Tech Stack

Technology

Role in the Project

C++

Core game logic, OOP model, state management, and file handling

OpenGL / FreeGLUT

2D rendering, game window, keyboard input, and timer callbacks

SDL2 + SDL2_mixer

Background music and gameplay sound effects

FreeImage / CImg

Graphics and image utility support

Make

Build configuration

🚀 Getting Started

Build and run the game locally on Linux/Ubuntu using the existing Makefile.

The current build configuration targets Linux / Ubuntu.

Step 1 — Clone the Repository

git clone https://github.com/fizzahussain/RushHour-game.git
cd RushHour-game

Step 2 — Install Dependencies

On Ubuntu/Debian:

sudo apt update
sudo apt install \
  build-essential \
  freeglut3-dev \
  libfreeimage-dev \
  libsdl2-dev \
  libsdl2-mixer-dev \
  libgl1-mesa-dev \
  libglu1-mesa-dev \
  libx11-dev

[!NOTE]The repository also includes its original install-libraries.sh helper. The command above lists the libraries referenced by the current Makefile, including SDL2 and SDL2_mixer.

Step 3 — Build the Game

make

Step 4 — Run the Game

./game

To remove generated object files and the executable:

make clean

Keep the .mp3 audio files in the project directory when running the game.

📁 Project Structure

RushHour-game/
├── game.cpp               # Game entities, board logic, menus and game state
├── util.cpp               # Drawing and graphics helper functions
├── util.h                 # Utility declarations and colour definitions
├── CImg.h                 # Bundled third-party image header
├── Makefile               # Build configuration
├── install-libraries.sh   # Original dependency helper
├── music.mp3              # Background music
├── pickup.mp3             # Pickup sound effect
├── dropoff.mp3            # Drop-off sound effect
├── collision.mp3          # Collision sound effect
├── gameover.mp3           # Game-over sound effect
└── docs/
    └── TECHNICAL.md       # Deeper implementation notes

highscores.txt is created/updated at runtime when leaderboard scores are saved.

🔍 Technical Documentation

For a deeper walkthrough of the class hierarchy, board generation, DFS reachability, traffic updates, role switching, fuel system, and leaderboard persistence, see docs/TECHNICAL.md.

📌 Project Background

Rush Hour was developed as an Object-Oriented Programming course project. It combines inheritance, abstraction, polymorphism, composition, dynamic allocation, graphical event handling, collision checks, file persistence, audio, and game-state management in one playable C++ application.