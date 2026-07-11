#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <vector>
#include <optional>
#include "Carta.h"

class GameState {
private:
    std::vector<Carta> cartasNaMesa;
    std::optional<Naipe> naipeLider; // O naipe da primeira carta jogada na rodada

public:
    void adicionarCartaNaMesa(Carta c);
    void limparMesa();
    
    std::optional<Naipe> obterNaipeLider() const;
    std::vector<Carta> obterCartasNaMesa() const;
};

#endif // GAMESTATE_H