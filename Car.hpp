#ifndef CAR_HPP
#define CAR_HPP

#include <SFML/Graphics.hpp>
#include <string>

// Base Class demonstrating OOP concepts (Inheritance, Abstraction)
class Car {
protected:
    sf::Sprite sprite;
    sf::Texture texture;
    bool active;

public:
    Car();
    virtual ~Car() {}

    bool loadTexture(const std::string& path);
    void setPosition(sf::Vector2f pos);
    void draw(sf::RenderWindow& window);
    
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
    
    bool isActive() const { return active; }
    void setActive(bool val) { active = val; }
};

// Derived Class for the Player
class PlayerCar : public Car {
public:
    // Controls the player's left and right movement within road boundaries
    void update(float deltaTime, float leftBound, float rightBound);
};

// Derived Class for the Enemies
class EnemyCar : public Car {
private:
    float speed;
public:
    EnemyCar(float s);
    // Controls the downward movement of the enemy car
    void update(float deltaTime);
};

#endif
