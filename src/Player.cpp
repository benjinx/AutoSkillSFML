#include <Player.hpp>

Player::Player()
{
    _shape.setFillColor(sf::Color::Cyan);
}

void Player::Update()
{
    float speed = 0.1f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        _shape.move(-1.0f * speed, 0.0f);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        _shape.move(1.0f * speed, 0.0f);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        _shape.move(0.0f, -1.0f * speed);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        _shape.move(0.0f, 1.0f * speed);
    }
}

void Player::Render(sf::RenderWindow& window)
{
    window.draw(_shape);
}