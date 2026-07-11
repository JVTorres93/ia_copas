#ifndef BOT_RULE_BASED_H
#define BOT_RULE_BASED_H

#include "Jogador.h"

class BotRuleBased : public Jogador {
public:
    BotRuleBased(std::string n);
    
    Carta escolherCarta(std::optional<Naipe> naipeLider, const std::vector<Carta>& cartasNaMesa) override;
    std::vector<Carta> escolherCartasParaPassar() override;
};

#endif // BOT_RULE_BASED_H