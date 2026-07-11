#include "GameEngine.h"
#include "BotSimple.h"
#include "BotRuleBased.h"
#include "BotMonteCarlo.h"
#include <SFML/System.hpp>
#include <SFML/Window/Mouse.hpp>
#include <string>
#include <algorithm> 

// ==========================================
// CONTEXTO: INICIALIZAÇÃO E MEMÓRIA
// ==========================================

GameEngine::GameEngine(const sf::Font& f) : fonte(f) {
    // Por padrão, todos os bots começam como "Simples"
    for (int i = 0; i < 4; i++) {
        modelosEscolhidos[i] = TipoDeBot::Simples;
    }
}

// Destrutor: limpa a memória quando a GameEngine for destruída
GameEngine::~GameEngine() {
    limparJogadoresDaMemoria();
}

void GameEngine::adicionarJogador(Jogador* j) { 
    jogadores.push_back(j); 
}

// Libera os ponteiros alocados dinamicamente para evitar vazamento de memória
void GameEngine::limparJogadoresDaMemoria() {
    for (Jogador* j : jogadores) {
        delete j;
    }
    jogadores.clear();
}

// Cria os jogadores baseando-se nas escolhas da interface
void GameEngine::prepararJogadores() {
    limparJogadoresDaMemoria();
    std::string nomes[4] = {"Bot Sul", "Bot Oeste", "Bot Norte", "Bot Leste"};

    for (int i = 0; i < 4; i++) {
        Jogador* novoBot = nullptr;
        if (modelosEscolhidos[i] == TipoDeBot::Simples) {
            novoBot = new BotSimple(nomes[i]);
        } else if (modelosEscolhidos[i] == TipoDeBot::RuleBased) {
            novoBot = new BotRuleBased(nomes[i]);
        } else if (modelosEscolhidos[i] == TipoDeBot::MonteCarlo) {
            novoBot = new BotMonteCarlo(nomes[i]);
        }
        jogadores.push_back(novoBot);
    }
}

void GameEngine::distribuirCartas() {
    Baralho baralhoAtual(fonte);
    baralhoAtual.embaralhar();
    for (int i = 0; i < 13; i++) {
        for (Jogador* j : jogadores) {
            j->adicionarCarta(baralhoAtual.distribuirCarta());
        }
    }
}

// ==========================================
// CONTEXTO: ANDAMENTO
// ==========================================

