#define QUANTIDADE_JOGOS 100
#define QTD_PARTIDAS_ATE_EVOLUIR 4
#define ALTURA_CENARIO 600
#define LARGURA_CENARIO 820
#define LARGURA_JOGO    500
#define PADDING_JOGO    30
#define QTD_JOGOS_LINHA 5
#define MOSTRAR_CARTAS_ADVERSARIO 0

#define MODO_DUELO 1
#define MODO_EVOLUCAO 2
#define MODO_JOGO MODO_DUELO

int QTD_JOGOS = QUANTIDADE_JOGOS;

int SpriteFundo;
int SpriteBaralho;
int SpriteFundoCarta;
int SpritePlacar;
int SpriteRecomecar;
int SpriteGanhou;
int SpritePerdeu;

PIGEvento evento;          //evento ser tratado a cada passada do loop principal
PIGTeclado meuTeclado;     //variável como mapeamento do teclado
int timerDesenho;
int timerJogadas;
int timerTeclado;
int fonte;
int partida = 0;
int Geracao = 0;

bool JOGO_RODANDO = true;
bool EXIBIR_ANIMACAO = true;

double getRandomValue(){
    return (rand()%20001/10.0) - 1000.0;
}

double degToRad(){
    return M_PI / 180;
}

bool XeYDentroDoPerimetroInformado(int xPerimetro, int yPerimetro, int xValidar, int yValidar, int altura, int largura){
    int xPerimetroMax = xPerimetro + largura;
    int yPerimetroMax = yPerimetro + altura;

    bool xDentro = false;
    bool yDentro = false;

    if(xValidar >= xPerimetro && xValidar <= xPerimetroMax) xDentro = true;
    if(yValidar >= yPerimetro && yValidar <= yPerimetroMax) yDentro = true;

    return xDentro && yDentro;
}

void getXYRotacionado(double *X, double *Y, double xCentro, double yCentro, double Angulo){
    /**
        x' = h + (x-h)*cos(A)-(y-k)*sin(A)
        y' = k + (x-h)*sin(A)+(y-k)*cos(A)
        onde:

        (x', y') = são as novas coordenadas após a rotação
        (x, y) = são as coordenadas originais do ponto
        (h, k) = são as coordenadas do centro de rotação
        A = é o ângulo de rotação em radianos

    **/

    double X1 = *X;
    double Y1 = *Y;
    double H = xCentro;
    double K = yCentro;
    double AnguloRad = degToRad()*Angulo;
    double xLinha = H + (X1 - H) * cos(AnguloRad) - (Y1 - K) * sin(AnguloRad);
    double yLinha = K + (X1 - H) * sin(AnguloRad) + (Y1 - K) * cos(AnguloRad);

    *X = xLinha;
    *Y = yLinha;
}
