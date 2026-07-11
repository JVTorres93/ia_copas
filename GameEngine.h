#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "Baralho.h"
#include "Jogador.h"
#include "GameState.h"
#include "Button.h"
#include "CheckBox.h"

// Enumerador para os modelos de Inteligência Artificial
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

    // Guarda as configurações de modelos escolhidas para os 4 robôs
    TipoDeBot modelosEscolhidos[4];

    // --- MÉTODOS DE INICIALIZAÇÃO E MEMÓRIA (Uso interno) ---
    void prepararJogadores();
    void limparJogadoresDaMemoria();
    void distribuirCartas();

    // --- MÉTODOS DE ANDAMENTO (Uso interno) ---
    void jogarPartida(sf::RenderWindow& window);
    bool atingiuLimiteDePontos() const;

    // --- MÉTODOS DE INTERFACE (Uso interno) ---
    void desenharInterface(sf::RenderWindow& window);
    int mostrarMenuInicial(sf::RenderWindow& window);
    void mostrarConfiguracaoBots(sf::RenderWindow& window); // Nova tela
    void mostrarPlacar(sf::RenderWindow& window, bool ehPlacarFinal);

public:
    GameEngine(const sf::Font& f);
    ~GameEngine(); // Destrutor adicionado para limpar a memória ao fechar o jogo

    void adicionarJogador(Jogador* j);
    void iniciarFluxo(sf::RenderWindow& window); 
};

#endif // GAMEENGINE_H