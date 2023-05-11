#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <random>
#include <algorithm>


int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Game");
    window.setFramerateLimit(60);

    // Create player sprite
    sf::Texture playerTexture;
    playerTexture.loadFromFile("player.png");
    sf::Sprite playerSprite(playerTexture);
    playerSprite.setPosition(window.getSize().x / 2, window.getSize().y / 2);
    playerSprite.setScale(0.05f, 0.05f);

    // Create enemy sprite vector
    std::vector<sf::Sprite> enemies;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, window.getSize().x);
    sf::Texture enemyTexture;
    enemyTexture.loadFromFile("enemy.png");

    //Create background
    sf::Texture backgroundTexture;
    backgroundTexture.loadFromFile("gibb.jpg");
    sf::Sprite backgroundSprite(backgroundTexture);

    //Create FPS Text
    sf::Font font;
    font.loadFromFile("minecraft.ttf");
    sf::Text text;
    text.setFont(font);
    text.setString("FPS");
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Red);

    //Load Music
    sf::Music music;
    music.openFromFile("livinonaprayer.wav"); //ooooohh, were halfway there 
    music.setVolume(50.0f);
    music.play(); //oooohhh, we livin on a prayer

    //Delta Time Clock
    sf::Clock clock;

    // Game loop
    while (window.isOpen())
    {

        sf::Time elapsed = clock.restart();
        sf::Int64 fps = 1000000/elapsed.asMicroseconds();
        sf::Int64 fps_decimal = 10000000/elapsed.asMicroseconds()%fps;
        text.setString("FPS: " + std::to_string(fps) + "." + std::to_string(fps_decimal));

        // Handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        // Move player sprite based on arrow keys
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && playerSprite.getPosition().x > 0)
        {
            playerSprite.move(-5, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && playerSprite.getPosition().x < window.getSize().x)
        {
            playerSprite.move(5, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && playerSprite.getPosition().y > 0)
        {
            playerSprite.move(0, -5);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && playerSprite.getPosition().y < window.getSize().y)
        {
            playerSprite.move(0, 5);
        }

        // Generate new enemies at random positions
        if (enemies.size() < 5)
        {
            sf::Sprite enemySprite(enemyTexture);
            enemySprite.setScale(0.05f, 0.05f);
            enemySprite.setPosition(dis(gen), -50);
            enemies.push_back(enemySprite);
        }

        // Move enemies down and check for collisions with player
        for (auto& enemy : enemies)
        {
            enemy.move(0, 5);
            if (enemy.getGlobalBounds().intersects(playerSprite.getGlobalBounds()))
            {
                window.close();
            }
        }

        //Kill out of bounds enemies
        for(auto& enemy : enemies){
            if(enemy.getPosition().y > window.getSize().y){
                enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
               [&window](auto& enemy){ return enemy.getPosition().y > window.getSize().y; }), enemies.end());
            }
        }

        // Draw everything
        window.clear();
        window.draw(backgroundSprite);
        window.draw(text);
        window.draw(playerSprite);
        for (auto& enemy : enemies)
        {
            window.draw(enemy);
        }
        window.display();
    }

    return 0;
}