#ifndef BOT_SIMPLE_H
#define BOT_SIMPLE_H

#include "Jogador.h"

class BotSimple : public Jogador {
public:
    BotSimple(std::string n);
    
    Carta escolherCarta(std::optional<Naipe> naipeLider, const std::vector<Carta>& cartasNaMesa) override;
    std::vector<Carta> escolherCartasParaPassar() override; 
};

#endif // BOT_SIMPLE_H