void GameEngine::jogarPartida(sf::RenderWindow& window, bool headless) {
    distribuirCartas();

    // ==========================================
    // FASE DE PASSAGEM DE CARTAS
    // ==========================================
    int rodadaAtual = jogadores[0]->obterHistoricoDePontos().size();
    int tipoPassagem = rodadaAtual % 4; // 0=Esquerda, 1=Frente, 2=Direita, 3=Ninguém

    if (tipoPassagem != 3) {
        std::vector<std::vector<Carta>> cartasParaPassar(4);
        
        // Cada jogador escolhe as suas 3 cartas
        for (int i = 0; i < 4; i++) {
            cartasParaPassar[i] = jogadores[i]->escolherCartasParaPassar();
        }

        // Entregamos as cartas ao alvo correto
        for (int i = 0; i < 4; i++) {
            int alvo = 0;
            if (tipoPassagem == 0) alvo = (i + 1) % 4; // Passa para a esquerda
            else if (tipoPassagem == 1) alvo = (i + 2) % 4; // Passa para a frente
            else if (tipoPassagem == 2) alvo = (i + 3) % 4; // Passa para a direita

            jogadores[alvo]->receberCartas(cartasParaPassar[i]);
        }
    }

    // ==========================================
    // INÍCIO DAS JOGADAS (13 VAZAS)
    // ==========================================
    int liderAtual = 0;
    for (int i = 0; i < 4; i++) {
        if (jogadores[i]->possuiDoisDePaus()) {
            liderAtual = i;
            break;
        }
    }

    for (int vaza = 0; vaza < 13; vaza++) {

        for (int i = 0; i < 4; i++) {
        int turno = (liderAtual + i) % 4;
    
    // ATUALIZAÇÃO 1: Passar as cartas da mesa para o bot olhar
        Carta jogada = jogadores[turno]->escolherCarta(estado.obterNaipeLider(), estado.obterCartasNaMesa());
    
        jogada.definirViradaParaCima(true);
        if (turno == 0) jogada.definirPosicao(360.f, 300.f); 
        if (turno == 1) jogada.definirPosicao(260.f, 240.f); 
        if (turno == 2) jogada.definirPosicao(360.f, 180.f); 
        if (turno == 3) jogada.definirPosicao(460.f, 240.f); 

        estado.adicionarCartaNaMesa(jogada);
        desenharInterface(window);
    
        for(int observador = 0; observador < 4; observador++) {
            jogadores[observador]->observarJogada(turno, jogada, estado.obterNaipeLider());
            }
    
        sf::sleep(sf::milliseconds(5)); 
        }

        std::vector<Carta> mesa = estado.obterCartasNaMesa();
        Naipe naipeLider = estado.obterNaipeLider().value();
        Valor maiorValor = Valor::Dois;
        int indiceVencedorMesa = 0;
        int pontosDaVaza = 0;

        for (int i = 0; i < 4; i++) {
            pontosDaVaza += mesa[i].obterPontos();
            if (mesa[i].obterNaipe() == naipeLider) {
                if (mesa[i].obterValor() >= maiorValor || i == 0) {
                    maiorValor = mesa[i].obterValor();
                    indiceVencedorMesa = i;
                }
            }
        }

        liderAtual = (liderAtual + indiceVencedorMesa) % 4;
        jogadores[liderAtual]->adicionarPontos(pontosDaVaza);

        sf::sleep(sf::milliseconds(5)); 
        estado.limparMesa();
        desenharInterface(window);
    }

    // Regra: Pedra na Lua (Acertar a Lua / Shoot the Moon)
    int jogadorComLua = -1;
    for (int i = 0; i < 4; i++) {
        if (jogadores[i]->obterPontosDaRodadaAtual() == 26) {
            jogadorComLua = i;
            break;
        }
    }

    if (jogadorComLua != -1) {
        for (int i = 0; i < 4; i++) {
            if (i == jogadorComLua) {
                jogadores[i]->definirPontosDaRodadaAtual(0); 
            } else {
                jogadores[i]->definirPontosDaRodadaAtual(26); 
            }
        }
    }

    for (Jogador* j : jogadores) {
        j->finalizarRodada();
    }
}

bool GameEngine::atingiuLimiteDePontos() const {
    for (Jogador* j : jogadores) {
        if (j->obterPontosTotais() >= 100) return true;
    }
    return false;
}

void GameEngine::iniciarFluxo(sf::RenderWindow& window) {
    while (window.isOpen()) {
        int escolhaMenu = mostrarMenuInicial(window);
        
        if (escolhaMenu == 0) {
            break; // Sair
        } else if (escolhaMenu == 2) {
            mostrarConfiguracaoBots(window); // Abre a tela de configuração
        } else if (escolhaMenu == 1) {
            
            // Instancia os robôs antes da partida começar
            prepararJogadores();

            for (Jogador* j : jogadores) j->resetarPartida();

            while (!atingiuLimiteDePontos()) {
                jogarPartida(window);
                
                if (atingiuLimiteDePontos()) {
                    mostrarPlacar(window, true);
                } else {
                    mostrarPlacar(window, false);
                }
            }
        }
    }
}

// ==========================================
// CONTEXTO: INTERFACE
// ==========================================

