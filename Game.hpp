#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <memory>
#include "Car.hpp"

class Game {
private:
    sf::RenderWindow window;
    unsigned int width;
    unsigned int height;

    PlayerCar player;
    std::vector<std::unique_ptr<EnemyCar>> enemies;

    // UI Elements
    sf::Font font;
    sf::Text scoreText;
    sf::Text levelText;
    sf::Text speedText;
    sf::Text difficultyText;

    // Standard SFML Audio (No external managers)
    sf::Music bgm;
    sf::SoundBuffer collisionBuffer;
    sf::Sound collisionSound;

    // Game Mechanics
    float spawnTimer;
    float spawnInterval;
    float baseEnemySpeed;
    
    int score;
    bool isGameOver;
    bool isPaused;

    // Road Mechanics
    float roadLeft;
    float roadRight;
    float roadOffset;
    float roadSpeed;

    void processEvents();
    void update(float deltaTime);
    void render();
    void spawnEnemy();
    void resetGame();
    bool checkCollision(const Car& a, const Car& b);

public:
    Game();
    ~Game();
    void run();
};

#endif
