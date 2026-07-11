#include "CheckBox.h"

CheckBox::CheckBox(std::string rotulo, const sf::Font& font, float x, float y) : text(font), isChecked(false) {
    boxOuter.setSize(sf::Vector2f({20.f, 20.f}));
    boxOuter.setPosition(sf::Vector2f({x, y}));
    boxOuter.setFillColor(sf::Color::Black);
    boxOuter.setOutlineColor(sf::Color::White);
    boxOuter.setOutlineThickness(2.f);

    boxInner.setSize(sf::Vector2f({12.f, 12.f}));
    boxInner.setPosition(sf::Vector2f({x + 4.f, y + 4.f}));
    boxInner.setFillColor(sf::Color::Yellow);

    text.setString(rotulo);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f({x + 30.f, y}));
}

void CheckBox::draw(sf::RenderWindow& window) {
    window.draw(boxOuter);
    if (isChecked) {
        window.draw(boxInner);
    }
    window.draw(text);
}

bool CheckBox::isClicked(sf::Vector2f mousePos) const {
    return boxOuter.getGlobalBounds().contains(mousePos);
}

void CheckBox::setChecked(bool checked) { isChecked = checked; }
bool CheckBox::getChecked() const { return isChecked; }