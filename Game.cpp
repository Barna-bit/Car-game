#include "Game.hpp"
#include <iostream>
#include <cstdlib>

Game::~Game() {
    bgm.stop();
}

Game::Game() : 
    spawnTimer(0.0f), spawnInterval(1.5f), baseEnemySpeed(300.0f),
    score(0), isGameOver(false), isPaused(false), roadOffset(0.0f), roadSpeed(400.0f) {
    
    // True Fullscreen as requested
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    width = desktop.width;
    height = desktop.height;
    
    // Fallback if WSL returns 0x0
    if (width == 0 || height == 0) {
        width = 1920;
        height = 1080;
    }
    
    window.create(sf::VideoMode(width, height), "Car Dodging Game", sf::Style::Fullscreen);
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false); // WSL FIX: Prevent rapid key toggles from lagging audio

    roadLeft = width * 0.25f;
    roadRight = width * 0.75f;

    // Load Fonts and UI
    if (!font.loadFromFile("assets/font.ttf")) {
        std::cout << "Warning: Could not load font.ttf" << std::endl;
    }

    scoreText.setFont(font);
    scoreText.setCharacterSize(35);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(20, 20);

    levelText.setFont(font);
    levelText.setCharacterSize(35);
    levelText.setFillColor(sf::Color::Yellow);
    levelText.setPosition(20, 70);

    difficultyText.setFont(font);
    difficultyText.setCharacterSize(35);
    difficultyText.setFillColor(sf::Color::Green);
    difficultyText.setPosition(20, 120);

    speedText.setFont(font);
    speedText.setCharacterSize(35);
    speedText.setFillColor(sf::Color::White);
    speedText.setPosition(width - 250, 20);

    player.loadTexture("assets/WhiteCar.png");

    // Standard SFML Audio Setup
    if (bgm.openFromFile("assets/bgm.wav")) {
        bgm.setLoop(false); // Disable native loop to prevent race condition
        bgm.setVolume(40.0f);
        bgm.play();
    }
    
    if (collisionBuffer.loadFromFile("assets/collision.wav")) {
        collisionSound.setBuffer(collisionBuffer);
        collisionSound.setVolume(80.0f);
    }

    resetGame();
}

void Game::resetGame() {
    collisionSound.stop();
    isGameOver = false;
    isPaused = false;
    score = 0;
    spawnTimer = 0.0f;
    spawnInterval = 1.5f;     // Traffic density starts normal
    baseEnemySpeed = 300.0f;  // Speed starts slow
    roadSpeed = 400.0f;
    roadOffset = 0.0f;
    enemies.clear();
    player.setPosition(sf::Vector2f(width / 2.0f, height - 150.0f));
}

void Game::spawnEnemy() {
    // Randomized enemy car speeds as requested in syllabus
    float randomSpeedBoost = static_cast<float>(rand() % 100);
    std::unique_ptr<EnemyCar> enemy(new EnemyCar(baseEnemySpeed + randomSpeedBoost));
    
    const char* enemyFiles[] = {"assets/RedCar1.png", "assets/RedCar2.png", "assets/YellowCar1.png", "assets/YellowCar2.png", "assets/YellowCar3.png"};
    enemy->loadTexture(enemyFiles[rand() % 5]);
    
    // Spawn randomly in different lanes
    int laneWidth = (int)(roadRight - roadLeft - 100);
    if(laneWidth < 1) laneWidth = 1;
    float x = roadLeft + 50 + (rand() % laneWidth);
    
    enemy->setPosition(sf::Vector2f(x, -150.0f));
    enemies.push_back(std::move(enemy));
}

bool Game::checkCollision(const Car& a, const Car& b) {
    // Simple AABB Collision
    return a.getBounds().intersects(b.getBounds());
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window.close();
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) window.close();
            // Pause and resume functionality
            if (event.key.code == sf::Keyboard::P && !isGameOver) {
                isPaused = !isPaused;
            }
            // Restart option after game over
            if (event.key.code == sf::Keyboard::R && isGameOver) {
                resetGame();
            }
        }
    }
}

