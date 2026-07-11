#include "Button.h"

// CORREÇÃO: Adicionamos ': text(font)' para criar o texto já com a fonte (Exigência do SFML 3.1.0)
Button::Button(std::string textoBotao, const sf::Font& font, float x, float y, float largura, float altura) : text(font) {
    shape.setSize(sf::Vector2f({largura, altura}));
    shape.setPosition(sf::Vector2f({x, y}));
    shape.setFillColor(sf::Color(70, 130, 180)); // Cor Azul Aço
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(2.f);

    text.setString(textoBotao);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    
    // Centraliza o texto de forma aproximada dentro do botão
    text.setPosition(sf::Vector2f({x + largura / 6.f, y + altura / 3.f}));
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(text);
}

bool Button::isClicked(sf::Vector2f mousePosition) const {
    return shape.getGlobalBounds().contains(mousePosition);
}