#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <string>

class Button {
private:
    sf::RectangleShape shape;
    sf::Text text;

public:
    Button(std::string textoBotao, const sf::Font& font, float x, float y, float largura, float altura);
    
    void draw(sf::RenderWindow& window);
    bool isClicked(sf::Vector2f mousePosition) const;
};

#endif // BUTTON_H