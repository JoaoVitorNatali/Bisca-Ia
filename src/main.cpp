#include <string>
#include "PIG.h"
#include "definicoes.h"
#include "objetos.h"
#include "desenhar.h"
#include "verificaTecladoEMouse.h"

void randomMutations()
{
    static double RangeRandom = baralhos[0].jogadores[0].TamanhoDNA;

	int quantidadeIndividuos = QTD_JOGOS;
    Jogador* Vetor[QTD_JOGOS * 2];
    Jogador* Temp;

    int indice = 0;
    for(int i=0; i<QTD_JOGOS; i++)
    {
        Vetor[i] = & baralhos[i].jogadores[0];
        indice++;
        Vetor[i] = & baralhos[i].jogadores[1];
        indice++;
    }

    for(int i=0; i<quantidadeIndividuos; i++)
    {
        for(int j=0; j<quantidadeIndividuos-1; j++)
        {
            if(Vetor[j]->Fitness < Vetor[j+1]->Fitness)
            {
                Temp = Vetor[j];
                Vetor[j] = Vetor[j+1];
                Vetor[j+1] = Temp;
            }
        }
    }

    int Step = 5;
    for(int i=0; i<Step; i++)  /// Clonando individuos
    {
        for(int j=Step+i; j<quantidadeIndividuos; j=j+Step)
        {
            for(int k=0; k<Vetor[j]->TamanhoDNA; k++)
            {
                Vetor[j]->DNA[k] = Vetor[i]->DNA[k];        /// individuo 'j' recebe dna do individuo 'i'
            }
        }
    }

    for(int j=Step; j<quantidadeIndividuos; j++)
    {
        int tipo;
        int mutations = (rand()%(int)RangeRandom)+1;

        for(int k=0; k<mutations; k++)
        {
            tipo = rand()%3;

            int indice = rand()%Vetor[j]->TamanhoDNA;
            switch(tipo)
            {
                case 0:
                {
                    //int mutations = 20;
                    Vetor[j]->DNA[indice] = getRandomValue();       /// Valor Aleatorio

                }   break;
                case 1:
                {
                    double number = (rand()%10001)/10000.0 + 0.5;
                    Vetor[j]->DNA[indice] = Vetor[j]->DNA[indice]*number;   /// Multiplicação aleatoria

                }   break;
                case 2:
                {
                    double number = getRandomValue()/100.0;
                    Vetor[j]->DNA[indice] = Vetor[j]->DNA[indice] + number; /// Soma aleatoria

                }   break;
            }
        }
    }


    RangeRandom = RangeRandom*0.999;
    if(RangeRandom < 15){
        RangeRandom = 15;
    }

    Geracao++;

    printf("\nGeracao %d", Geracao);
}

void iniciarSpritesBaralhos(){
    for(int i = 0; i < 10; i++){

        std::string carta1 = "img\\" + NOMES_CARTAS_BARALHO[i] + "_" + NOMES_NAIPES_BARALHO[0] + ".png";
        std::string carta2 = "img\\" + NOMES_CARTAS_BARALHO[i] + "_" + NOMES_NAIPES_BARALHO[1] + ".png";
        std::string carta3 = "img\\" + NOMES_CARTAS_BARALHO[i] + "_" + NOMES_NAIPES_BARALHO[2] + ".png";
        std::string carta4 = "img\\" + NOMES_CARTAS_BARALHO[i] + "_" + NOMES_NAIPES_BARALHO[3] + ".png";

        char* carta1char = new char[carta1.length() + 1];
        strcpy(carta1char, carta1.c_str());

        char* carta2char = new char[carta2.length() + 1];
        strcpy(carta2char, carta2.c_str());

        char* carta3char = new char[carta3.length() + 1];
        strcpy(carta3char, carta3.c_str());

        char* carta4char = new char[carta4.length() + 1];
        strcpy(carta4char, carta4.c_str());

        PIGCor cor = PRETO;

        SPRITES_BARALHO_ESPADA[i] = CriaSprite(carta1char, 0, &cor);
        SPRITES_BARALHO_PAUS[i] = CriaSprite(carta2char, 0, &cor);
        SPRITES_BARALHO_OURO[i] = CriaSprite(carta3char, 0, &cor);
        SPRITES_BARALHO_COPAS[i] = CriaSprite(carta4char, 0, &cor);
    }
}

void inicializarSprites(){
    SpriteFundo = CriaSprite("img\\fundo-8bits.jpg");
    SpriteFundoCarta = CriaSprite("img\\verso-carta.png");
    SpritePlacar = CriaSprite("img\\placar-8bits.png");
    SpriteRecomecar = CriaSprite("img\\recomecar.png");
    SpriteGanhou = CriaSprite("img\\ganhou.png");
    SpritePerdeu = CriaSprite("img\\perdeu.png");
    iniciarSpritesBaralhos();
}

