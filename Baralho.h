#ifndef BARALHO_H
#define BARALHO_H

#include <vector> // Biblioteca padrão para listas dinâmicas
#include "Carta.h"

class Baralho {
private:
    // A lista dinâmica que armazenará os objetos de Carta internamente
    std::vector<Carta> cartas;

public:
    // Construtor: Inicializa o baralho criando as 52 cartas originais
    Baralho(const sf::Font& fonte);

    // Método para misturar a ordem das cartas na lista
    void embaralhar();

    // Método para retirar e entregar a carta do topo
    Carta distribuirCarta();

    // Método de consulta para saber quantas cartas restam
    int obterTamanho() const;
};

#endif // BARALHO_H