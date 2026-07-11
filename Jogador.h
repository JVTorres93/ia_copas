#ifndef JOGADOR_H
#define JOGADOR_H

#include <vector>
#include <string>
#include <optional>
#include "Carta.h"

class Jogador {
protected:
    std::string nome;
    std::vector<Carta> mao;
    
    int pontosDaRodadaAtual;
    int pontosTotais;
    std::vector<int> historicoDePontos;

public:
    Jogador(std::string n);
    virtual ~Jogador() = default;

    std::string obterNome() const;
    void adicionarCarta(Carta c);
    std::vector<Carta>& obterMao();
    
    bool possuiDoisDePaus() const;
    void adicionarPontos(int pontos);
    int obterPontosDaRodadaAtual() const;
    void definirPontosDaRodadaAtual(int pontos);

    void finalizarRodada();
    int obterPontosTotais() const;
    std::vector<int> obterHistoricoDePontos() const;
    void resetarPartida();
    
    void receberCartas(std::vector<Carta> cartasRecebidas);

    // MÉTODOS VIRTUAIS DE INTELIGÊNCIA E MEMÓRIA
    // Agora o bot recebe a mesa para poder analisar as cartas jogadas na vaza atual
    virtual Carta escolherCarta(std::optional<Naipe> naipeLider, const std::vector<Carta>& cartasNaMesa) = 0; 
    virtual std::vector<Carta> escolherCartasParaPassar() = 0;
    
    // Novos métodos para a memória do Monte Carlo (vazios por padrão)
    virtual void observarJogada(int turnoDoJogador, Carta jogada, std::optional<Naipe> naipeLider) {}
    virtual void limparMemoria() {}
};

#endif // JOGADOR_H