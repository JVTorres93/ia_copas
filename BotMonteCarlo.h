#ifndef BOT_MONTE_CARLO_H
#define BOT_MONTE_CARLO_H

#include "Jogador.h"
#include <map>

class BotMonteCarlo : public Jogador {
private:
    bool espadasQJogada;
    // Mapeia o jogador (0 a 3) e o Naipe. Se for true, significa que o jogador NÃO tem o naipe (jogada cancelada)
    std::map<Naipe, bool> jogadoresSemNaipe[4]; 

public:
    BotMonteCarlo(std::string n);
    
    Carta escolherCarta(std::optional<Naipe> naipeLider, const std::vector<Carta>& cartasNaMesa) override;
    std::vector<Carta> escolherCartasParaPassar() override;
    
    // Funções de memória
    void observarJogada(int turnoDoJogador, Carta jogada, std::optional<Naipe> naipeLider) override;
    void limparMemoria() override;
};

#endif // BOT_MONTE_CARLO_H