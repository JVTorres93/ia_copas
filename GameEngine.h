#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "Baralho.h"
#include "Jogador.h"
#include "GameState.h"
#include "Button.h"
#include "CheckBox.h"

enum class TipoDeBot {
    Simples,
    RuleBased,
    MonteCarlo
};

class GameEngine {
private:
    GameState estado;
    std::vector<Jogador*> jogadores;
    sf::Font fonte;

    TipoDeBot modelosEscolhidos[4];

    // --- MÉTODOS DE INICIALIZAÇÃO E MEMÓRIA (Uso interno) ---
    void prepararJogadores();
    void limparJogadoresDaMemoria();
    void distribuirCartas();

// --- MÉTODOS DE ANDAMENTO (Uso interno) ---
    void jogarPartida(sf::RenderWindow& window, bool headless = false); // Atualizado
    bool atingiuLimiteDePontos() const;

    // --- MÉTODOS DE INTERFACE (Uso interno) ---
    void desenharInterface(sf::RenderWindow& window);
    int mostrarMenuInicial(sf::RenderWindow& window);
    void mostrarConfiguracaoBots(sf::RenderWindow& window);
    void mostrarPlacar(sf::RenderWindow& window, bool ehPlacarFinal);
    void rodarCampeonato(sf::RenderWindow& window); // Novo
    void mostrarGraficosCampeonato(sf::RenderWindow& window, const std::vector<int>& pc, const std::vector<int>& pp); // Novo

public:
    GameEngine(const sf::Font& f);
    ~GameEngine();
    
    void adicionarJogador(Jogador* j);
    void iniciarFluxo(sf::RenderWindow& window); 
};

#endif // GAMEENGINE_H