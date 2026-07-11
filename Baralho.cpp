#include "Baralho.h"
#include <algorithm> // Necessário para a função de misturar (shuffle)
#include <random>    // Necessário para a geração de números aleatórios
#include <stdexcept> // Necessário para tratar erros (exceções)

// O construtor é chamado automaticamente quando criamos o Baralho
Baralho::Baralho(const sf::Font& fonte) {
    // Definimos coleções fixas com todos os valores e naipes possíveis
    Naipe todosOsNaipes[] = { Naipe::Paus, Naipe::Ouros, Naipe::Espadas, Naipe::Copas };
    Valor todosOsValores[] = {
        Valor::Dois, Valor::Tres, Valor::Quatro, Valor::Cinco,
        Valor::Seis, Valor::Sete, Valor::Oito, Valor::Nove,
        Valor::Dez, Valor::Valete, Valor::Dama, Valor::Rei, Valor::As
    };

    // Laço de repetição duplo: para cada naipe, percorremos todos os valores
    for (Naipe n : todosOsNaipes) {
        for (Valor v : todosOsValores) {
            // Criamos uma carta nova e a empurramos para o final da nossa lista
            cartas.push_back(Carta(v, n, fonte));
        }
    }
}

// Implementação do método de misturar
void Baralho::embaralhar() {
    // Configura um dispositivo baseado no relógio do computador para gerar aleatoriedade real
    std::random_device dispositivoAleatorio;
    // Cria o motor de geração de números (algoritmo Mersenne Twister)
    std::mt19937 gerador(dispositivoAleatorio());

    // Mistura do começo ao fim da lista usando o nosso gerador
    std::shuffle(cartas.begin(), cartas.end(), gerador);
}

// Implementação do método de entrega
Carta Baralho::distribuirCarta() {
    // Trava de segurança: se o baralho estiver vazio, o programa acusa um erro
    if (cartas.empty()) {
        throw std::out_of_range("Nao e possivel distribuir: o baralho esta vazio.");
    }

    // Copia a última carta da lista (o "topo" do baralho)
    Carta cartaDoTopo = cartas.back();
    
    // Remove esta carta da lista do baralho, diminuindo o tamanho total
    cartas.pop_back();

    return cartaDoTopo;
}

// Retorna a quantidade de elementos na lista
int Baralho::obterTamanho() const {
    return cartas.size();
}