void Game::update(float dt) {
    // WSL BUG FIX: Manually loop music 0.5s before end to prevent PulseAudio crash
    if (bgm.getStatus() == sf::Music::Playing) {
        if (bgm.getPlayingOffset().asSeconds() >= 23.5f) {
            bgm.setPlayingOffset(sf::seconds(0.f));
        }
    } else if (bgm.getStatus() == sf::Music::Stopped) {
        bgm.play(); // Fallback if it somehow stops
    }

    if (isGameOver || isPaused) return;

    player.update(dt, roadLeft, roadRight);
    
    // Animate road
    roadOffset += roadSpeed * dt;
    if (roadOffset >= 100.0f) roadOffset -= 100.0f;

    // Traffic Density over time (spawnInterval decreases)
    spawnTimer += dt;
    if (spawnTimer >= spawnInterval) {
        spawnEnemy();
        spawnTimer = 0.0f;
        
        // Speed Mechanism: Gradually increase game speed over time
        if (spawnInterval > 0.6f) spawnInterval -= 0.02f; 
        baseEnemySpeed += 5.0f;
        roadSpeed += 5.0f;
    }

    for (auto it = enemies.begin(); it != enemies.end(); ) {
        (*it)->update(dt);
        
        // Remove cars once they go off-screen
        if (!(*it)->isActive()) {
            it = enemies.erase(it);
            // Increase score based on number of dodged cars
            score += 1;
        } else {
            // Detect collision between player car and enemy cars
            if (checkCollision(player, **it)) {
                isGameOver = true;
                if (collisionSound.getStatus() != sf::Sound::Playing) {
                    collisionSound.play();
                }
                break; // Break loop to prevent multiple collisions triggering simultaneously
            }
            ++it;
        }
    }

    // Determine Difficulty Level based on score/speed
    std::string difficulty = "Easy";
    difficultyText.setFillColor(sf::Color::Green);
    if (score > 15) {
        difficulty = "Medium";
        difficultyText.setFillColor(sf::Color::Yellow);
    }
    if (score > 40) {
        difficulty = "Hard";
        difficultyText.setFillColor(sf::Color::Red);
    }

    int level = (score / 10) + 1;
    scoreText.setString("SCORE: " + std::to_string(score));
    speedText.setString("SPEED: " + std::to_string(level));
}

void Game::render() {
    window.clear(sf::Color(30, 30, 30)); // Dark Gray background as originally requested
    
    // Render road
    sf::RectangleShape road(sf::Vector2f(roadRight - roadLeft, (float)height));
    road.setPosition(roadLeft, 0.f);
    road.setFillColor(sf::Color(50, 50, 50));
    window.draw(road);

    // Render lane markings
    for (float y = -100.0f + roadOffset; y < height; y += 100.0f) {
        sf::RectangleShape line(sf::Vector2f(10.0f, 60.0f));
        line.setPosition((roadLeft + roadRight)/2.0f - 5.0f, y);
        line.setFillColor(sf::Color::White);
        window.draw(line);
    }

    // Draw Cars
    player.draw(window);
    for (size_t i = 0; i < enemies.size(); ++i) {
        enemies[i]->draw(window);
    }
    
    // Draw UI
    window.draw(scoreText);
    window.draw(speedText);

    if (isGameOver) {
        sf::RectangleShape overlay(sf::Vector2f((float)width, (float)height));
        overlay.setFillColor(sf::Color(0, 0, 0, 180));
        window.draw(overlay);

        sf::Text go("GAME OVER\nPress R to Restart", font, 80);
        go.setFillColor(sf::Color::Red);
        sf::FloatRect b = go.getLocalBounds();
        go.setOrigin(b.width/2, b.height/2);
        go.setPosition(width/2.0f, height/2.0f);
        window.draw(go);
    }
    
    if (isPaused && !isGameOver) {
        sf::Text paused("PAUSED", font, 100);
        paused.setFillColor(sf::Color::Yellow);
        sf::FloatRect b = paused.getLocalBounds();
        paused.setOrigin(b.width/2, b.height/2);
        paused.setPosition(width/2.0f, height/2.0f);
        window.draw(paused);
    }

    window.display();
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        processEvents();
        update(clock.restart().asSeconds());
        render();
    }
}
