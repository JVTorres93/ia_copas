#include "BotSimple.h"

BotSimple::BotSimple(std::string n) : Jogador(n) {}

// ATUALIZAÇÃO: Adicionado o parâmetro cartasNaMesa aqui também
Carta BotSimple::escolherCarta(std::optional<Naipe> naipeLider, const std::vector<Carta>& cartasNaMesa) {
    int indiceEscolhido = 0; 
    if (!naipeLider.has_value()) {
        for (int i = 0; i < mao.size(); i++) {
            if (mao[i].obterNaipe() == Naipe::Paus && mao[i].obterValor() == Valor::Dois) {
                indiceEscolhido = i; break;
            }
        }
    } else {
        Naipe naipeObrigatorio = naipeLider.value();
        for (int i = 0; i < mao.size(); i++) {
            if (mao[i].obterNaipe() == naipeObrigatorio) {
                indiceEscolhido = i; break; 
            }
        }
    }
    Carta cartaJogada = mao[indiceEscolhido];
    mao.erase(mao.begin() + indiceEscolhido);
    return cartaJogada;
}

std::vector<Carta> BotSimple::escolherCartasParaPassar() {
    std::vector<Carta> passadas;
    for (int k = 0; k < 3; k++) {
        passadas.push_back(mao.back());
        mao.pop_back();
    }
    return passadas;
}