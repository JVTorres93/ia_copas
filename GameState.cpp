#include "GameState.h"

void GameState::adicionarCartaNaMesa(Carta c) {
    // Se a mesa estiver vazia, esta carta define o naipe obrigatório
    if (cartasNaMesa.empty()) {
        naipeLider = c.obterNaipe(); 
    }
    cartasNaMesa.push_back(c);
}

void GameState::limparMesa() {
    cartasNaMesa.clear();
    naipeLider = std::nullopt; // Remove a obrigatoriedade de naipe
}

std::optional<Naipe> GameState::obterNaipeLider() const {
    return naipeLider;
}

std::vector<Carta> GameState::obterCartasNaMesa() const {
    return cartasNaMesa;
}