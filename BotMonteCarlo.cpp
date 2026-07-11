#include "BotMonteCarlo.h"

BotMonteCarlo::BotMonteCarlo(std::string n) : Jogador(n) {
    limparMemoria();
}

void BotMonteCarlo::limparMemoria() {
    espadasQJogada = false;
    for (int i = 0; i < 4; i++) {
        jogadoresSemNaipe[i][Naipe::Paus] = false;
        jogadoresSemNaipe[i][Naipe::Ouros] = false;
        jogadoresSemNaipe[i][Naipe::Copas] = false;
        jogadoresSemNaipe[i][Naipe::Espadas] = false;
    }
}

void BotMonteCarlo::observarJogada(int turnoDoJogador, Carta jogada, std::optional<Naipe> naipeLider) {
    if (jogada.obterNaipe() == Naipe::Espadas && jogada.obterValor() == Valor::Dama) {
        espadasQJogada = true;
    }
    
    if (naipeLider.has_value() && jogada.obterNaipe() != naipeLider.value()) {
        jogadoresSemNaipe[turnoDoJogador][naipeLider.value()] = true;
    }
}

std::vector<Carta> BotMonteCarlo::escolherCartasParaPassar() {
    std::vector<Carta> copiaMao = mao;
    std::vector<Carta> passadas;

    // Prioridade 1: Q, K, A de Espadas
    for(auto it = copiaMao.begin(); it != copiaMao.end(); ) {
        if(passadas.size() == 3) break;
        if(it->obterNaipe() == Naipe::Espadas && 
          (it->obterValor() == Valor::Dama || it->obterValor() == Valor::Rei || it->obterValor() == Valor::As)) {
            passadas.push_back(*it);
            it = copiaMao.erase(it);
        } else { ++it; }
    }

    // Prioridade 2: Naipes com 3 ou menos cartas
    std::map<Naipe, int> contagemNaipe;
    for(const auto& c : copiaMao) contagemNaipe[c.obterNaipe()]++;
    
    for(auto it = copiaMao.begin(); it != copiaMao.end(); ) {
        if(passadas.size() == 3) break;
        if(contagemNaipe[it->obterNaipe()] <= 3) {
            passadas.push_back(*it);
            contagemNaipe[it->obterNaipe()]--; 
            it = copiaMao.erase(it);
        } else { ++it; }
    }

    // Prioridade 3: Copas mais altas
    while(passadas.size() < 3) {
        auto melhorIt = copiaMao.end();
        int maxVal = -1;
        for(auto it = copiaMao.begin(); it != copiaMao.end(); ++it) {
            if(it->obterNaipe() == Naipe::Copas && (int)it->obterValor() > maxVal) {
                maxVal = (int)it->obterValor();
                melhorIt = it;
            }
        }
        if(melhorIt != copiaMao.end()) {
            passadas.push_back(*melhorIt);
            copiaMao.erase(melhorIt);
        } else break;
    }

    // Prioridade 4: Cartas mais altas no geral
    while(passadas.size() < 3) {
        auto melhorIt = copiaMao.end();
        int maxVal = -1;
        for(auto it = copiaMao.begin(); it != copiaMao.end(); ++it) {
            if((int)it->obterValor() > maxVal) {
                maxVal = (int)it->obterValor();
                melhorIt = it;
            }
        }
        if(melhorIt != copiaMao.end()) {
            passadas.push_back(*melhorIt);
            copiaMao.erase(melhorIt);
        } else break;
    }

    mao = copiaMao;
    return passadas;
}

