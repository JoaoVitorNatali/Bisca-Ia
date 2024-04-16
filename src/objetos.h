#include "redeNeural.c"
#include "vector"
#include "array"

#define QTD_LAYERS   1
#define QTD_INPUT    10
#define QTD_HIDE     6
#define QTD_OUTPUT   3


#define DOIS 1
#define TRES 2
#define QUATRO 3
#define CINCO 4
#define SEIS 5
#define DAMA 6
#define VALETE 7
#define REIS 8
#define SETE 9
#define AS 10

#define OITO 55
#define NOVE 56
#define DEZ 57

#define COPAS 1
#define OURO 2
#define ESPADA 3
#define PAUS 4


std::array<int, 10> CARTAS_EM_ORDEM_DE_VALOR = {DOIS, TRES, QUATRO, CINCO, SEIS, DAMA, VALETE, REIS, SETE, AS};
std::array<int, 10> CARTAS_EM_ORDEM_SPRITE = {AS, DOIS, TRES, QUATRO, CINCO, SEIS, SETE, VALETE, DAMA, REIS};
std::array<int, 10> VALORES_CARTAS = {0, 0, 0, 0, 0, 2, 3, 4, 10, 11};
std::array<int, 4> NAIPES = {ESPADA, PAUS, OURO, COPAS};
std::vector<std::string> NOMES_CARTAS_BARALHO = {"as", "dois", "tres", "quatro", "cinco", "seis", "sete", "valete", "dama", "reis"};
std::vector<std::string> NOMES_NAIPES_BARALHO = {"espada", "paus", "ouro", "copas"};

std::array<int, 13> SPRITES_BARALHO_COPAS;
std::array<int, 13> SPRITES_BARALHO_OURO;
std::array<int, 13> SPRITES_BARALHO_ESPADA;
std::array<int, 13> SPRITES_BARALHO_PAUS;



class Jogador{
public:
    int TamanhoDNA;
    double* DNA;
    double Fitness;
    RedeNeural* Cerebro;

    Jogador(){
        this->mao.push_back({0,0});
        this->mao.push_back({0,0});
        this->mao.push_back({0,0});
    }

    virtual ~Jogador(){
        RNA_DestruirRedeNeural(this->Cerebro);
        free(this->DNA);
    }

    void alocar(){
        this->Cerebro = RNA_CriarRedeNeural(QTD_LAYERS, QTD_INPUT, QTD_HIDE, QTD_OUTPUT);
        this->TamanhoDNA = RNA_QuantidadePesos(this->Cerebro);
        this->DNA = (double*)malloc(this->TamanhoDNA*sizeof(double));

        for(int j=0; j<this->TamanhoDNA; j++){
            this->DNA[j] = getRandomValue();
        }
    }

    void alocarCerebro(RedeNeural *rede){
        this->Cerebro = RNA_CriarRedeNeural(
                                            rede->QuantidadeEscondidas,
                                            rede->CamadaEntrada.QuantidadeNeuronios,
                                            rede->CamadaEscondida->QuantidadeNeuronios,
                                            rede->CamadaSaida.QuantidadeNeuronios);

        int TamanhoDNA = RNA_QuantidadePesos(this->Cerebro);
        this->DNA = (double*)malloc(TamanhoDNA*sizeof(double));

        RNA_CopiarCamadasParaVetor(rede, this->DNA);
        RNA_CopiarVetorParaCamadas(this->Cerebro, this->DNA);
    }

    void inicializar(){
        this->Fitness = 0;
        RNA_CopiarVetorParaCamadas(this->Cerebro, this->DNA);
    }

    void iniciarNovoJogo(int cartaTrunfo, int naipeTrunfo){
        this->trunfo[0] = cartaTrunfo;
        this->trunfo[1] = naipeTrunfo;
        this->pontos = 0;
        this->zerarMao();
    }

    void addCarta(int carta, int naipe){
        for(int i = 0; i < this->mao.size(); i++){
            if(this->mao[i][0] == 0){
                this->mao[i][0] = carta;
                this->mao[i][1] = naipe;
                return;
            }
        }
    }

    void setPontos(int pontos){
        this->pontos = pontos;
    }

    void jogar(int cartaMesa, int naipeMesa, int *carta, int *naipe){
        this->cartasMesa[0][0] = cartaMesa;
        this->cartasMesa[0][1] = naipeMesa;

        int posicaoCartaEscolhida = this->escolherCarta();

        posicaoCartaEscolhida = this->verificarCartaEscolhida(posicaoCartaEscolhida);

        *carta = this->mao[posicaoCartaEscolhida][0];
        *naipe = this->mao[posicaoCartaEscolhida][1];

        this->removeCarta(posicaoCartaEscolhida);
    }

    std::vector<std::vector<int>> getMao(){
        return this->mao;
    }

    void somarFitness(){
        this->Fitness += this->pontos;
    }

    void removeCarta(int posCarta){
        this->mao[posCarta][0] = 0;
        this->mao[posCarta][1] = 0;
        ///this->mao.erase(this->mao.begin() + posCarta); /// remover carta do vetor
    }

