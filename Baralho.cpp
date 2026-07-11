#include "Baralho.h"
#include <algorithm>
#include <random>   
#include <stdexcept>

Baralho::Baralho(const sf::Font& fonte) {
    Naipe todosOsNaipes[] = { Naipe::Paus, Naipe::Ouros, Naipe::Espadas, Naipe::Copas };
    Valor todosOsValores[] = {
        Valor::Dois, Valor::Tres, Valor::Quatro, Valor::Cinco,
        Valor::Seis, Valor::Sete, Valor::Oito, Valor::Nove,
        Valor::Dez, Valor::Valete, Valor::Dama, Valor::Rei, Valor::As
    };

    for (Naipe n : todosOsNaipes) {
        for (Valor v : todosOsValores) {
            cartas.push_back(Carta(v, n, fonte));
        }
    }
}

void Baralho::embaralhar() {
    std::random_device dispositivoAleatorio;
    std::mt19937 gerador(dispositivoAleatorio());

    std::shuffle(cartas.begin(), cartas.end(), gerador);
}

Carta Baralho::distribuirCarta() {
    if (cartas.empty()) {
        throw std::out_of_range("Nao e possivel distribuir: o baralho esta vazio.");
    }

    Carta cartaDoTopo = cartas.back();
    
    cartas.pop_back();

    return cartaDoTopo;
}

int Baralho::obterTamanho() const {
    return cartas.size();
}