void GameEngine::desenharInterface(sf::RenderWindow& window) {
    window.clear(sf::Color(0, 128, 0));

    for (int i = 0; i < 4; i++) {
        std::string textoInfo = jogadores[i]->obterNome();
        sf::Text label(fonte);
        label.setString(textoInfo);
        label.setCharacterSize(16);
        label.setFillColor(sf::Color::White);

        float baseX = 0, baseY = 0;
        if (i == 0) { baseX = 250.f; baseY = 450.f; label.setPosition(sf::Vector2f({350.f, 575.f})); }
        if (i == 1) { baseX = 20.f;  baseY = 150.f; label.setPosition(sf::Vector2f({20.f, 120.f})); }
        if (i == 2) { baseX = 250.f; baseY = 20.f;  label.setPosition(sf::Vector2f({350.f, 0.f})); }
        if (i == 3) { baseX = 700.f; baseY = 150.f; label.setPosition(sf::Vector2f({650.f, 120.f})); }

        window.draw(label);

        auto& mao = jogadores[i]->obterMao();
        for (int c = 0; c < mao.size(); c++) {
            mao[c].definirViradaParaCima(false);
            if (i == 0 || i == 2) mao[c].definirPosicao(baseX + (c * 20.f), baseY);
            else mao[c].definirPosicao(baseX, baseY + (c * 20.f));
            mao[c].desenhar(window);
        }
    }

    for (Carta c : estado.obterCartasNaMesa()) {
        c.desenhar(window);
    }

    window.display();
}

int GameEngine::mostrarMenuInicial(sf::RenderWindow& window) {
    Button btnNovoJogo("Novo Jogo", fonte, 300.f, 200.f, 200.f, 50.f);
    Button btnConfig("Configurar Bots", fonte, 300.f, 280.f, 200.f, 50.f);
    Button btnSair("Sair", fonte, 300.f, 360.f, 200.f, 50.f);

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                return 0;
            }
            if (const auto* mouseBtn = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseBtn->button == sf::Mouse::Button::Left) {
                    sf::Vector2f posClique({static_cast<float>(mouseBtn->position.x), static_cast<float>(mouseBtn->position.y)});
                    if (btnNovoJogo.isClicked(posClique)) return 1;
                    if (btnConfig.isClicked(posClique)) return 2;
                    if (btnSair.isClicked(posClique)) {
                        window.close();
                        return 0;
                    }
                }
            }
        }

        window.clear(sf::Color(0, 100, 0));
        sf::Text titulo(fonte);
        titulo.setString("JOGO DE COPAS");
        titulo.setCharacterSize(40);
        titulo.setFillColor(sf::Color::Yellow);
        titulo.setPosition(sf::Vector2f({250.f, 100.f}));
        window.draw(titulo);

        btnNovoJogo.draw(window);
        btnConfig.draw(window);
        btnSair.draw(window);
        window.display();
    }
    return 0;
}

void GameEngine::mostrarConfiguracaoBots(sf::RenderWindow& window) {
    Button btnVoltar("Voltar", fonte, 300.f, 500.f, 200.f, 50.f);
    
    // Matriz de CheckBoxes [4 robôs][3 opções]
    std::vector<std::vector<CheckBox>> caixas;
    std::string nomes[4] = {"Sul", "Oeste", "Norte", "Leste"};

    for (int i = 0; i < 4; i++) {
        std::vector<CheckBox> linha;
        float y = 150.f + (i * 80.f);
        linha.push_back(CheckBox("Simples", fonte, 200.f, y));
        linha.push_back(CheckBox("RuleBased", fonte, 350.f, y));
        linha.push_back(CheckBox("MonteCarlo", fonte, 500.f, y));
        
        // Marca a opção baseada no que está salvo na memória da Engine
        linha[static_cast<int>(modelosEscolhidos[i])].setChecked(true);
        caixas.push_back(linha);
    }

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                return;
            }
            if (const auto* mouseBtn = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseBtn->button == sf::Mouse::Button::Left) {
                    sf::Vector2f posClique({static_cast<float>(mouseBtn->position.x), static_cast<float>(mouseBtn->position.y)});
                    
                    if (btnVoltar.isClicked(posClique)) return;

                    // Lógica de seleção única por linha (comportamento de "Radio Button")
                    for (int i = 0; i < 4; i++) {
                        for (int opcao = 0; opcao < 3; opcao++) {
                            if (caixas[i][opcao].isClicked(posClique)) {
                                caixas[i][0].setChecked(false);
                                caixas[i][1].setChecked(false);
                                caixas[i][2].setChecked(false);
                                caixas[i][opcao].setChecked(true);
                                
                                // Salva a escolha
                                modelosEscolhidos[i] = static_cast<TipoDeBot>(opcao);
                            }
                        }
                    }
                }
            }
        }

        window.clear(sf::Color(0, 80, 0));
        
        sf::Text titulo(fonte);
        titulo.setString("CONFIGURAR BOTS");
        titulo.setCharacterSize(30);
        titulo.setFillColor(sf::Color::Yellow);
        titulo.setPosition(sf::Vector2f({260.f, 50.f}));
        window.draw(titulo);

        for (int i = 0; i < 4; i++) {
            sf::Text nomeStr(fonte);
            nomeStr.setString("Bot " + nomes[i] + ":");
            nomeStr.setCharacterSize(20);
            nomeStr.setFillColor(sf::Color::Cyan);
            nomeStr.setPosition(sf::Vector2f({80.f, 148.f + (i * 80.f)}));
            window.draw(nomeStr);

            for (int opcao = 0; opcao < 3; opcao++) {
                caixas[i][opcao].draw(window);
            }
        }

        btnVoltar.draw(window);
        window.display();
    }
}

