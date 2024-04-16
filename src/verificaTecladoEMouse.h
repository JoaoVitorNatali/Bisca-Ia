void verificarTeclado(){
    if(meuTeclado[PIG_TECLA_ENTER] || meuTeclado[PIG_TECLA_KP_ENTER]){
        JOGO_RODANDO = false;
    }

    if(meuTeclado[PIG_TECLA_x]){
        EXIBIR_ANIMACAO = !EXIBIR_ANIMACAO;
        ReiniciaTimer(timerTeclado);
    }
}

bool XeYEstaNaCarta(int xCarta, int yCarta, int xClique, int yClique){
    return XeYDentroDoPerimetroInformado(xCarta, yCarta, xClique, yClique, ALTURA_CARTA, LARGURA_CARTA);
}

bool XeYEstaNaCarta1(int x, int y){
    double xCarta = getXPrimeiraCarta(&baralhos[0]);
    double yCarta = getYCartasJogador(&baralhos[0], 0);
    return XeYEstaNaCarta(xCarta, yCarta, x, y);
}

bool XeYEstaNaCarta2(int x, int y){
    double xCarta = getXSegundaCarta(&baralhos[0]);
    double yCarta = getYCartasJogador(&baralhos[0], 0);
    return XeYEstaNaCarta(xCarta, yCarta, x, y);
}

bool XeYEstaNaCarta3(int x, int y){
    double xCarta = getXTerceiraCarta(&baralhos[0]);
    double yCarta = getYCartasJogador(&baralhos[0], 0);
    return XeYEstaNaCarta(xCarta, yCarta, x, y);
}

void jogadorSelecionarCarta(int indiceCarta){
    baralhos[0].jogadaUsuario(indiceCarta);
}

void verificarMouseJogador(){
    if(MODO_JOGO != MODO_DUELO) return;

    if(evento.mouse.acao == PIG_MOUSE_PRESSIONADO){
        int x = evento.mouse.posX;
        int y = evento.mouse.posY;

        if(XeYEstaNaCarta1(x, y)) jogadorSelecionarCarta(0);
        else if(XeYEstaNaCarta2(x, y)) jogadorSelecionarCarta(1);
        else if(XeYEstaNaCarta3(x, y)) jogadorSelecionarCarta(2);

        ReiniciaTimer(timerTeclado);
    }
}




void movimentarCamera(){
    double CameraZoom = GetAfastamentoCamera();
    int CameraPosX,CameraPosY;
    GetXYCamera(&CameraPosX, &CameraPosY);

    double A = 20.0/(CameraZoom+1);
    if(meuTeclado[PIG_TECLA_w] == 1){
        CameraPosY += A;
    }
    if(meuTeclado[PIG_TECLA_s] == 1){
        CameraPosY -= A;
    }
    if(meuTeclado[PIG_TECLA_a] == 1){
        CameraPosX -= A;
    }
    if(meuTeclado[PIG_TECLA_d] == 1){
        CameraPosX += A;
    }

    double B = 0.02;

    if(meuTeclado[PIG_TECLA_q] == 1){
        CameraZoom -= B;
        if(CameraZoom >= 50) CameraZoom = 50;
    }
    if(meuTeclado[PIG_TECLA_e] == 1){
        CameraZoom += B;
        if(CameraZoom <= -0.9999) CameraZoom = -0.9999;
    }

    SetAfastamentoCamera(CameraZoom);
    MoveCamera(CameraPosX, CameraPosY);
}