    void printarCartas(){
        printf("\n %d, %d    %d, %d   %d, %d", mao[0][0], mao[0][1], mao[1][0], mao[1][1], mao[2][0], mao[2][1]);
    }


private:
    int cartasMesa[2][2] = {{0,0},{0,0}};
    std::vector<std::vector<int>> mao;
    int pontos = 0;
    int trunfo[2];

    int verificarCartaEscolhida(int posicao){
        if(this->mao[posicao][0] != 0) return posicao;
        else {
            for(int i = 0; i < this->mao.size(); i++){
                if(this->mao[i][0]) return i;
            }
        }
    }

    int escolherCarta(){
        double entradas[QTD_INPUT];
        double saidas[QTD_OUTPUT];

        this->aplicaEntrada(entradas);

        RNA_CopiarParaEntrada(this->Cerebro, entradas);
        RNA_CalcularSaida(this->Cerebro);
        RNA_CopiarDaSaida(this->Cerebro, saidas);

        return this->obterSaida(saidas);
    }

    void aplicaEntrada(double *entradas){
        int i = 0;
        for(i = 0; i < 3; i++){
            if(i < this->mao.size()){
                entradas[i] = this->mao[i][0];
                entradas[i] = this->mao[i][1];
            } else {
                entradas[i] = 0;
                entradas[i] = 0;
            }
        }
        entradas[i] = this->cartasMesa[0][0]; /// carta na mesa se houver
        entradas[i+1] = this->cartasMesa[0][1]; /// naipe da carta na mesa se houver
        entradas[i+2] = this->trunfo[0]; /// carta do trunfo
        entradas[i+3] = this->trunfo[1]; /// carta do trunfo
    }

    int obterSaida(double *saidas){
        if(saidas[0] > 0.0) return 0;
        if(saidas[1] > 0.0) return 1;
        if(saidas[2] > 0.0) return 2;
    }

    void zerarMao(){
        for(int i = 0; i < 3; i++){
            this->mao[i][0] = 0;
            this->mao[i][1] = 0;
        }
    }
};


class Baralho{

public:
    int x;
    int y;
    int cartasMesa[2][2] = {{0,0}, {0,0}};
    std::vector<std::vector<int>> cartas;
    std::array<Jogador, 2> jogadores;
    int vez;
    int pontos[2] = {0,0};
    int trunfo[2] = {0,0};
    bool trunfoEstaNoBaralho = true;
    bool jogoAcabou = false;

    Baralho(){
    }

    virtual ~Baralho(){
    }

    void jogar(){
        /// jogada de treinamento evolutivo, ia contra ia

        if(this->jogoAcabou) return;

        int cartaJogada, naipeJogada, cartaMesa, naipeMesa;
        cartaMesa = cartasMesa[0][0];
        naipeMesa = cartasMesa[0][1];

        this->jogadores[this->vez].jogar(cartaMesa, naipeMesa, &cartaJogada, &naipeJogada);

        this->realizarJogada(cartaJogada, naipeJogada);

        this->verificarEstadoJogo();
    }

    void jogadaUsuario(int posicaoCarta){
        if(this->jogoAcabou) return;
        if(this->vez == 0){
            int cartaJogada = this->jogadores[0].getMao()[posicaoCarta][0];
            int naipeJogada = this->jogadores[0].getMao()[posicaoCarta][1];
            this->jogadores[0].removeCarta(posicaoCarta);

            if(cartaJogada == 0) return;

            this->realizarJogada(cartaJogada, naipeJogada);
            this->verificarEstadoJogo();
        }
    }

    void jogadaIAContraUsuario(){
        if(this->vez == 1){
            this->jogar();
        }
    }

    void alocarJogadores(){
        jogadores[0].alocar();
        jogadores[1].alocar();
    }

    void inicializarJogadores(){
        jogadores[0].inicializar();
        jogadores[1].inicializar();
    }

    void reiniciarJogo(){
        this->reiniciarBaralho();
        this->jogadores[0].setPontos(0);
        this->jogadores[1].setPontos(0);
        this->jogadores[0].iniciarNovoJogo(this->trunfo[0], this->trunfo[1]);
        this->jogadores[1].iniciarNovoJogo(this->trunfo[0], this->trunfo[1]);

        this->darCartas();this->darCartas();this->darCartas(); /// dar as 3 primeiras cartas
    }

    bool jogoFinalizado(){
        return this->jogoAcabou;
    }


private:

    void verificarEstadoJogo(){
        if(this->cartas.size() == 0 && jogadorComMaoVazia(0) && jogadorComMaoVazia(1)){
            this->jogoAcabou = true;
        }
    }

    bool jogadorComMaoVazia(int idJogador){
        bool vazia = true;
        for(int i = 0; i < 3; i++){
            if(this->jogadores[idJogador].getMao()[i][0] != 0) vazia = false;
        }
        return vazia;
    }

    void darCartas(){
        this->darCarta(this->vez);
        this->darCarta((this->vez == 0) ? 1 : 0);
    }

