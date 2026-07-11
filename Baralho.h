#ifndef BARALHO_H
#define BARALHO_H

#include <vector>
#include "Carta.h"

class Baralho {
private:
    std::vector<Carta> cartas;

public:
    Baralho(const sf::Font& fonte);

    void embaralhar();

    Carta distribuirCarta();

    int obterTamanho() const;
};

#endif // BARALHO_H