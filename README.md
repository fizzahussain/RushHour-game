# RushHour-game    – 2D C++ OOP Game Project

Welcome to **Rush Hour**, a console-based 2D simulation game where every keypress is a survival tactic. Designed and developed in **C++** using core **Object-Oriented Programming (OOP)** concepts, this was my semester project for *CS1004 – Object-Oriented Programming* at FAST NUCES (SP25).

---

## 🎮 Game Overview

Take control as a **Taxi Driver** or a **Delivery Driver** and navigate a living, moving city grid. Earn money, complete tasks, avoid chaos—and don’t run out of fuel!

Your vehicle starts with a limited tank, but you decide how much to refuel at stations based on your earnings. Complete two tasks, and the streets get more crowded with faster moving cars.

Oh, and yes—your fuel tank? It's custom-drawn and **visually changes color** as your fuel depletes. Fuel panic has never looked this artistic.

---

## 🚗 Game Modes

### 🚖 Taxi Driver
- Pick up passengers and drop them at a green-highlighted destination.
- Earn points + fare.
- Lose points if you hit cars, obstacles, or (oops) pedestrians.

### 📦 Delivery Driver
- Grab packages from pickup points.
- Deliver to random drop-off zones.
- Higher rewards, but also higher penalties.

🌀 **Switch roles mid-game** from a designated station using `P`. The board updates to show your new tasks.

---

## 🌆 Game Features

- **Dynamic 30x30 Grid**: With buildings, roads, fuel stations, and packages.
- **Free-moving Cars**: Cars move randomly across the board, increasing the challenge.
- **Color-Changing Fuel Tank**: A self-drawn visual fuel indicator updates as fuel depletes.
- **Wallet-Based Refueling**: Choose how much fuel to buy based on your current funds.
- **Live HUD**:
  - Current fuel level
  - Wallet balance
  - Task status (passenger/package onboard)
  - Score earned after each task
- **Smart Scaling Difficulty**: Every 2 completed tasks = more cars + faster speeds.
- Persistent **Leaderboard** using binary file handling (`highscores.txt`).

---

## 🧠 OOP Concepts in Action

- **Encapsulation**: Clean modular class structure.
- **Inheritance**: Base vehicle class extended into Taxi/Delivery subclasses.
- **Polymorphism**: Handles display and collisions for different object types.
- **Composition & Association**: Rich relationships between map, player, and objects.
- **File I/O (Binary)**: Leaderboard save/load via persistent file storage.

---

## 📦 Project Structure

- `main.cpp` – Game logic and engine
- `classes/` – Class definitions (Player, Vehicle, Board, etc.)
- `graphics.cpp` – For rendering game visuals
- `highscores.txt` – Stores top 10 scores
- `UML_Diagram.jpg` – Class relationships overview
- `README.md` – You're reading it!

---

## 🏁 Winning and End Conditions

- Game runs for 3 minutes.
- Win by scoring **100+ points**.
- Game ends if:
  - Time runs out
  - Fuel runs out
  - Score drops below zero

---

## 💻 How to Run

1. Compile using `g++` and the graphics library.
2. Ensure full-screen terminal for best experience.
3. Controls:
   - `Arrow keys`: Movement
   - `Spacebar`: Pick up passenger/package
   - `P`: Change role mid-game

In terminal run:
make
./game
