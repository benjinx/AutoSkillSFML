#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

class Player
{
public:
    Player();
    ~Player(){};

    void Update();

    void Render(sf::RenderWindow& window);

private:
    sf::CircleShape _shape = sf::CircleShape(50.0f);
};

#endif PLAYER_HPP