Carta BotMonteCarlo::escolherCarta(std::optional<Naipe> naipeLider, const std::vector<Carta>& cartasNaMesa) {
    int indiceEscolhido = -1; 

    if (!naipeLider.has_value()) {
        for (int i = 0; i < mao.size(); i++) {
            if (mao[i].obterNaipe() == Naipe::Paus && mao[i].obterValor() == Valor::Dois) {
                indiceEscolhido = i; break;
            }
        }
    }
    if (indiceEscolhido != -1) {
        Carta c = mao[indiceEscolhido];
        mao.erase(mao.begin() + indiceEscolhido);
        return c;
    }

    if (!naipeLider.has_value()) {
        // --- QUANDO É O LÍDER ---
        int minJogadoresSemNaipe = 999;
        Naipe naipeEscolhido = Naipe::Copas; // Segurança
        
        std::map<Naipe, bool> tenhoNaipe;
        for(const auto& c : mao) tenhoNaipe[c.obterNaipe()] = true;

        // 1. Escolher naipe onde a minoria fez jogada cancelada
        Naipe todosOsNaipes[] = {Naipe::Paus, Naipe::Ouros, Naipe::Copas, Naipe::Espadas};
        for(Naipe n : todosOsNaipes) {
            if(tenhoNaipe[n]) {
                int faltas = 0;
                for(int p = 0; p < 4; p++) {
                    if(jogadoresSemNaipe[p][n]) faltas++;
                }
                if(faltas < minJogadoresSemNaipe) {
                    minJogadoresSemNaipe = faltas;
                    naipeEscolhido = n;
                }
            }
        }

        // 2. Jogar a carta mais baixa
        int menorValor = 999;
        for(int i = 0; i < mao.size(); i++) {
            if(mao[i].obterNaipe() == naipeEscolhido && (int)mao[i].obterValor() < menorValor) {
                menorValor = (int)mao[i].obterValor();
                indiceEscolhido = i;
            }
        }

    } else {
        // --- QUANDO NÃO É O LÍDER ---
        Naipe naipeObrigatorio = naipeLider.value();
        bool temNaipe = false;
        for(const auto& c : mao) {
            if(c.obterNaipe() == naipeObrigatorio) { temNaipe = true; break; }
        }

        if (temNaipe) {
            // Tem o naipe: jogar a mais alta possível que seja menor que a maior da mesa
            int maiorMesa = -1;
            for(const auto& c : cartasNaMesa) {
                if(c.obterNaipe() == naipeObrigatorio && (int)c.obterValor() > maiorMesa) {
                    maiorMesa = (int)c.obterValor();
                }
            }

            int melhorValorMenor = -1;
            int indiceMenor = -1;
            int menorAbsoluto = 999;
            int indiceMenorAbsoluto = -1;

            for(int i = 0; i < mao.size(); i++) {
                if(mao[i].obterNaipe() == naipeObrigatorio) {
                    int v = (int)mao[i].obterValor();
                    
                    if(v < menorAbsoluto) { 
                        menorAbsoluto = v;
                        indiceMenorAbsoluto = i;
                    }

                    if(v < maiorMesa && v > melhorValorMenor) {
                        melhorValorMenor = v;
                        indiceMenor = i;
                    }
                }
            }

            if(indiceMenor != -1) indiceEscolhido = indiceMenor; 
            else indiceEscolhido = indiceMenorAbsoluto;

        } else {
            // 1. Jogar Q_espadas
            for(int i = 0; i < mao.size(); i++) {
                if(mao[i].obterNaipe() == Naipe::Espadas && mao[i].obterValor() == Valor::Dama) {
                    indiceEscolhido = i; break;
                }
            }
            // 2. K_espadas ou A_espadas
            if(indiceEscolhido == -1) {
                int maiorEspada = -1;
                for(int i = 0; i < mao.size(); i++) {
                    if(mao[i].obterNaipe() == Naipe::Espadas && (mao[i].obterValor() == Valor::Rei || mao[i].obterValor() == Valor::As)) {
                        if((int)mao[i].obterValor() > maiorEspada) {
                            maiorEspada = (int)mao[i].obterValor();
                            indiceEscolhido = i;
                        }
                    }
                }
            }
            // 3. Copas mais alta
            if(indiceEscolhido == -1) {
                int maiorCopas = -1;
                for(int i = 0; i < mao.size(); i++) {
                    if(mao[i].obterNaipe() == Naipe::Copas && (int)mao[i].obterValor() > maiorCopas) {
                        maiorCopas = (int)mao[i].obterValor();
                        indiceEscolhido = i;
                    }
                }
            }
            // 4. Carta mais alta no geral
            if(indiceEscolhido == -1) {
                int maiorGeral = -1;
                for(int i = 0; i < mao.size(); i++) {
                    if((int)mao[i].obterValor() > maiorGeral) {
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