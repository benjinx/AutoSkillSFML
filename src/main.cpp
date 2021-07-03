#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <Player.hpp>

int main(int argc, char ** argv)
{
    // Create window
    sf::RenderWindow window(sf::VideoMode(800, 600), "My Window");

    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        return 1;
    }

    sf::Text text;

    text.setFont(font);

    text.setString("Hello world");

    text.setCharacterSize(16);

    text.setFillColor(sf::Color::Red);

    text.setStyle(sf::Text::Bold | sf::Text::Underlined);

    text.setPosition(100, 100);

    sf::RectangleShape rectangle(sf::Vector2f(100.0f, 50.0f));
    rectangle.setFillColor(sf::Color(150, 50, 250));
    rectangle.setPosition(100, 100);
    
    Player player;

    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // close window exit
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        // Update player
        player.Update();

        // Clear screen
        window.clear();

        // printf("Hello world\n");
        // fflush(stdout);

        // Render player
        player.Render(window);

        window.draw(rectangle);

        // Text
        window.draw(text);

        // Update the window
        window.display();
    }

    return 0;
}