    void darCarta(int idJogador){
        int carta = NULL, naipe = NULL;
        if(this->cartas.size() > 0){
            int idCarta = obterCartaAleatoria(); /// obter uma posicao aleatoria no array de cartas
            carta = this->cartas[idCarta][0]; /// pegar valor da carta
            naipe = this->cartas[idCarta][1]; /// pegar naipe da carta
            this->cartas.erase(this->cartas.begin() + idCarta); /// remover carta do vetor
        } else if(this->trunfoEstaNoBaralho){
            carta = this->trunfo[0];
            naipe = this->trunfo[1];
            this->trunfoEstaNoBaralho = false;
        }

        if(carta != NULL)
            this->jogadores[idJogador].addCarta(carta, naipe); /// adicionar carta para o jogador
    }

    int obterCartaAleatoria(){
        int quantidadeCartas = this->cartas.size();
        int indiceCarta = rand() % quantidadeCartas;
        return indiceCarta;
    }

    void realizarJogada(int carta, int naipe){
        /// verificar se slot 0 ou 1 esta vazio e preencher com a jogada
        int indice;
        if(cartasMesa[0][0] == 0) indice = 0;
        else indice = 1;

        cartasMesa[indice][0] = carta;
        cartasMesa[indice][1] = naipe;

        if(cartasMesa[1][0] != 0){/// slot 1 nao ta vazio, os dois jogadores ja jogaram
            verificaQuemGanhouJogada();
        } else { /// teve somente uma jogada, alterar vez
            this->alterarVez();
        }
    }

    void verificaQuemGanhouJogada(){
        int carta1 = this->cartasMesa[0][0];
        int naipe1 = this->cartasMesa[0][1];
        int carta2 = this->cartasMesa[1][0];
        int naipe2 = this->cartasMesa[1][1];
        int cartaTrunfo = this->trunfo[0];
        int naipeTrunfo = this->trunfo[1];
        int idGanhador;

        if(naipe1 != naipe2 && naipe2 != naipeTrunfo){
            /// jogador da vez nao jogou trunfo nem encartou
            idGanhador = (vez == 1) ? 0 : 1;
        } else if(naipe1 != naipeTrunfo && naipe2 == naipeTrunfo){
            /// jogador da vez jogou um trunfo em cima de uma carta normal
            idGanhador = vez;
        } else if(naipe1 == naipe2){
            /// verificar se houve encarte0
            int valor1 = this->getValorCartaById(carta1);
            int valor2 = this->getValorCartaById(carta2);
            if(valor2 > valor1) idGanhador = vez;
            else idGanhador = (vez == 1) ? 0 : 1;
        } else {
            printf("\nJogada nao mapeada, carta1 %d, carta2 %d, naipe1 %d, naipe2 %d", carta1, carta2, naipe1, naipe2);
        }

        this->calcularPontos(idGanhador, carta1, carta2);
        this->vez = idGanhador; /// quem ganhou a jogada continua com a vez
        this->zerarCartasMesa();
        this->darCartas();
    }

    void calcularPontos(int indiceGanhador, int carta1, int carta2){
        int valor1 = getValorCartaById(carta1);
        int valor2 = getValorCartaById(carta2);
        int soma = valor1 + valor2;
        this->pontos[indiceGanhador] += soma;
        this->jogadores[indiceGanhador].setPontos(this->pontos[indiceGanhador]);
    }

    void preencherCartasBaralho(){
        for(int i = 0; i < 4; i++){ /// percorrer naipes
            char naipe = NAIPES[i];

            for(int j = 0; j < 10; j++){ /// percorrer cartas
                int carta = CARTAS_EM_ORDEM_DE_VALOR[j];
                this->cartas.push_back({carta, naipe});
            }
        }
    }

    void reiniciarBaralho(){
        this->zerarPontos();
        this->definirQuemComeca();
        this->zerarCartasMesa();
        this->preencherCartasBaralho();
        this->definirTrunfo();
        this->jogoAcabou = false;
    }

    void zerarPontos(){
        this->pontos[0] = 0;
        this->pontos[1] = 0;
    }

    void zerarCartasMesa(){
        for(int i = 0; i < 2; i++){
            this->cartasMesa[i][0] = 0;
            this->cartasMesa[i][1] = 0;
        }
    }

    void definirTrunfo(){
        this->trunfoEstaNoBaralho = true;
        int idCarta = obterCartaAleatoria();
        int carta = this->cartas[idCarta][0];
        int naipe = this->cartas[idCarta][1];

        this->trunfo[0] = carta;
        this->trunfo[1] = naipe;

        this->cartas.erase(this->cartas.begin() + idCarta); /// remover carta do vetor
    }

    void definirQuemComeca(){
        int x = rand()%2;
        this->vez = x;
    }

    void alterarVez(){
        if(this->vez == 1) this->vez = 0;
        else this->vez = 1;
    }

    int getValorCartaById(int idCarta){
        int indiceCarta;
        for(int i = 0; i < 10; i++){
            if(CARTAS_EM_ORDEM_DE_VALOR[i] == idCarta)
                indiceCarta = i;
        }
        return VALORES_CARTAS[indiceCarta];
    }
};


std::array<Baralho, QUANTIDADE_JOGOS> baralhos;

