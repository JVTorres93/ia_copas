#ifndef CARTA_H
#define CARTA_H

#include <SFML/Graphics.hpp>
#include <string>

enum class Naipe {
    Paus,
    Ouros,
    Espadas,
    Copas
};

enum class Valor {
    Dois = 2, Tres, Quatro, Cinco, Seis, Sete, Oito, Nove, Dez,
    Valete, Dama, Rei, As
};

class Carta {
private:
    Naipe naipe;
    Valor valor;
    bool estaViradaParaCima;

    sf::RectangleShape forma;
    sf::Text texto; // No SFML 3, isso exige uma fonte ao ser criado

public:
    // ATUALIZAÇÃO: O construtor agora recebe a fonte tipográfica
    Carta(Valor v, Naipe n, const sf::Font& fonte);

    Naipe obterNaipe() const;
    Valor obterValor() const;
    int obterPontos() const; 

    void definirPosicao(float x, float y);
    void definirViradaParaCima(bool viradaParaCima);
    
    // ATUALIZAÇÃO: Não precisamos mais passar a fonte aqui, pois a carta já a possui
    void desenhar(sf::RenderWindow& janela);
};

#endif // CARTA_H