#include "Button.h"

Button::Button(std::string textoBotao, const sf::Font& font, float x, float y, float largura, float altura) : text(font) {
    shape.setSize(sf::Vector2f({largura, altura}));
    shape.setPosition(sf::Vector2f({x, y}));
    shape.setFillColor(sf::Color(70, 130, 180));
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(2.f);

    text.setString(textoBotao);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    
    text.setPosition(sf::Vector2f({x + largura / 6.f, y + altura / 3.f}));
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(text);
}

bool Button::isClicked(sf::Vector2f mousePosition) const {
    return shape.getGlobalBounds().contains(mousePosition);
}