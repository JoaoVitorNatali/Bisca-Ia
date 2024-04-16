
int LARGURA_ORIGINAL_SPRITE = NULL;
int ALTURA_ORIGINAL_SPRITE = NULL;

double LARGURA_CARTA = (LARGURA_JOGO / 12) * 2;
double ALTURA_CARTA = (LARGURA_JOGO / 12) * 3;
double PADDING_CARTAS = (LARGURA_JOGO / 24);

void obterLarguraAlturaOriginalSprite(){
    GetDimensoesOriginaisSprite(SpriteBaralho, &ALTURA_ORIGINAL_SPRITE, &LARGURA_ORIGINAL_SPRITE);
}

int getPosicaoCartaSprite(int idCartaNormal){
    for(int i = 0; i < 13; i++){
        if(CARTAS_EM_ORDEM_SPRITE[i] == idCartaNormal) return i;
    }
}

int getIdCartaSprite(int carta, int naipe, bool esconderCarta = false){
    if(esconderCarta) return SpriteFundoCarta;

    int posCartaSprite = getPosicaoCartaSprite(carta);

    if(naipe == COPAS) return SPRITES_BARALHO_COPAS[posCartaSprite];
    if(naipe == OURO) return SPRITES_BARALHO_OURO[posCartaSprite];
    if(naipe == ESPADA) return SPRITES_BARALHO_ESPADA[posCartaSprite];
    if(naipe == PAUS) return SPRITES_BARALHO_PAUS[posCartaSprite];
}

double getXPrimeiraCarta(Baralho *baralho){
    double centroJogo = (LARGURA_JOGO / 2) + baralho->x;
    double umaCartaEMeia = LARGURA_CARTA + (LARGURA_CARTA/2) + PADDING_CARTAS;
    return centroJogo - umaCartaEMeia;
}

double getXSegundaCarta(Baralho *baralho){
    double centroJogo = (LARGURA_JOGO / 2) + baralho->x;
    double meiaCarta = LARGURA_CARTA / 2;
    return centroJogo - meiaCarta;
}

double getXTerceiraCarta(Baralho *baralho){
    double centroJogo = (LARGURA_JOGO / 2) + baralho->x;
    double meiaCartaEPadding = (LARGURA_CARTA/2) + PADDING_CARTAS;
    return centroJogo + meiaCartaEPadding;
}

void desenharCarta(int carta, int naipe, double x, double y, bool esconderCarta = false, double angulo = 0){
    if(carta == 0) return;

    int idSprite = getIdCartaSprite(carta, naipe, esconderCarta);

    MoveSprite(idSprite, x, y);
    SetDimensoesSprite(idSprite, ALTURA_CARTA, LARGURA_CARTA);
    SetAnguloSprite(idSprite, 0);
    SetAnguloSprite(idSprite, angulo);
    DesenhaSprite(idSprite);
}

double getYCartasJogador(Baralho *baralho, int idJogador){
    double paddingBordas = LARGURA_JOGO / 12;
    if(idJogador == 0){
        return baralho->y + paddingBordas;
    } else {
        double topoJogo = baralho->y + LARGURA_JOGO;
        return topoJogo - ALTURA_CARTA - paddingBordas;
    }
}

void desenharCartasJogador(Baralho *baralho, int idJogador, bool esconderCarta = false){
    double alturaCarta = getYCartasJogador(baralho, idJogador);

    std::vector<int> carta1, carta2, carta3;
    carta1 = baralho->jogadores[idJogador].getMao()[0];
    carta2 = baralho->jogadores[idJogador].getMao()[1];
    carta3 = baralho->jogadores[idJogador].getMao()[2];

    desenharCarta(carta1[0], carta1[1], getXPrimeiraCarta(baralho), alturaCarta, esconderCarta);
    desenharCarta(carta2[0], carta2[1], getXSegundaCarta(baralho), alturaCarta, esconderCarta);
    desenharCarta(carta3[0], carta3[1], getXTerceiraCarta(baralho), alturaCarta, esconderCarta);
}

