#include "GameState.h"

void GameState::adicionarCartaNaMesa(Carta c) {
    if (cartasNaMesa.empty()) {
        naipeLider = c.obterNaipe(); 
    }
    cartasNaMesa.push_back(c);
}

void GameState::limparMesa() {
    cartasNaMesa.clear();
    naipeLider = std::nullopt;
}

std::optional<Naipe> GameState::obterNaipeLider() const {
    return naipeLider;
}

std::vector<Carta> GameState::obterCartasNaMesa() const {
    return cartasNaMesa;
}