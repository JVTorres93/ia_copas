#include "Carta.h"

Carta::Carta(Valor v, Naipe n, const sf::Font& fonte) : valor(v), naipe(n), estaViradaParaCima(true), texto(fonte) {
    forma.setSize(sf::Vector2f({80.f, 120.f})); 
    forma.setFillColor(sf::Color::White);
    forma.setOutlineColor(sf::Color::Black);
    forma.setOutlineThickness(2.f);
}

Naipe Carta::obterNaipe() const { return naipe; }
Valor Carta::obterValor() const { return valor; }

int Carta::obterPontos() const {
    if (naipe == Naipe::Copas) return 1;
    if (naipe == Naipe::Espadas && valor == Valor::Dama) return 13;
    return 0;
}

void Carta::definirPosicao(float x, float y) {
    forma.setPosition(sf::Vector2f({x, y}));
    texto.setPosition(sf::Vector2f({x + 5.f, y + 5.f})); 
}

void Carta::definirViradaParaCima(bool viradaParaCima) {
    estaViradaParaCima = viradaParaCima;
}

void Carta::desenhar(sf::RenderWindow& janela) {
    if (!estaViradaParaCima) {
        forma.setFillColor(sf::Color(180, 0, 0));
        janela.draw(forma);
        return;
    }

    forma.setFillColor(sf::Color::White);
    janela.draw(forma);

    texto.setCharacterSize(16);

    if (naipe == Naipe::Copas || naipe == Naipe::Ouros) {
        texto.setFillColor(sf::Color::Red);
    } else {
        texto.setFillColor(sf::Color::Black);
    }

    std::string textoValor;
    switch (valor) {
        case Valor::Valete: textoValor = "J"; break;
        case Valor::Dama:   textoValor = "Q"; break;
        case Valor::Rei:    textoValor = "K"; break;
        case Valor::As:     textoValor = "A"; break;
        default:            textoValor = std::to_string(static_cast<int>(valor)); break;
    }

    std::string textoNaipe;
    switch (naipe) {
        case Naipe::Paus:    textoNaipe = "Paus"; break;
        case Naipe::Ouros:   textoNaipe = "Ouros"; break;
        case Naipe::Espadas: textoNaipe = "Espadas"; break;
        case Naipe::Copas:   textoNaipe = "Copas"; break;
    }

    texto.setString(textoValor + "\n" + textoNaipe);
    janela.draw(texto);
}