void criarJogos(){
    int X = 0;
    int Y = 0;
    int j = 0;

    for(int i = 0; i < QTD_JOGOS; i++){
        baralhos[i].alocarJogadores();
        baralhos[i].x = X;
        baralhos[i].y = Y;

        if(j == QTD_JOGOS_LINHA){
            j = 0;
            X = 0;
            Y += LARGURA_JOGO;
        } else {
            X += LARGURA_JOGO;
        }
        j++;
    }
}

void iniciarNovaPartida(){
    for(int i = 0; i < QTD_JOGOS; i++){
        baralhos[i].reiniciarJogo();
    }
}

void inicializarJogadores(){
    for(int i = 0; i < QTD_JOGOS; i++){
        baralhos[i].inicializarJogadores();
    }
}

bool carregarRedeNeural();

void configuracoesIniciais(){
    CriaJogo("Bisca IA");
    SetTamanhoJanela(ALTURA_CENARIO, LARGURA_CENARIO);

    fonte = CriaFonteNormal("..\\fontes\\arial.ttf", LARGURA_JOGO/24);

    meuTeclado = GetTeclado();
    timerDesenho = CriaTimer();
    timerJogadas = CriaTimer();
    timerTeclado = CriaTimer();

    PreparaCameraMovel();
    inicializarSprites();

    if(MODO_JOGO == MODO_DUELO) QTD_JOGOS = 1;


    if(carregarRedeNeural() == false)
        criarJogos(); /// se nao carregou a rede neural, alocar com valores aleatorios

    inicializarJogadores();
    iniciarNovaPartida();
}



void desenhar(){
    if(!EXIBIR_ANIMACAO) return;
    IniciaDesenho();

    desenharJogos();

    EncerraDesenho();
}

void aplicaFitness(){
    for(int i = 0; i < QTD_JOGOS; i++){
        baralhos[i].jogadores[0].somarFitness();
        baralhos[i].jogadores[1].somarFitness();
    }
}

void verificarEstadoPartida(){
    bool acabou = true;
    for(int i = 0; i < QTD_JOGOS; i++){
        if(!baralhos[i].jogoFinalizado()) acabou = false;
    }

    if(acabou){
        partida++;

        if(partida >= QTD_PARTIDAS_ATE_EVOLUIR){
            partida = 0;
            randomMutations();
            inicializarJogadores();
        } else {
            aplicaFitness();
        }

        iniciarNovaPartida();
   }
}

void realizarJogadaJogadores(){
    if(MODO_JOGO == MODO_DUELO){
        baralhos[0].jogadaIAContraUsuario();
    } else {
        for(int i = 0; i < QTD_JOGOS; i++){
            if(!baralhos[i].jogoFinalizado()){
                baralhos[i].jogar();
            }
        }
    }
}

Jogador* obterMelhorJogador(){
    Jogador *melhor = &(baralhos[0].jogadores[0]);

    for(int i = 0; i < QTD_JOGOS; i++){
        Jogador *jogador1 = &baralhos[i].jogadores[0];
        Jogador *jogador2 = &baralhos[i].jogadores[1];

        if(jogador1->Fitness > melhor->Fitness) melhor = jogador1;
        if(jogador2->Fitness > melhor->Fitness) melhor = jogador2;
    }

    return melhor;
}

void salvarRedeNeural(){
    if(MODO_JOGO == MODO_DUELO) return;
    Jogador *jogador = obterMelhorJogador();
    RNA_SalvarRede(jogador->Cerebro, "RedeNeuralFile");
}

bool carregarRedeNeural(){
    RedeNeural *rede = NULL;
    rede = RNA_CarregarRede("RedeNeuralFile");

    if(rede == NULL) {
        printf("\n Nome da rede invalido");
        return false;
    }

    for(int i = 0; i < QTD_JOGOS; i++){
        baralhos[i].jogadores[0].alocarCerebro(rede);
        baralhos[i].jogadores[1].alocarCerebro(rede);
    }
    return true;
}

bool jogoRodando(){
    return JOGO_RODANDO;
}


int main( int argc, char* args[] ){
    srand(time(NULL));

    configuracoesIniciais();

    while(jogoRodando() && JogoRodando()){
        evento = GetEvento();

        if(TempoDecorrido(timerTeclado) > 0.3){
            verificarTeclado();
            verificarMouseJogador();
        }

        if(TempoDecorrido(timerJogadas) >= 1 || EXIBIR_ANIMACAO == false){
            realizarJogadaJogadores();
            verificarEstadoPartida();

            ReiniciaTimer(timerJogadas);
        }

        if(TempoDecorrido(timerDesenho) >= 0.01){
            desenhar();
            movimentarCamera();

            ReiniciaTimer(timerDesenho);
        }
    }

    salvarRedeNeural();

    FinalizaJogo();

    return 0;
}
