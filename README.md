# Car-game
A 2D Car Dodging Game built with C++ and SFML featuring enemy AI, collision detection, scoring system, progressive difficulty, audio effects, and object-oriented game architecture.
# 🚗 Car Dodging Game

A fast-paced **2D Car Dodging Game** developed using **C++ and SFML**, where players control a car to avoid incoming enemy vehicles, survive as long as possible, and achieve the highest score.

## 🎮 Features

- 🚘 Smooth player car movement
- 🚗 Dynamic enemy car spawning
- 💥 Collision detection and Game Over system
- 📈 Real-time score tracking
- ⚡ Progressive difficulty and increasing enemy speed
- 🔊 Background music and collision sound effects
- 🖥️ Fullscreen gameplay
- 📝 User interface displaying score and game information
- 🎯 Frame-rate independent movement using delta time

## 🛠️ Technologies Used

- C++
- SFML (Simple and Fast Multimedia Library)
- Object-Oriented Programming (OOP)
- STL Containers (`vector`, `unique_ptr`)

## 📂 Project Structure

```
Car-game/
├── Car.cpp
├── Car.hpp
├── Game.cpp
├── Game.hpp
├── main.cpp
├── assets/
│   ├── fonts/
│   ├── textures/
│   ├── music/
│   └── sounds/
└── README.md
```

## 🎯 Game Objective

Control your car and avoid colliding with enemy vehicles. The longer you survive, the higher your score. As the game progresses, enemy cars become faster, making the game more challenging.

## 🚀 How to Run

### Prerequisites

- Install SFML on your system.
- Configure your C++ compiler to link with SFML libraries.

### Clone the Repository

```bash
git clone https://github.com/Barna-bit/Car-game.git
cd Car-game
```

### Build and Run

Compile the project with SFML libraries and run the generated executable.

Example (g++):

```bash
g++ main.cpp Game.cpp Car.cpp -o CarGame ^
-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

./CarGame
```

## 📚 Concepts Used

- Inheritance
- Abstraction
- Polymorphism
- Smart Pointers
- Collision Detection
- Event Handling
- Game Loop Architecture
- Audio Integration

## 📸 Screenshots

Add gameplay screenshots here after running the game.

## 🔮 Future Improvements

- Multiple difficulty levels
- Pause and Resume functionality
- High score saving system
- Different car skins
- Power-ups and bonus items
- Main menu and settings screen

## 👨‍💻 Author

**Adityabarna Das**

B.Tech CSE Student | Aspiring Software Developer | Data Analytics Enthusiast

---

⭐ If you like this project, don't forget to star this repository!
