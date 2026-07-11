#include "BotRuleBased.h"

BotRuleBased::BotRuleBased(std::string n) : Jogador(n) {}

std::vector<Carta> BotRuleBased::escolherCartasParaPassar() {
    std::vector<Carta> passadas;
    
    for (int k = 0; k < 3; k++) {
        int indiceEscolhido = -1;

        for (int i = 0; i < mao.size(); i++) {
            if (mao[i].obterNaipe() == Naipe::Espadas && 
               (mao[i].obterValor() == Valor::Dama || mao[i].obterValor() == Valor::Rei || mao[i].obterValor() == Valor::As)) {
                indiceEscolhido = i;
                break; 
            }
        }

        if (indiceEscolhido == -1) {
            int maiorCopas = -1;
            for (int i = 0; i < mao.size(); i++) {
                if (mao[i].obterNaipe() == Naipe::Copas && (int)mao[i].obterValor() > maiorCopas) {
                    maiorCopas = (int)mao[i].obterValor();
                    indiceEscolhido = i;
                }
            }
        }

        if (indiceEscolhido == -1) {
            int maiorGeral = -1;
            for (int i = 0; i < mao.size(); i++) {
                if ((int)mao[i].obterValor() > maiorGeral) {
                    maiorGeral = (int)mao[i].obterValor();
                    indiceEscolhido = i;
                }
            }
        }

        passadas.push_back(mao[indiceEscolhido]);
        mao.erase(mao.begin() + indiceEscolhido);
    }
    
    return passadas;
}

Carta BotRuleBased::escolherCarta(std::optional<Naipe> naipeLider, const std::vector<Carta>& cartasNaMesa) {
    int indiceEscolhido = -1; 

    if (!naipeLider.has_value()) {
        for (int i = 0; i < mao.size(); i++) {
            if (mao[i].obterNaipe() == Naipe::Paus && mao[i].obterValor() == Valor::Dois) {
                indiceEscolhido = i; break;
            }
        }

        if (indiceEscolhido == -1) {
            int menorValor = 999;
            for (int i = 0; i < mao.size(); i++) {
                if (mao[i].obterNaipe() == Naipe::Espadas && mao[i].obterValor() == Valor::Dama) continue;
                
                if ((int)mao[i].obterValor() < menorValor) {
                    menorValor = (int)mao[i].obterValor();
                    indiceEscolhido = i;
                }
            }
            if (indiceEscolhido == -1) indiceEscolhido = 0; 
        }
    } else {
        Naipe naipeObrigatorio = naipeLider.value();
        int menorValor = 999;
        
        for (int i = 0; i < mao.size(); i++) {
            if (mao[i].obterNaipe() == naipeObrigatorio && (int)mao[i].obterValor() < menorValor) {
                menorValor = (int)mao[i].obterValor();
                indiceEscolhido = i;
            }
        }

        if (indiceEscolhido == -1) {
            for (int i = 0; i < mao.size(); i++) {
                if (mao[i].obterNaipe() == Naipe::Espadas && mao[i].obterValor() == Valor::Dama) {
                    indiceEscolhido = i; break;
                }
            }
            if (indiceEscolhido == -1) {
                int maiorCopas = -1;
                for (int i = 0; i < mao.size(); i++) {
                    if (mao[i].obterNaipe() == Naipe::Copas && (int)mao[i].obterValor() > maiorCopas) {
                        maiorCopas = (int)mao[i].obterValor();
                        indiceEscolhido = i;
                    }
                }
            }
            if (indiceEscolhido == -1) {
                int maiorGeral = -1;
                for (int i = 0; i < mao.size(); i++) {
                    if ((int)mao[i].obterValor() > maiorGeral) {
                        maiorGeral = (int)mao[i].obterValor();
                        indiceEscolhido = i;
                    }
                }
            }
        }
    }

    Carta cartaJogada = mao[indiceEscolhido];
    mao.erase(mao.begin() + indiceEscolhido);
    return cartaJogada;
}