void GameEngine::mostrarPlacar(sf::RenderWindow& window, bool ehPlacarFinal) {
    Button btnProxima("Proxima Rodada", fonte, 300.f, 520.f, 200.f, 40.f);
    Button btnMenu("Tela Inicial", fonte, 200.f, 520.f, 180.f, 40.f);
    Button btnSair("Sair", fonte, 420.f, 520.f, 180.f, 40.f);

    bool aguardandoClique = true;
    float deslocamentoScroll = 0.f;

    int minTotal = 99999;
    int maxTotal = -1;
    
    for (int i = 0; i < 4; i++) {
        int total = jogadores[i]->obterPontosTotais();
        if (total < minTotal) minTotal = total;
        if (total > maxTotal) maxTotal = total;
    }

    sf::Color corDaColuna[4];
    int colocacao[4]; 

    for (int i = 0; i < 4; i++) {
        int total = jogadores[i]->obterPontosTotais();
        
        if (total == minTotal) {
            corDaColuna[i] = sf::Color::Yellow; 
        } else if (total == maxTotal) {
            corDaColuna[i] = sf::Color::Red;    
        } else {
            corDaColuna[i] = sf::Color::White;  
        }

        colocacao[i] = 1; 
        for (int j = 0; j < 4; j++) {
            if (jogadores[j]->obterPontosTotais() < jogadores[i]->obterPontosTotais()) {
                colocacao[i]++;
            }
        }
    }

    while (window.isOpen() && aguardandoClique) {
        int qtdRodadas = jogadores[0]->obterHistoricoDePontos().size();
        float alturaConteudo = qtdRodadas * 30.f + 100.f; 
        float limiteScroll = std::max(0.f, alturaConteudo - 400.f); 

        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                return;
            }

            if (const auto* mouseBtn = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseBtn->button == sf::Mouse::Button::Left) {
                    sf::Vector2f posClique({static_cast<float>(mouseBtn->position.x), static_cast<float>(mouseBtn->position.y)});
                    if (ehPlacarFinal) {
                        if (btnMenu.isClicked(posClique)) aguardandoClique = false;
                        if (btnSair.isClicked(posClique)) window.close();
                    } else {
                        if (btnProxima.isClicked(posClique)) aguardandoClique = false;
                    }
                }
            }

            if (const auto* scroll = event->getIf<sf::Event::MouseWheelScrolled>()) {
                if (scroll->wheel == sf::Mouse::Wheel::Vertical) {
                    deslocamentoScroll -= scroll->delta * 30.f;
                    if (deslocamentoScroll < 0.f) deslocamentoScroll = 0.f;
                    if (deslocamentoScroll > limiteScroll) deslocamentoScroll = limiteScroll;
                }
            }
        }

        window.setView(window.getDefaultView()); 
        
        sf::RectangleShape fundoEscuro(sf::Vector2f({800.f, 600.f}));
        fundoEscuro.setFillColor(sf::Color(0, 0, 0, 240));
        window.draw(fundoEscuro);

        sf::Text titulo(fonte);
        titulo.setString(ehPlacarFinal ? "Fim de Jogo! Placar Final:" : "Placar Acumulado (Rodada " + std::to_string(qtdRodadas) + "):");
        titulo.setCharacterSize(30);
        titulo.setFillColor(sf::Color::Yellow);
        titulo.setPosition(sf::Vector2f({180.f, 20.f}));
        window.draw(titulo);

        float eixoX[] = {50.f, 180.f, 330.f, 480.f, 630.f};

        for (int i = 0; i < 4; i++) {
            sf::Text cabecalho(fonte);
            std::string textoCabecalho = std::to_string(colocacao[i]) + "o - " + jogadores[i]->obterNome();
            cabecalho.setString(textoCabecalho);
            cabecalho.setCharacterSize(16); 
            cabecalho.setFillColor(corDaColuna[i]); 
            cabecalho.setPosition(sf::Vector2f({eixoX[i + 1], 70.f}));
            window.draw(cabecalho);
        }

        sf::View viewTabela;
        viewTabela.setSize(sf::Vector2f({800.f, 400.f}));
        viewTabela.setCenter(sf::Vector2f({400.f, 200.f + deslocamentoScroll}));
        viewTabela.setViewport(sf::FloatRect({0.f, 100.f / 600.f}, {1.f, 400.f / 600.f}));
        
        window.setView(viewTabela);

        float eixoY = 20.f; 
        sf::RectangleShape divisorTopo(sf::Vector2f({700.f, 2.f}));
        divisorTopo.setFillColor(sf::Color::White);
        divisorTopo.setPosition(sf::Vector2f({50.f, eixoY}));
        window.draw(divisorTopo);
        eixoY += 15.f;

        for (int r = 0; r < qtdRodadas; r++) {
            sf::Text rodadaStr(fonte);
            rodadaStr.setString("Rodada " + std::to_string(r + 1));
            rodadaStr.setCharacterSize(18);
            rodadaStr.setFillColor(sf::Color::White);
            rodadaStr.setPosition(sf::Vector2f({eixoX[0], eixoY}));
            window.draw(rodadaStr);

            for (int i = 0; i < 4; i++) {
                sf::Text pontosStr(fonte);
                int pts = jogadores[i]->obterHistoricoDePontos()[r];
                pontosStr.setString(std::to_string(pts));
                pontosStr.setCharacterSize(18);
                pontosStr.setFillColor(corDaColuna[i]);
                pontosStr.setPosition(sf::Vector2f({eixoX[i + 1], eixoY}));
                window.draw(pontosStr);
            }
            eixoY += 30.f;
        }

        window.setView(window.getDefaultView());

        if (limiteScroll > 0) {
            sf::RectangleShape barraFundo(sf::Vector2f({10.f, 400.f}));
            barraFundo.setPosition(sf::Vector2f({780.f, 100.f}));
            barraFundo.setFillColor(sf::Color(50, 50, 50, 150));
            window.draw(barraFundo);

            float tamanhoBarra = std::max(30.f, 400.f * (400.f / alturaConteudo));
            float proporcao = deslocamentoScroll / limiteScroll;
            
            sf::RectangleShape marcadorRolagem(sf::Vector2f({10.f, tamanhoBarra}));
            marcadorRolagem.setPosition(sf::Vector2f({780.f, 100.f + proporcao * (400.f - tamanhoBarra)}));
            marcadorRolagem.setFillColor(sf::Color(180, 180, 180));
            window.draw(marcadorRolagem);
        }

        sf::RectangleShape divisorBase(sf::Vector2f({800.f, 2.f}));
        divisorBase.setFillColor(sf::Color::White);
        divisorBase.setPosition(sf::Vector2f({0.f, 500.f}));
        window.draw(divisorBase);

        if (ehPlacarFinal) {
            btnMenu.draw(window);
            btnSair.draw(window);
        } else {
            btnProxima.draw(window);
        }

        window.display();
    }
}