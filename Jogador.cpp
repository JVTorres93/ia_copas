#include "Jogador.h"

Jogador::Jogador(std::string n) : nome(n), pontosDaRodadaAtual(0), pontosTotais(0) {}

std::string Jogador::obterNome() const { return nome; }
void Jogador::adicionarCarta(Carta c) { mao.push_back(c); }
std::vector<Carta>& Jogador::obterMao() { return mao; }

bool Jogador::possuiDoisDePaus() const {
    for (const Carta& c : mao) {
        if (c.obterNaipe() == Naipe::Paus && c.obterValor() == Valor::Dois) {
            return true;
        }
    }
    return false;
}

void Jogador::adicionarPontos(int pontos) { pontosDaRodadaAtual += pontos; }
int Jogador::obterPontosDaRodadaAtual() const { return pontosDaRodadaAtual; }
void Jogador::definirPontosDaRodadaAtual(int pontos) { pontosDaRodadaAtual = pontos; }

void Jogador::finalizarRodada() {
    pontosTotais += pontosDaRodadaAtual;
    historicoDePontos.push_back(pontosTotais);
    pontosDaRodadaAtual = 0;
    mao.clear();
}

int Jogador::obterPontosTotais() const { return pontosTotais; }
std::vector<int> Jogador::obterHistoricoDePontos() const { return historicoDePontos; }

void Jogador::resetarPartida() {
    pontosDaRodadaAtual = 0;
    pontosTotais = 0;
    mao.clear();
    historicoDePontos.clear();
    limparMemoria(); 
}


// Implementação: junta as cartas recebidas à mão atual
void Jogador::receberCartas(std::vector<Carta> cartasRecebidas) {
    for (Carta c : cartasRecebidas) {
        mao.push_back(c);
    }
}