#ifndef CHECKBOX_H
#define CHECKBOX_H

#include <SFML/Graphics.hpp>
#include <string>

class CheckBox {
private:
    sf::RectangleShape boxOuter;
    sf::RectangleShape boxInner;
    sf::Text text;
    bool isChecked;

public:
    CheckBox(std::string rotulo, const sf::Font& font, float x, float y);
    
    void draw(sf::RenderWindow& window);
    bool isClicked(sf::Vector2f mousePos) const;
    
    void setChecked(bool checked);
    bool getChecked() const;
};

#endif // CHECKBOX_H