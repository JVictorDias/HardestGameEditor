/********************************
A fun��o DefineFundo() permite escolher um arquivo de imagem como fundo (background) do jogo que ser� automaticamente desenhado a cada nova etapa de desenho na tela.
Par�metros:
nome_ArquivoImagem (entrada, passagem por refer�ncia*): indica o caminho relativo ou absoluto do arquivo de imagem que ser� utilizado como fundo do jogo.
� utilizado o operador * apenas por se tratar de um par�metro string. O valor do par�metro n�o � alterado dentro da fun��o.
********************************/
void DefinirFundo(char *nome_ArquivoImagem)
{
    jogo->DefineFundo(nome_ArquivoImagem);
}

/********************************
A fun��o GetTeclado() faz um mapeamento entre o teclado f�sico a uma vari�vel do tipo PIG_Teclado que funciona como um vetor de vari�veis l�gicas,
sendo cada posi��o do vetor relativo a uma tecla f�sica.
Uma lista das poss�veis teclas e seus c�digo pode ser vista no arquivo Tipos_PIG.h.
Retorno:
O valor de retorno da fun��o � um vetor de n�meros inteiros que indicam se uma determinada tecla (�ndice do vetor) foi
pressionada (valor diferente de 0,zero) ou n�o (valor igual a zero).
********************************/
PIG_Teclado PegarTeclado()
{
    return jogo->PegaTeclado();
}

/********************************
A fun��o CriaJogo() � respons�vel pela cria��o da janela do jogo e pelos componentes audio-visuais.
Ela deve ser chamada antes de qualquer outra fun��o da biblioetac PIG.h. N�o � poss�vel criar dois
jogos no mesmo progama. Logo, somente a primeira chamada ter� efeito.
Par�metros:
nome_Janela (entrada, passagem por refer�ncia*): indica o nome que a janela do jogo mostrar� no seu t�tulo.
� utilizado o operador * apenas por se tratar de um par�metro string. O valor do par�metro n�o � alterado dentro da fun��o.
cursor_Proprio (entrada, passagem por valor): indica se o jogo utilizar� cursores (ponteiros) pr�prios para o mouse ou utilizar� o cursor padr�o.
********************************/
void CriarJanela(char *nome_Janela,int cursor_Proprio=0)
{
    if (jogo==NULL){
        jogo = new CJogo(nome_Janela,cursor_Proprio);
        CAssetLoader::Inicia();
        if (cursor_Proprio!=0)
            CMouse::Inicia(jogo->renderer);
        CGerenciadorTimers::Inicia();
        CGerenciadorObjetos::Inicia(jogo->renderer);
        CGerenciadorParticulas::Inicia(jogo->renderer);
        CGerenciadorAnimacoes::Inicia(jogo->renderer);
        PIG_meuTeclado = PegarTeclado();
        PIG_JogoRodando = 1;
        srand(time(NULL));
    }
}

/********************************
A fun��o GetEvento() indica se houve algum tipo de evento desde a �ltima vez que a fun��o foi chamada.
Um evento pode ser uma tecla pressionada, um clique do mouse ou outra coisa relacionada � tela do jogo.
Maiores informa��e podem ser vistas no arquivo Tipo_PIG.h
Retorno:
O valor de retorno � um struct contendo o tipo de evento ocorrido e v�rios outros structs com informa��es espec�ficas.
********************************/
PIG_Evento PegarEvento()
{
    return jogo->PegaEvento();
}

/********************************
A fun��o IniciaDesenho() � respons�vel por preparar a janela do jogo para que os objetos gr�ficos
possam ser mostrados, al�m de desenhar a imagem de fundo do jogo, caso tenha sido definida.
Deve ser chamada repetidamente a cada novo quadro (frame) a ser desenhado.
********************************/
void IniciarDesenho()
{
    jogo->IniciaDesenho();
}

/********************************
A fun��o EncerraDesenho() � respons�vel por encerrar o desenho que foi criado, mostrando na tela do jogo.
Deve ser chamado sempre ao final do loop principal do jogo.
********************************/
void EncerrarDesenho()
{
    if (jogo->cursorPadrao!=0)
        CMouse::Desenha();
    jogo->EncerraDesenho();
}

/********************************
A fun��o PegarTecla reconhece a tecla apertada ou soltada(cima,baixo,esquerda,direita,enter,esc,z,x) e retorna um codigo.

Apertar = Positivo
Soltar = Negativo

********************************/

