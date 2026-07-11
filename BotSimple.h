#ifndef BOT_SIMPLE_H
#define BOT_SIMPLE_H

#include "Jogador.h"

// Classe que implementa a lógica mais básica do jogo
class BotSimple : public Jogador {
public:
    BotSimple(std::string n);
    
    // ATUALIZAÇÃO: Assinatura agora inclui const std::vector<Carta>& cartasNaMesa
    Carta escolherCarta(std::optional<Naipe> naipeLider, const std::vector<Carta>& cartasNaMesa) override;
    std::vector<Carta> escolherCartasParaPassar() override; 
};

#endif // BOT_SIMPLE_H