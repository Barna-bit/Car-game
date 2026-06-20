#include "Car.hpp"

// Base Car Class Methods
Car::Car() : active(true) {}

bool Car::loadTexture(const std::string& path) {
    if (texture.loadFromFile(path)) {
        sprite.setTexture(texture);
        // Center the origin for easier positioning and collision
        sf::FloatRect b = sprite.getLocalBounds();
        sprite.setOrigin(b.width / 2.0f, b.height / 2.0f);
        return true;
    }
    return false;
}

void Car::setPosition(sf::Vector2f pos) {
    sprite.setPosition(pos);
}

void Car::draw(sf::RenderWindow& window) {
    if (active) {
        window.draw(sprite);
    }
}

sf::FloatRect Car::getBounds() const {
    return sprite.getGlobalBounds();
}

sf::Vector2f Car::getPosition() const {
    return sprite.getPosition();
}


// PlayerCar Class Methods
void PlayerCar::update(float deltaTime, float leftBound, float rightBound) {
    float moveSpeed = 600.0f;
    sf::Vector2f pos = sprite.getPosition();

    // Move left and right using keyboard input
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        pos.x -= moveSpeed * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        pos.x += moveSpeed * deltaTime;
    }

    // Restrict movement within road boundaries
    if (pos.x < leftBound + 40) pos.x = leftBound + 40;
    if (pos.x > rightBound - 40) pos.x = rightBound - 40;

    sprite.setPosition(pos);
}


// EnemyCar Class Methods
EnemyCar::EnemyCar(float s) : speed(s) {
    sprite.setRotation(180.f);
}

void EnemyCar::update(float deltaTime) {
    // Move downward
    sprite.move(0.f, speed * deltaTime);
    
    // Remove cars once they go off-screen
    if (sprite.getPosition().y > 1500.f) {
        active = false;
    }
}