int PegarTecla()
{
    int i;

    if(PIGTeclasControle == 0)
    {
        if(PIG_evento.teclado.acao == 768)
        {
            for(i=0; i<SDL_NUM_SCANCODES; i++)
            {
                if(PIG_meuTeclado[i] == 1)
                {
                    PIGTeclasReturn = i;
                    PIGTeclasControle = 1;
                    return i;
                }
            }
            return 0;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        if(PIG_evento.teclado.acao == 769)
        {
            for(i=0; i<SDL_NUM_SCANCODES; i++)
            {
                if(PIGTeclasReturn == i && PIG_meuTeclado[i] == 0)
                {
                    PIGTeclasControle = 0;
                    return -i;
                }
            }
            return 0;
        }
        else
        {
            return 0;
        }
    }
}

int PegarBotao()
{
    static int Controle = 0;
    if(PIG_evento.mouse.acao == 1025)
    {
        Controle = 1;
    }
    else
    {
        if(PIG_evento.mouse.acao == 1026 && Controle == 1)
        {
            if(PIG_evento.mouse.botao == 1)
            {
                Controle = 0;
                return 1;
            }
            else
            {
                if(PIG_evento.mouse.botao == 2)
                {
                    Controle = 0;
                    return 2;
                }
                else
                {
                    Controle = 0;
                    return 3;
                }
            }
        }
    }
    return 0;
}

/********************************
A fun��o AtualizarEstadoJogo() � repons�vel por retornar se o jogo ainda est� ativo ou n�o.
O jogo ser� encerrado se a janela do jogo for fechada, por exemplo.
Retorno:
inteiro que indica se o jogo ainda est� sendo executado (valor diferente de zero) ou n�o (valor igual a 0, zero).
********************************/

int AtualizarJanela()
{
    PIG_evento = PegarEvento();
    if(PIG_JogoRodando != 0)
    {
        PIG_JogoRodando = jogo->GetRodando();
    }
    PIG_Tecla = PegarTecla();
    PIG_Botao = PegarBotao();
}

/********************************
A fun��o Espera() "congela" a tela do jogo por alguns milisegundos, impedindo que qualquer outro comando seja executado enquanto isso.
Par�metros:
tempo (entrada, passagem por valor): indica a quantidade de milisegundos durante a qual o jogo ser� pausado.
********************************/
void Esperar(int tempo)
{
    SDL_Delay(tempo);
}

/********************************
A fun��o FinalizaJogo() � respons�vel por encerrar com a PIG. Ap�s tudo o jogo ser executado
a fun��o deve ser chamada e ela ir� realizar a libera��o de mem�ria dos elementos criados pela PIG.
********************************/
void FinalizarJanela()
{
    CGerenciadorAnimacoes::Encerra();
    CGerenciadorParticulas::Encerra();
    CGerenciadorObjetos::Encerra();
    CGerenciadorTimers::Encerra();
    if (jogo->cursorPadrao==0)
        CMouse::Encerra();
    CAssetLoader::Encerra();
    delete jogo;
}

/********************************
A fun��o GetFPS() � respons�vel por calcular o valor do Frames Per Seconds (FPS), que representa a quantidade de
frames (quadros) desenhados por segundo pela PIG.
Retorno:
float que indica a m�dia de quadros desenhados por segundo.
********************************/
float PegarFPS()
{
    return jogo->GetFPS();
}

/********************************
A fun��o CarregaCursor() � respons�vel por armazenar um novo cursor para o mouse, dado pelo arquivo de imagem indicado.
O arquivo deve ser uma imagem com 32 pixels de altura orbigatoriamente.
Par�metro:
indice (entrada, passagem por valor): informa um �ndice para este novo cursor. Se o �ndice j� estiver sendo utilizado,
o cursor antigo � removido. O valor deve estar entre 0..MAX_CURSORES-1.
nomeArquivoCursor (entrada, passagem por refer�ncia*): indica o nome do arquivo de imagem que ser� utilziado como cursor do mouse.
� utilizado o operador * apenas por se tratar de um par�metro string. O valor do par�metro n�o � alterado dentro da fun��o.
********************************/
void CarregarCursor(int indice,char *nomeArquivoCursor)
{
    CMouse::CarregaCursor(indice,nomeArquivoCursor);
}

/********************************
A fun��o MudaCursor() � respons�vel por alterar o cursor atual do mouse por outro j� carregado.
Par�metro:
indice (entrada, passagem por valor): informa um �ndice para este novo cursor que j� deve ter sido
carregado anteriormente e deve estar no intervalo 0..MAX_CURSORES-1.
********************************/
void MudarCursor(int indice)
{
    CMouse::MudaCursor(indice);
}
