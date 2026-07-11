#include <SFML/Graphics.hpp>
#include "GameEngine.h"
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Jogo de Copas");

    sf::Font font;
    if (!font.openFromFile("arial.ttf")) {
        std::cout << "Falha ao carregar a fonte!" << std::endl;
        return -1;
    }

    // Inicializa a Engine, que agora controla o menu, os bots e a memória
    GameEngine engine(font);
    
    // Inicia o loop mestre do jogo
    engine.iniciarFluxo(window);

    return 0;
}