void desenharCartasJogadores(Baralho *baralho){
    desenharCartasJogador(baralho, 0); /// desenha na parte de baixo
    bool esconderCarta = MODO_JOGO == MODO_DUELO;
    if(MOSTRAR_CARTAS_ADVERSARIO == 1) esconderCarta = false;
    desenharCartasJogador(baralho, 1, esconderCarta); /// desenha na parte de cima
}

void desenharCartasMesa(Baralho *baralho){
    double x1, x2, altura;

    /// MEIO DO JOGO - ALTURA DE MEIA CARTA
    altura = (LARGURA_JOGO / 2) - (ALTURA_CARTA / 2);
    /// MEIO DO JOGO - LARGURA CARTA - PADDING CARTA
    x1 = (LARGURA_JOGO / 2) - LARGURA_CARTA - PADDING_CARTAS;
    /// MEIO DO JOGO + PADDING CARTA
    x2 = (LARGURA_JOGO / 2) + PADDING_CARTAS;

    int carta1 = baralho->cartasMesa[0][0];
    int naipe1 = baralho->cartasMesa[0][1];
    int carta2 = baralho->cartasMesa[1][0];
    int naipe2 = baralho->cartasMesa[1][1];

    desenharCarta(carta1, naipe1, x1, altura);
    desenharCarta(carta2, naipe2, x2, altura);
}

void desenhaFundo(int x, int y){
    MoveSprite(SpriteFundo, x, y);
    SetDimensoesSprite(SpriteFundo, LARGURA_JOGO, LARGURA_JOGO);
    DesenhaSprite(SpriteFundo);
}

void desenhaTrunfo(Baralho *baralho){
    if(!baralho->trunfoEstaNoBaralho) return;

    double x, y;
    x = baralho->x + PADDING_CARTAS;
    y = baralho->y + (LARGURA_JOGO / 2) - ((ALTURA_CARTA / 3) * 2);

    int carta = baralho->trunfo[0];
    int naipe = baralho->trunfo[1];

    desenharCarta(carta, naipe, x, y, false, 0);
}

void desenharCartasBaralhoETrunfo(Baralho *baralho){
    desenhaTrunfo(baralho);

    double x = baralho->x + PADDING_CARTAS;
    double y = baralho->y + (LARGURA_JOGO / 2);

    for(int i = 0; i < baralho->cartas.size(); i+=6){
        desenharCarta(1, 1, x, y, true, 0);
        x += 3;
    }
}

void desenharPontosJogador(Baralho *baralho, int idJogador){
    int pontosJogador = baralho->pontos[idJogador];

    double posYJogador = getYCartasJogador(baralho, idJogador);
    double umTercoCarta = ALTURA_CARTA /3;

    double posYCarta = posYJogador + umTercoCarta;
    double posXCarta = getXTerceiraCarta(baralho) + LARGURA_CARTA + PADDING_CARTAS;

    double larguraPlacar = (LARGURA_JOGO / 12) + (LARGURA_JOGO/48);
    MoveSprite(SpritePlacar, posXCarta, posYCarta);
    SetDimensoesSprite(SpritePlacar, larguraPlacar, larguraPlacar);
    DesenhaSprite(SpritePlacar);

    double posXPonto = posXCarta + (larguraPlacar/2);
    double posYPonto = posYCarta + (larguraPlacar/4);
    EscreveInteiroCentralizado(pontosJogador, posXPonto, posYPonto, PRETO, fonte);
}

void desenharJogo(int i){
    Baralho *baralho = &baralhos[i];

    desenhaFundo(baralho->x, baralho->y);
    desenharCartasJogadores(baralho);
    desenharCartasMesa(baralho);
    desenharCartasBaralhoETrunfo(baralho);
    desenharPontosJogador(baralho, 0);
    desenharPontosJogador(baralho, 1);
}

void desenharJogos(){
    if(LARGURA_ORIGINAL_SPRITE == NULL) obterLarguraAlturaOriginalSprite();

    for(int i = 0; i < QTD_JOGOS; i++){
        desenharJogo(i);
    }
}
