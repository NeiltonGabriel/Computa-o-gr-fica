#include <GL/glut.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <mmsystem.h>

#define PI 3.14159265
#define PREENCHIDO 1
#define BORDA 0

#define FONTE_TEXTO_PADRAO GLUT_BITMAP_HELVETICA_18
#define FONTE_TEXTO_GRANDE GLUT_BITMAP_TIMES_ROMAN_24

const char* ARQ_SOM_AMBIENTE = "ambiente.wav";
const char* ARQ_SOM_GOL = "gol.wav";
const char* ARQ_SOM_INTERCEPTACAO = "interceptacao.wav";
const char* ARQ_SOM_DEFESA = "defesa.wav";
const char* ARQ_SOM_DISPARADA = "disparada.wav";
const char* ARQ_SOM_BOLA_ANDANDO = "bola_andando.wav";
const char* ARQ_SOM_CHAPEU = "chapeu.wav";
const char* ARQ_SOM_TORCIDA_VIBRA = "torcida.wav";
const char* ARQ_SOM_APITO = "apito.wav";

float VOL_AMBIENTE = 0.5f;
float VOL_GOL = 1.0f;
float VOL_INTERCEPTACAO = 0.8f;
float VOL_DEFESA = 0.8f;
float VOL_DISPARADA = 0.7f;
float VOL_BOLA_ANDANDO = 0.4f;
float VOL_CHAPEU = 0.9f;
float VOL_TORCIDA_VIBRA = 1.0f;
float VOL_APITO = 1.0f;

const float COR_TIME_ESQ_R = 0.2f;
const float COR_TIME_ESQ_G = 0.2f;
const float COR_TIME_ESQ_B = 1.0f;

const float COR_TIME_DIR_R = 1.0f;
const float COR_TIME_DIR_G = 0.2f;
const float COR_TIME_DIR_B = 0.2f;

const float VEL_GK = 1.5f;
const float VEL_DEF = 2.8f;
const float VEL_MID = 3.5f;
const float VEL_ATK = 4.0f;

const float PELE_1_R = 1.0f;
const float PELE_1_G = 0.8f;
const float PELE_1_B = 0.6f;
const float PELE_2_R = 0.8f;
const float PELE_2_G = 0.6f;
const float PELE_2_B = 0.4f;
const float PELE_3_R = 0.4f;
const float PELE_3_G = 0.2f;
const float PELE_3_B = 0.1f;

const float ESPESSURA_CONTORNO_BOLA = 2.5f;
#define TIME_INICIAL_DIREITA 1

const float TELA_LIMITE_X = 1000.0f;
const float TELA_LIMITE_Y = 600.0f;
const float COR_FUNDO_R = 0.2f;
const float COR_FUNDO_G = 0.2f;
const float COR_FUNDO_B = 0.2f;

const float OFFSET_CAMPO_X = 0.0f;
const float OFFSET_CAMPO_Y = -125.0f;

const float PLACAR_LARGURA = 250.0f;
const float PLACAR_ALTURA = 240.0f;
const float DIGITO_LARGURA = 85.0f;
const float DIGITO_ALTURA = 170.0f;
const float DIGITO_ESPESSURA = 10.0f;
const float PLACAR_POS_Y = 150.0f;

const float TEMPO_POS_X = 842.5f;
const float TEMPO_POS_Y = -526.0f;
const float TEMPO_LARGURA = 265.0f;
const float TEMPO_ALTURA = 80.0f;

const float OPCOES_POS_X = 710.0f;
const float OPCOES_POS_Y = -476.0f;
const float OPCOES_LARGURA = 265.0f;
const float OPCOES_ALTURA = 80.0f;
const char* TEXTO_OPCOES = "OPCOES";
const char* TEXTO_OP_1 = "1 minuto";
const char* TEXTO_OP_2 = "3 minutos";
const char* TEXTO_OP_3 = "5 minutos";
const char* TEXTO_OP_R = "Reiniciar";
const char* TEXTO_OP_S = "Sair";

const float CONTROLES_POS_X = -980.0f;
const float CONTROLES_POS_Y = -567.5f;
const float CONTROLES_LARGURA = 265.0f;
const float CONTROLES_ALTURA = 150.0f;
const char* TEXTO_CTRL_TITULO = "CONTROLES";
const char* TEXTO_CTRL_1 = "WASD/Setas: Mover";
const char* TEXTO_CTRL_2 = "ESPACO: Pular";

const float ESCALA_MAX_CHAPEU = 1.75f;

const int QTD_FAIXAS_GRAMA = 10;
const float GRAMA_CLARA_R = 0.133f;
const float GRAMA_CLARA_G = 0.545f;
const float GRAMA_CLARA_B = 0.133f;
const float GRAMA_ESCURA_R = 0.100f;
const float GRAMA_ESCURA_G = 0.450f;
const float GRAMA_ESCURA_B = 0.100f;

#define ESCALA 13.0f
#define CAMPO_C (105.0f * ESCALA)
#define CAMPO_L (68.0f * ESCALA)

const float RAIO_CENTRO = 9.15f * ESCALA;
const float AREA_G_C = 16.5f * ESCALA;
const float AREA_G_L = 40.3f * ESCALA;
const float AREA_P_C = 5.5f * ESCALA;
const float AREA_P_L = 18.32f * ESCALA;
const float GOL_L = 7.32f * ESCALA;
const float GOL_PROF = 2.4f * ESCALA;
const float MARCA_PENALTI = 11.0f * ESCALA;
const float RAIO_ESCANTEIO = 2.0f * ESCALA;

const float RAIO_DEFESA_GK = 47.5f;
const float DISTANCIA_MINIMA_COBRANCA = 39.0f;

const float RAIO_PONTO_CENTRAL = 0.35f * ESCALA;
const float RAIO_MARCA_PENALTI_ESQ = 0.35f * ESCALA;
const float RAIO_MARCA_PENALTI_DIR = 0.35f * ESCALA;

const float MEIO_C = CAMPO_C / 2.0f; 
const float MEIO_L = CAMPO_L / 2.0f; 

typedef enum {
    EVENTO_NENHUM,
    EVENTO_GOL_ESQ,
    EVENTO_GOL_DIR,
    EVENTO_LATERAL_CIMA,
    EVENTO_LATERAL_BAIXO,
    EVENTO_ESCANTEIO_ESQ_CIMA,
    EVENTO_ESCANTEIO_ESQ_BAIXO,
    EVENTO_ESCANTEIO_DIR_CIMA,
    EVENTO_ESCANTEIO_DIR_BAIXO,
    EVENTO_TOMADA_BOLA,
    EVENTO_INTERVALO,
    EVENTO_FIM_JOGO
} TipoEvento;

typedef struct {
    int id;
    int time;
    int role;
    float x, y;
    float homeX, homeY;
    float speed;
    float skinR, skinG, skinB;
    float animOffset;
    float angle;
} Jogador;

Jogador jogadores[22];

int placarEsq = 0;
int placarDir = 0;

float bolaX = 0.0f;
float bolaY = 0.0f;
float bolaVX = 0.0f;
float bolaVY = 0.0f;
float velAnterior = 0.0f;

const float BOLA_RAIO = 0.6f * ESCALA;
const float BOLA_ACEL = 0.35f;       
const float BOLA_ATRITO = 0.95f;    
const float BOLA_MAX_VEL = 20.0f;   

const float TEMPO_CHAPEU_SEC = 1.5f;
const float DURACAO_CHAPEU_SEC = 0.5f;
float progressoChapeu = 0.0f;
bool executandoChapeu = false;
float tempoAtualChapeu = 0.0f;

bool teclaW = false, teclaA = false, teclaS = false, teclaD = false;
bool teclaI = false, teclaJ = false, teclaK = false, teclaL = false;
bool teclaCima = false, teclaBaixo = false, teclaEsq = false, teclaDir = false;

bool inputsBloqueados = false;
bool bolaEmJogo = true;
float ultimaSaidaX = 0.0f;
float ultimaSaidaY = 0.0f;
// Constantes para os limites do Botão
#define BOTAO_X_MIN (TELA_LIMITE_X - 90.0f)
#define BOTAO_X_MAX (TELA_LIMITE_X - 30.0f)
#define BOTAO_Y_MIN (TELA_LIMITE_Y - 80.0f)
#define BOTAO_Y_MAX (TELA_LIMITE_Y - 20.0f)

void desenhaBotaoSair() {
    // Fundo vermelho escuro
    glColor3f(0.8f, 0.2f, 0.2f);
    glRectf(BOTAO_X_MIN, BOTAO_Y_MIN, BOTAO_X_MAX, BOTAO_Y_MAX);

    // Borda branca
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(BOTAO_X_MIN, BOTAO_Y_MIN);
        glVertex2f(BOTAO_X_MAX, BOTAO_Y_MIN);
        glVertex2f(BOTAO_X_MAX, BOTAO_Y_MAX);
        glVertex2f(BOTAO_X_MIN, BOTAO_Y_MAX);
    glEnd();

    // Desenhando o "X" branco no meio do botão
    glLineWidth(4.0f);
    glBegin(GL_LINES);
        glVertex2f(BOTAO_X_MIN + 20, BOTAO_Y_MIN + 15);
        glVertex2f(BOTAO_X_MAX - 20, BOTAO_Y_MAX - 15);
        
        glVertex2f(BOTAO_X_MIN + 20, BOTAO_Y_MAX - 15);
        glVertex2f(BOTAO_X_MAX - 20, BOTAO_Y_MIN + 15);
    glEnd();
    glLineWidth(2.0f); // Reseta a espessura da linha
}

int tempoOpcoes[3] = {1, 3, 5};
int tempoPartidaMinutos = 3;
float tempoJogoVirtualSegundos = 0.0f;
bool cronometroRodando = false;
bool menuAberto = false;
bool segundoTempo = false;
bool fimDeJogo = false;
bool gramaInvertida = false;
bool posseDireita = TIME_INICIAL_DIREITA;
bool defendendoGolContra = false;
bool esperandoCobranca = false;

void initJogadores() {
    float sX[11] = { -MEIO_C + 20.0f, -MEIO_C + 180.0f, -MEIO_C + 180.0f, -MEIO_C + 180.0f, -MEIO_C + 180.0f, -MEIO_C + 350.0f, -MEIO_C + 350.0f, -MEIO_C + 350.0f, -MEIO_C + 350.0f, -80.0f, -80.0f };
    float sY[11] = { 0.0f, -250.0f, -80.0f, 80.0f, 250.0f, -250.0f, -80.0f, 80.0f, 250.0f, -100.0f, 100.0f };
    int sRole[11] = { 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3 };

    for(int i = 0; i < 11; i++) {
        jogadores[i].id = i;
        jogadores[i].time = 0;
        jogadores[i].homeX = sX[i];
        jogadores[i].homeY = sY[i];
        jogadores[i].x = sX[i];
        jogadores[i].y = sY[i];
        jogadores[i].role = sRole[i];
        float baseSpeed = (sRole[i]==0)?VEL_GK:(sRole[i]==1)?VEL_DEF:(sRole[i]==2)?VEL_MID:VEL_ATK;
        jogadores[i].speed = baseSpeed + (i * 0.05f);
        int s = i % 3;
        jogadores[i].skinR = s==0?PELE_1_R:(s==1?PELE_2_R:PELE_3_R);
        jogadores[i].skinG = s==0?PELE_1_G:(s==1?PELE_2_G:PELE_3_G);
        jogadores[i].skinB = s==0?PELE_1_B:(s==1?PELE_2_B:PELE_3_B);
        jogadores[i].animOffset = (float)i;
        jogadores[i].angle = 0.0f;

        int j = i + 11;
        jogadores[j].id = j;
        jogadores[j].time = 1;
        jogadores[j].homeX = -sX[i];
        jogadores[j].homeY = sY[i];
        jogadores[j].x = -sX[i];
        jogadores[j].y = sY[i];
        jogadores[j].role = sRole[i];
        baseSpeed = (sRole[i]==0)?VEL_GK:(sRole[i]==1)?VEL_DEF:(sRole[i]==2)?VEL_MID:VEL_ATK;
        jogadores[j].speed = baseSpeed + (j * 0.05f);
        s = j % 3;
        jogadores[j].skinR = s==0?PELE_1_R:(s==1?PELE_2_R:PELE_3_R);
        jogadores[j].skinG = s==0?PELE_1_G:(s==1?PELE_2_G:PELE_3_G);
        jogadores[j].skinB = s==0?PELE_1_B:(s==1?PELE_2_B:PELE_3_B);
        jogadores[j].animOffset = (float)j;
        jogadores[j].angle = 180.0f;
    }
}

void tocarSomArquivo(const char* arquivo, float volume) {
    char comando[256];
    sprintf(comando, "setaudio %s volume to %d", arquivo, (int)(volume * 1000));
    mciSendString(comando, NULL, 0, NULL);
    sprintf(comando, "play %s from 0", arquivo);
    mciSendString(comando, NULL, 0, NULL);
}

void atualizarSomBola(float velocidade) {
    int vol = (int)((velocidade / BOLA_MAX_VEL) * VOL_BOLA_ANDANDO * 1000);
    char comando[256];
    sprintf(comando, "setaudio %s volume to %d", ARQ_SOM_BOLA_ANDANDO, vol);
    mciSendString(comando, NULL, 0, NULL);
    if (velocidade > 0.5f) {
        char cmdPlay[256];
        sprintf(cmdPlay, "play %s", ARQ_SOM_BOLA_ANDANDO);
        mciSendString(cmdPlay, NULL, 0, NULL);
    } else {
        char cmdPause[256];
        sprintf(cmdPause, "pause %s", ARQ_SOM_BOLA_ANDANDO);
        mciSendString(cmdPause, NULL, 0, NULL);
    }
}

void init() {
    glClearColor(COR_FUNDO_R, COR_FUNDO_G, COR_FUNDO_B, 1.0f); 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-TELA_LIMITE_X, TELA_LIMITE_X, -TELA_LIMITE_Y, TELA_LIMITE_Y);
    
    initJogadores();

    char comandoAbre[256];
    sprintf(comandoAbre, "open %s alias ambiente", ARQ_SOM_AMBIENTE);
    mciSendString(comandoAbre, NULL, 0, NULL);
    char comando[256];
    sprintf(comando, "setaudio ambiente volume to %d", (int)(VOL_AMBIENTE * 1000));
    mciSendString(comando, NULL, 0, NULL);
    mciSendString("play ambiente repeat", NULL, 0, NULL);
}

void desenhaTextoPersonalizado(const char* texto, float x, float y, void* fonte) {
    glRasterPos2f(x, y);
    for (int i = 0; i < strlen(texto); i++) {
        glutBitmapCharacter(fonte, texto[i]);
    }
}

void desenhaTextoDireita(const char* texto, float limite_x, float y, void* fonte) {
    float width = 0;
    for (int i = 0; i < strlen(texto); i++) {
        width += glutBitmapWidth(fonte, texto[i]);
    }
    glRasterPos2f(limite_x - width, y);
    for (int i = 0; i < strlen(texto); i++) {
        glutBitmapCharacter(fonte, texto[i]);
    }
}

void desenhaCirculo(float x_centro, float y_centro, float raio, int segmentos, float angulo_inicio, float angulo_fim, int caminho) {
    if (caminho == BORDA) {
        glBegin(GL_LINE_STRIP);
    } else {
        glBegin(GL_POLYGON);
    }
    for (int i = 0; i <= segmentos; i++) {
        float angulo = (angulo_inicio + (angulo_fim - angulo_inicio) * i / segmentos) * PI / 180.0;
        glVertex2f(x_centro + raio * cos(angulo), y_centro + raio * sin(angulo));
    }
    glEnd();
}

void desenhaRede(float x_min, float x_max, float y_min, float y_max) {
    glColor3f(0.8f, 0.8f, 0.8f);
    glLineWidth(1.0f);
    
    float passoX = (x_max - x_min) / 4.0f; 
    float passoY = (y_max - y_min) / 10.0f; 
    
    glBegin(GL_LINE_LOOP);
    for (float x = x_min; x < x_max - 0.1f; x += passoX) {
        for (float y = y_min; y < y_max - 0.1f; y += passoY) {
            float cx = x + passoX / 2.0f;
            float cy = y + passoY / 2.0f;
            
            glBegin(GL_LINE_LOOP);
                glVertex2f(cx, y + passoY);
                glVertex2f(x + passoX, cy);
                glVertex2f(cx, y);
                glVertex2f(x, cy);
            glEnd();
        }
    }
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.0f);
}

void desenhaSegmento(int segmento, float x, float y, float w, float h, float t) {
    glBegin(GL_POLYGON);
    switch (segmento) {
        case 0: glVertex2f(x, y+h); glVertex2f(x+w, y+h); glVertex2f(x+w-t, y+h-t); glVertex2f(x+t, y+h-t); break;
        case 1: glVertex2f(x+w, y+h); glVertex2f(x+w, y+h/2); glVertex2f(x+w-t, y+h/2+t/2); glVertex2f(x+w-t, y+h-t); break;
        case 2: glVertex2f(x+w, y+h/2); glVertex2f(x+w, y); glVertex2f(x+w-t, y+t); glVertex2f(x+w-t, y+h/2-t/2); break;
        case 3: glVertex2f(x, y); glVertex2f(x+w, y); glVertex2f(x+w-t, y+t); glVertex2f(x+t, y+t); break;
        case 4: glVertex2f(x, y+h/2); glVertex2f(x, y); glVertex2f(x+t, y+t); glVertex2f(x+t, y+h/2-t/2); break;
        case 5: glVertex2f(x, y+h); glVertex2f(x, y+h/2); glVertex2f(x+t, y+h/2+t/2); glVertex2f(x+t, y+h-t); break;
        case 6: glVertex2f(x+t/2, y+h/2+t/2); glVertex2f(x+w-t/2, y+h/2+t/2); glVertex2f(x+w-t/2, y+h/2-t/2); glVertex2f(x+t/2, y+h/2-t/2); break;
    }
    glEnd();
}

void desenhaDigito(int numero, float x, float y, float w, float h, float t) {
    int segmentos[10][7] = {
        {1,1,1,1,1,1,0}, {0,1,1,0,0,0,0}, {1,1,0,1,1,0,1}, {1,1,1,1,0,0,1}, {0,1,1,0,0,1,1}, 
        {1,0,1,1,0,1,1}, {1,0,1,1,1,1,1}, {1,1,1,0,0,0,0}, {1,1,1,1,1,1,1}, {1,1,1,1,0,1,1}
    };
    glColor3f(1.0f, 0.0f, 0.0f);
    for (int i = 0; i < 7; i++) {
        if (segmentos[numero][i]) desenhaSegmento(i, x, y, w, h, t);
    }
}

void desenhaOutdoor(float centro_x, float centro_y, int pontuacao) {
    glColor3f(0.1f, 0.1f, 0.1f);
    glRectf(centro_x - PLACAR_LARGURA/2, centro_y - PLACAR_ALTURA/2, 
            centro_x + PLACAR_LARGURA/2, centro_y + PLACAR_ALTURA/2);
            
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(centro_x - PLACAR_LARGURA/2, centro_y - PLACAR_ALTURA/2);
        glVertex2f(centro_x + PLACAR_LARGURA/2, centro_y - PLACAR_ALTURA/2);
        glVertex2f(centro_x + PLACAR_LARGURA/2, centro_y + PLACAR_ALTURA/2);
        glVertex2f(centro_x - PLACAR_LARGURA/2, centro_y + PLACAR_ALTURA/2);
    glEnd();

    float espaco = 20.0f;
    float y_base = centro_y - (DIGITO_ALTURA / 2.0f);
    
    float x_dezena = centro_x - (DIGITO_LARGURA + espaco/2.0f);
    float x_unidade = centro_x + (espaco/2.0f);

    desenhaDigito(pontuacao / 10, x_dezena, y_base, DIGITO_LARGURA, DIGITO_ALTURA, DIGITO_ESPESSURA);
    desenhaDigito(pontuacao % 10, x_unidade, y_base, DIGITO_LARGURA, DIGITO_ALTURA, DIGITO_ESPESSURA);
}

<<<<<<< HEAD
/*void desenhaTodosPlacares() {
=======
void desenhaTempo(float centro_x, float centro_y) {
    glColor3f(0.8f, 0.1f, 0.1f);
    glRectf(centro_x - TEMPO_LARGURA/2.0f, centro_y - TEMPO_ALTURA/2.0f, centro_x + TEMPO_LARGURA/2.0f, centro_y + TEMPO_ALTURA/2.0f);
    
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(centro_x - TEMPO_LARGURA/2.0f, centro_y - TEMPO_ALTURA/2.0f);
        glVertex2f(centro_x + TEMPO_LARGURA/2.0f, centro_y - TEMPO_ALTURA/2.0f);
        glVertex2f(centro_x + TEMPO_LARGURA/2.0f, centro_y + TEMPO_ALTURA/2.0f);
        glVertex2f(centro_x - TEMPO_LARGURA/2.0f, centro_y + TEMPO_ALTURA/2.0f);
    glEnd();

    int min = (int)tempoJogoVirtualSegundos / 60;
    int sec = (int)tempoJogoVirtualSegundos % 60;
    char tempoStr[10];
    sprintf(tempoStr, "%02d:%02d", min, sec);
    
    desenhaTextoPersonalizado(tempoStr, centro_x - 30.0f, centro_y - 10.0f, FONTE_TEXTO_GRANDE);
}

void desenhaComandos() {
    float x1 = CONTROLES_POS_X;
    float y1 = CONTROLES_POS_Y;
    float x2 = CONTROLES_POS_X + CONTROLES_LARGURA;
    float y2 = CONTROLES_POS_Y + CONTROLES_ALTURA;

    glColor3f(0.1f, 0.2f, 0.4f);
    glRectf(x1, y1, x2, y2);
    
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x1, y1); glVertex2f(x2, y1);
        glVertex2f(x2, y2); glVertex2f(x1, y2);
    glEnd();

    desenhaTextoPersonalizado(TEXTO_CTRL_TITULO, x1 + 20.0f, y2 - 40.0f, FONTE_TEXTO_PADRAO);
    desenhaTextoPersonalizado(TEXTO_CTRL_1, x1 + 20.0f, y2 - 90.0f, FONTE_TEXTO_PADRAO);
    desenhaTextoPersonalizado(TEXTO_CTRL_2, x1 + 20.0f, y2 - 130.0f, FONTE_TEXTO_PADRAO);
}

void desenhaBotaoConfig() {
    float btnX1 = OPCOES_POS_X;
    float btnX2 = OPCOES_POS_X + OPCOES_LARGURA;
    float btnY1 = OPCOES_POS_Y;
    float btnY2 = OPCOES_POS_Y + OPCOES_ALTURA;

    glColor3f(0.7f, 0.7f, 0.1f);
    glRectf(btnX1, btnY1, btnX2, btnY2);
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(btnX1, btnY1); glVertex2f(btnX2, btnY1);
        glVertex2f(btnX2, btnY2); glVertex2f(btnX1, btnY2);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    desenhaTextoDireita(TEXTO_OPCOES, btnX2 - 105, btnY1 + (OPCOES_ALTURA/2.0f) - 6.0f, FONTE_TEXTO_PADRAO);

    if (menuAberto) {
        float menuBase = btnY2 + 10.0f;
        float hItem = 42.0f;
        
        glColor3f(0.2f, 0.2f, 0.2f);
        glRectf(btnX1, menuBase, btnX2, menuBase + (hItem * 5));
        
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_LINE_LOOP);
            glVertex2f(btnX1, menuBase); glVertex2f(btnX2, menuBase);
            glVertex2f(btnX2, menuBase + (hItem * 5)); glVertex2f(btnX1, menuBase + (hItem * 5));
        glEnd();

        char opt[32];
        sprintf(opt, "%s %s", (tempoPartidaMinutos == tempoOpcoes[0]) ? "[X]" : "", TEXTO_OP_1);
        desenhaTextoDireita(opt, btnX2 - 105.0f, menuBase + (hItem * 0) + (hItem/2.0f) - 6.0f, FONTE_TEXTO_PADRAO);
        
        sprintf(opt, "%s %s", (tempoPartidaMinutos == tempoOpcoes[1]) ? "[X]" : "", TEXTO_OP_2);
        desenhaTextoDireita(opt, btnX2 - 105.0f, menuBase + (hItem * 1) + (hItem/2.0f) - 6.0f, FONTE_TEXTO_PADRAO);
        
        sprintf(opt, "%s %s", (tempoPartidaMinutos == tempoOpcoes[2]) ? "[X]" : "", TEXTO_OP_3);
        desenhaTextoDireita(opt, btnX2 - 105.0f, menuBase + (hItem * 2) + (hItem/2.0f) - 6.0f, FONTE_TEXTO_PADRAO);
        
        desenhaTextoDireita(TEXTO_OP_R, btnX2 - 105.0f, menuBase + (hItem * 3) + (hItem/2.0f) - 6.0f, FONTE_TEXTO_PADRAO);
        desenhaTextoDireita(TEXTO_OP_S, btnX2 - 120.0f, menuBase + (hItem * 4) + (hItem/2.0f) - 6.0f, FONTE_TEXTO_PADRAO);
    }
}

void desenhaTodosPlacares() {
>>>>>>> origin/mecanica
    float x_outdoor_esq = -TELA_LIMITE_X + (PLACAR_LARGURA / 2.0f) + 20.0f;
    desenhaOutdoor(x_outdoor_esq, PLACAR_POS_Y, placarEsq);

    float x_outdoor_dir = TELA_LIMITE_X - (PLACAR_LARGURA / 2.0f) - 20.0f;
    desenhaOutdoor(x_outdoor_dir, PLACAR_POS_Y, placarDir);
<<<<<<< HEAD
}*/
void desenhaPlacarCentral() {
    float centro_x = 0.0f; 
    float centro_y = TELA_LIMITE_Y - 120.0f; 
    float largura = 400.0f;
    float altura = 120.0f;

    // NOVO: Estrutura da cabine que desce do teto e tampa a arquibancada
    glColor3f(0.12f, 0.12f, 0.12f); 
    glRectf(centro_x - largura/2 - 30.0f, centro_y - altura/2 - 20.0f, 
            centro_x + largura/2 + 30.0f, TELA_LIMITE_Y);
            
    // Borda da cabine
    glColor3f(0.3f, 0.3f, 0.3f);
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(centro_x - largura/2 - 30.0f, centro_y - altura/2 - 20.0f);
        glVertex2f(centro_x + largura/2 + 30.0f, centro_y - altura/2 - 20.0f);
        glVertex2f(centro_x + largura/2 + 30.0f, TELA_LIMITE_Y);
        glVertex2f(centro_x - largura/2 - 30.0f, TELA_LIMITE_Y);
    glEnd();

    // A partir daqui é o placar que você já tinha:
    // Fundo do placar unificado
    glColor3f(0.05f, 0.05f, 0.05f);
    glRectf(centro_x - largura/2, centro_y - altura/2, 
            centro_x + largura/2, centro_y + altura/2);
            
    // Borda do placar principal
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(centro_x - largura/2, centro_y - altura/2);
        glVertex2f(centro_x + largura/2, centro_y - altura/2);
        glVertex2f(centro_x + largura/2, centro_y + altura/2);
        glVertex2f(centro_x - largura/2, centro_y + altura/2);
    glEnd();

    float dig_w = 40.0f, dig_h = 80.0f, dig_t = 8.0f, espaco = 15.0f;
    float y_base = centro_y - (dig_h / 2.0f);

    float x_esq_dez = centro_x - 120.0f - dig_w - espaco/2.0f;
    float x_esq_uni = centro_x - 120.0f + espaco/2.0f;
    desenhaDigito(placarEsq / 10, x_esq_dez, y_base, dig_w, dig_h, dig_t);
    desenhaDigito(placarEsq % 10, x_esq_uni, y_base, dig_w, dig_h, dig_t);

    glColor3f(1.0f, 1.0f, 1.0f);
    glRectf(centro_x - 15.0f, centro_y - 5.0f, centro_x + 15.0f, centro_y + 5.0f);

    float x_dir_dez = centro_x + 120.0f - dig_w - espaco/2.0f;
    float x_dir_uni = centro_x + 120.0f + espaco/2.0f;
    desenhaDigito(placarDir / 10, x_dir_dez, y_base, dig_w, dig_h, dig_t);
    desenhaDigito(placarDir % 10, x_dir_uni, y_base, dig_w, dig_h, dig_t);
}
void desenhaArquibancada() {
    float margem = 4.0f * ESCALA;
    float larguraDegrau = 3.5f * ESCALA;
    
    // Deixei 10 para a arquibancada acabar mais cedo e sobrar o cinza liso no fundo
    int numDegraus = 10; 

    float limiteGeralX = MEIO_C + margem + (numDegraus * larguraDegrau);
    float limiteGeralY = MEIO_L + margem + (numDegraus * larguraDegrau);

    // 1. Fundo base gigantão (A área cinza escuro onde NÃO terá NPCs)
    glColor3f(0.2f, 0.2f, 0.2f);
    glRectf(-TELA_LIMITE_X * 2, -TELA_LIMITE_Y * 2, TELA_LIMITE_X * 2, TELA_LIMITE_Y * 2);

    // 2. Margem do gramado (pista ao redor)
    glColor3f(0.25f, 0.35f, 0.25f);
    glRectf(-limiteGeralX, -limiteGeralY, limiteGeralX, limiteGeralY);

    // 3. Desenha os degraus
    for (int i = numDegraus; i > 0; i--) {
        float tomCinza = 0.35f + (i * 0.025f);
        glColor3f(tomCinza, tomCinza, tomCinza);

        float limiteX = MEIO_C + margem + (i * larguraDegrau);
        float limiteY = MEIO_L + margem + (i * larguraDegrau);

        // Preenchimento do degrau
        glBegin(GL_QUADS);
            glVertex2f(-limiteX, -limiteY);
            glVertex2f( limiteX, -limiteY);
            glVertex2f( limiteX,  limiteY);
            glVertex2f(-limiteX,  limiteY);
        glEnd();

        // Linha divisória fina para marcar bem o degrau
        glColor3f(0.15f, 0.15f, 0.15f);
        glLineWidth(1.0f);
        glBegin(GL_LINE_LOOP);
            glVertex2f(-limiteX, -limiteY);
            glVertex2f( limiteX, -limiteY);
            glVertex2f( limiteX,  limiteY);
            glVertex2f(-limiteX,  limiteY);
        glEnd();
    }

    // 4. Desenha as escadas (linhas grossas)
    // Aqui entra o recuo para a linha não vazar a borda por causa da espessura dela
    float recuo = 2.0f; 
    float calcX = limiteGeralX - recuo;
    float calcY = limiteGeralY - recuo;

    glColor3f(0.1f, 0.1f, 0.1f);
    glLineWidth(4.0f);
    glBegin(GL_LINES);
        // Retas
        glVertex2f(0.0f, MEIO_L + margem);     glVertex2f(0.0f, calcY);
        glVertex2f(0.0f, -MEIO_L - margem);    glVertex2f(0.0f, -calcY);
        glVertex2f(MEIO_C + margem, 0.0f);     glVertex2f(calcX, 0.0f);
        glVertex2f(-MEIO_C - margem, 0.0f);    glVertex2f(-calcX, 0.0f);
        
        // Diagonais
        glVertex2f(MEIO_C + margem, MEIO_L + margem);   glVertex2f(calcX, calcY);
        glVertex2f(-MEIO_C - margem, MEIO_L + margem);  glVertex2f(-calcX, calcY);
        glVertex2f(MEIO_C + margem, -MEIO_L - margem);  glVertex2f(calcX, -calcY);
        glVertex2f(-MEIO_C - margem, -MEIO_L - margem); glVertex2f(-calcX, -calcY);
    glEnd();
    glLineWidth(2.0f); // Reseta a linha
=======
    
    desenhaTempo(TEMPO_POS_X, TEMPO_POS_Y);
    desenhaComandos();
    desenhaBotaoConfig();
}

void desenhaJogadores() {
    for(int i = 0; i < 22; i++) {
        glPushMatrix();
        glTranslatef(jogadores[i].x, jogadores[i].y, 0.0f);
        glRotatef(jogadores[i].angle, 0.0f, 0.0f, 1.0f);
        
        if (jogadores[i].time == 0) glColor3f(COR_TIME_ESQ_R, COR_TIME_ESQ_G, COR_TIME_ESQ_B);
        else glColor3f(COR_TIME_DIR_R, COR_TIME_DIR_G, COR_TIME_DIR_B);

        float anim = sin(jogadores[i].animOffset) * 6.0f;
        desenhaCirculo(anim, 11.0f, 5.0f, 15, 0, 360, PREENCHIDO);
        desenhaCirculo(-anim, -11.0f, 5.0f, 15, 0, 360, PREENCHIDO);
        
        desenhaCirculo(0.0f, 0.0f, 12.0f, 25, 0, 360, PREENCHIDO);

        glColor3f(jogadores[i].skinR, jogadores[i].skinG, jogadores[i].skinB);
        desenhaCirculo(0.0f, 0.0f, 8.0f, 25, 0, 360, PREENCHIDO);

        glPopMatrix();
    }
>>>>>>> origin/mecanica
}

void campo() {
    float largura_faixa = CAMPO_C / QTD_FAIXAS_GRAMA;
    
    for (int i = 0; i < QTD_FAIXAS_GRAMA; i++) {
        if ((i % 2 == 0 && !gramaInvertida) || (i % 2 != 0 && gramaInvertida)) {
            glColor3f(GRAMA_CLARA_R, GRAMA_CLARA_G, GRAMA_CLARA_B);
        } else {
            glColor3f(GRAMA_ESCURA_R, GRAMA_ESCURA_G, GRAMA_ESCURA_B);
        }
        float x_inicio = -MEIO_C + (i * largura_faixa);
        float x_fim = x_inicio + largura_faixa;
        glRectf(x_inicio, -MEIO_L, x_fim, MEIO_L);
    }

    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.0f);

    glBegin(GL_LINE_LOOP);
        glVertex2f(-MEIO_C, -MEIO_L);
        glVertex2f( MEIO_C, -MEIO_L);
        glVertex2f( MEIO_C,  MEIO_L);
        glVertex2f(-MEIO_C,  MEIO_L);
    glEnd();

    glBegin(GL_LINES);
        glVertex2f(0.0f, -MEIO_L);
        glVertex2f(0.0f,  MEIO_L);
    glEnd();

    desenhaCirculo(0, 0, RAIO_CENTRO, 100, 0, 360, BORDA);
    desenhaCirculo(0, 0, RAIO_PONTO_CENTRAL, 30, 0, 360, PREENCHIDO); 

    float ga_y = AREA_G_L / 2.0f;
    float pa_y = AREA_P_L / 2.0f;
    float gol_y = GOL_L / 2.0f;

    float x_fundo_esq = -MEIO_C;
    glBegin(GL_LINE_STRIP);
        glVertex2f(x_fundo_esq, ga_y); glVertex2f(x_fundo_esq + AREA_G_C, ga_y);
        glVertex2f(x_fundo_esq + AREA_G_C, -ga_y); glVertex2f(x_fundo_esq, -ga_y);
    glEnd();
    glBegin(GL_LINE_STRIP);
        glVertex2f(x_fundo_esq, pa_y); glVertex2f(x_fundo_esq + AREA_P_C, pa_y);
        glVertex2f(x_fundo_esq + AREA_P_C, -pa_y); glVertex2f(x_fundo_esq, -pa_y);
    glEnd();

    float x_penalti_esq = x_fundo_esq + MARCA_PENALTI;
    desenhaCirculo(x_penalti_esq, 0, RAIO_MARCA_PENALTI_ESQ, 30, 0, 360, PREENCHIDO);

    float dist_para_linha = AREA_G_C - MARCA_PENALTI;
    float angulo = acos(dist_para_linha / RAIO_CENTRO) * 180.0 / PI;
    desenhaCirculo(x_penalti_esq, 0, RAIO_CENTRO, 50, -angulo, angulo, BORDA);

    desenhaRede(x_fundo_esq - GOL_PROF, x_fundo_esq, -gol_y, gol_y);
    glBegin(GL_LINE_STRIP);
        glVertex2f(x_fundo_esq, gol_y); glVertex2f(x_fundo_esq - GOL_PROF, gol_y);
        glVertex2f(x_fundo_esq - GOL_PROF, -gol_y); glVertex2f(x_fundo_esq, -gol_y);
    glEnd();

    float x_fundo_dir = MEIO_C;
    glBegin(GL_LINE_STRIP);
        glVertex2f(x_fundo_dir, ga_y); glVertex2f(x_fundo_dir - AREA_G_C, ga_y);
        glVertex2f(x_fundo_dir - AREA_G_C, -ga_y); glVertex2f(x_fundo_dir, -ga_y);
    glEnd();
    glBegin(GL_LINE_STRIP);
        glVertex2f(x_fundo_dir, pa_y); glVertex2f(x_fundo_dir - AREA_P_C, pa_y);
        glVertex2f(x_fundo_dir - AREA_P_C, -pa_y); glVertex2f(x_fundo_dir, -pa_y);
    glEnd();

    float x_penalti_dir = x_fundo_dir - MARCA_PENALTI;
    desenhaCirculo(x_penalti_dir, 0, RAIO_MARCA_PENALTI_DIR, 30, 0, 360, PREENCHIDO);
    desenhaCirculo(x_penalti_dir, 0, RAIO_CENTRO, 50, 180 - angulo, 180 + angulo, BORDA);

    desenhaRede(x_fundo_dir, x_fundo_dir + GOL_PROF, -gol_y, gol_y);
    glBegin(GL_LINE_STRIP);
        glVertex2f(x_fundo_dir, gol_y); glVertex2f(x_fundo_dir + GOL_PROF, gol_y);
        glVertex2f(x_fundo_dir + GOL_PROF, -gol_y); glVertex2f(x_fundo_dir, -gol_y);
    glEnd();

    desenhaCirculo(-MEIO_C, -MEIO_L, RAIO_ESCANTEIO, 15, 0, 90, BORDA);
    desenhaCirculo( MEIO_C, -MEIO_L, RAIO_ESCANTEIO, 15, 90, 180, BORDA);
    desenhaCirculo( MEIO_C,  MEIO_L, RAIO_ESCANTEIO, 15, 180, 270, BORDA);
    desenhaCirculo(-MEIO_C,  MEIO_L, RAIO_ESCANTEIO, 15, 270, 360, BORDA);
}

void desenhaBola() {
    float escalaChapeu = 1.0f;
    float offsetSombraX = 0.0f;
    float offsetSombraY = 0.0f;
    
    if (executandoChapeu) {
        float proporcao = tempoAtualChapeu / DURACAO_CHAPEU_SEC;
        escalaChapeu = 1.0f + (ESCALA_MAX_CHAPEU - 1.0f) * sin(proporcao * PI);
        offsetSombraX = 3.0f * sin(proporcao * PI);
        offsetSombraY = -6.0f * sin(proporcao * PI);
    }
    
    if (executandoChapeu) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.0f, 0.0f, 0.0f, 0.4f);
        desenhaCirculo(bolaX + offsetSombraX, bolaY + offsetSombraY, BOLA_RAIO, 30, 0, 360, PREENCHIDO);
        glDisable(GL_BLEND);
    }
    
    glPushMatrix();
    glTranslatef(bolaX, bolaY, 0.0f);
    glScalef(escalaChapeu, escalaChapeu, 1.0f);
    
    if (posseDireita) {
        glColor3f(COR_TIME_DIR_R, COR_TIME_DIR_G, COR_TIME_DIR_B);
    } else {
        glColor3f(COR_TIME_ESQ_R, COR_TIME_ESQ_G, COR_TIME_ESQ_B);
    }
    glLineWidth(ESPESSURA_CONTORNO_BOLA);
    desenhaCirculo(0.0f, 0.0f, BOLA_RAIO + ESPESSURA_CONTORNO_BOLA, 30, 0, 360, BORDA);
    
    glColor3f(1.0f, 1.0f, 1.0f);
    desenhaCirculo(0.0f, 0.0f, BOLA_RAIO, 30, 0, 360, PREENCHIDO);
    glPopMatrix();
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    float larguraBarra = BOLA_RAIO * 4.0f;
    float alturaBarra = 4.0f;
    float deslocamentoY = -BOLA_RAIO * 3.5f;
    
    glColor4f(0.1f, 0.1f, 0.1f, 0.6f);
    glRectf(bolaX - larguraBarra/2.0f, bolaY + deslocamentoY - alturaBarra, bolaX + larguraBarra/2.0f, bolaY + deslocamentoY);
    
    if (progressoChapeu >= 1.0f) {
        glColor4f(0.0f, 1.0f, 0.0f, 0.8f);
    } else {
        glColor4f(1.0f, 0.8f, 0.0f, 0.8f);
    }
    
    float larguraAtual = larguraBarra * progressoChapeu;
    glRectf(bolaX - larguraBarra/2.0f, bolaY + deslocamentoY - alturaBarra, bolaX - larguraBarra/2.0f + larguraAtual, bolaY + deslocamentoY);
    
    glDisable(GL_BLEND);
}

void liberarInputs(int value) {
    inputsBloqueados = false;
}

void chuteGoleiroEsquerda(int value) {
    inputsBloqueados = false;
    defendendoGolContra = false;
    bolaX = jogadores[0].x + RAIO_DEFESA_GK + BOLA_RAIO + 5.0f;
    bolaVX = BOLA_MAX_VEL; bolaVY = 0.0f;
    tocarSomArquivo(ARQ_SOM_DISPARADA, VOL_DISPARADA);
}

void chuteGoleiroDireita(int value) {
    inputsBloqueados = false;
    defendendoGolContra = false;
    bolaX = jogadores[11].x - RAIO_DEFESA_GK - BOLA_RAIO - 5.0f;
    bolaVX = -BOLA_MAX_VEL; bolaVY = 0.0f;
    tocarSomArquivo(ARQ_SOM_DISPARADA, VOL_DISPARADA);
}

void resetJogadores(bool isGoal) {
    for (int i = 0; i < 22; i++) {
        jogadores[i].x = jogadores[i].homeX;
        jogadores[i].y = jogadores[i].homeY;
        jogadores[i].animOffset = 0.0f;
        jogadores[i].angle = (jogadores[i].time == 0) ? 0.0f : 180.0f;
        
        if (!isGoal) {
            bool isEnemy = (jogadores[i].time == 0 && posseDireita) || (jogadores[i].time == 1 && !posseDireita);
            if (isEnemy) {
                float dx = jogadores[i].x - bolaX;
                float dy = jogadores[i].y - bolaY;
                float dist = sqrt(dx*dx + dy*dy);
                if (dist < DISTANCIA_MINIMA_COBRANCA) {
                    if (dist == 0.0f) { dx = 1.0f; dy = 0.0f; dist = 1.0f; }
                    jogadores[i].x = bolaX + (dx/dist) * DISTANCIA_MINIMA_COBRANCA;
                    jogadores[i].y = bolaY + (dy/dist) * DISTANCIA_MINIMA_COBRANCA;
                }
            }
        }
    }
}

void resolverSaidaDeBola(int evento) {
    bool isGoal = false;
    switch (evento) {
        case EVENTO_GOL_ESQ:
            placarDir++;
            bolaX = 0.0f; bolaY = 0.0f;
            progressoChapeu = 0.0f;
            posseDireita = false;
            isGoal = true;
            tocarSomArquivo(ARQ_SOM_GOL, VOL_GOL);
            tocarSomArquivo(ARQ_SOM_TORCIDA_VIBRA, VOL_TORCIDA_VIBRA);
            break;
        case EVENTO_GOL_DIR:
            placarEsq++;
            bolaX = 0.0f; bolaY = 0.0f;
            progressoChapeu = 0.0f;
            posseDireita = true;
            isGoal = true;
            tocarSomArquivo(ARQ_SOM_GOL, VOL_GOL);
            tocarSomArquivo(ARQ_SOM_TORCIDA_VIBRA, VOL_TORCIDA_VIBRA);
            break;
        case EVENTO_LATERAL_CIMA:
            bolaX = ultimaSaidaX; bolaY = MEIO_L - BOLA_RAIO - 1.0f;
            tocarSomArquivo(ARQ_SOM_APITO, VOL_APITO);
            esperandoCobranca = true;
            break;
        case EVENTO_LATERAL_BAIXO:
            bolaX = ultimaSaidaX; bolaY = -MEIO_L + BOLA_RAIO + 1.0f;
            tocarSomArquivo(ARQ_SOM_APITO, VOL_APITO);
            esperandoCobranca = true;
            break;
        case EVENTO_ESCANTEIO_ESQ_CIMA:
            bolaX = -MEIO_C + BOLA_RAIO + 1.0f; bolaY = MEIO_L - BOLA_RAIO - 1.0f;
            tocarSomArquivo(ARQ_SOM_APITO, VOL_APITO);
            esperandoCobranca = true;
            break;
        case EVENTO_ESCANTEIO_ESQ_BAIXO:
            bolaX = -MEIO_C + BOLA_RAIO + 1.0f; bolaY = -MEIO_L + BOLA_RAIO + 1.0f;
            tocarSomArquivo(ARQ_SOM_APITO, VOL_APITO);
            esperandoCobranca = true;
            break;
        case EVENTO_ESCANTEIO_DIR_CIMA:
            bolaX = MEIO_C - BOLA_RAIO - 1.0f; bolaY = MEIO_L - BOLA_RAIO - 1.0f;
            tocarSomArquivo(ARQ_SOM_APITO, VOL_APITO);
            esperandoCobranca = true;
            break;
        case EVENTO_ESCANTEIO_DIR_BAIXO:
            bolaX = MEIO_C - BOLA_RAIO - 1.0f; bolaY = -MEIO_L + BOLA_RAIO + 1.0f;
            tocarSomArquivo(ARQ_SOM_APITO, VOL_APITO);
            esperandoCobranca = true;
            break;
        case EVENTO_INTERVALO:
            bolaX = 0.0f; bolaY = 0.0f;
            gramaInvertida = !gramaInvertida;
            int temp = placarEsq;
            placarEsq = placarDir;
            placarDir = temp;
            segundoTempo = true;
            progressoChapeu = 0.0f;
            posseDireita = !TIME_INICIAL_DIREITA;
            isGoal = true;
            tocarSomArquivo(ARQ_SOM_APITO, VOL_APITO);
            break;
        case EVENTO_FIM_JOGO:
            placarEsq = 0; placarDir = 0;
            tempoJogoVirtualSegundos = 0.0f;
            segundoTempo = false;
            fimDeJogo = false;
            gramaInvertida = false;
            progressoChapeu = 0.0f;
            posseDireita = TIME_INICIAL_DIREITA;
            isGoal = true;
            tocarSomArquivo(ARQ_SOM_APITO, VOL_APITO);
            tocarSomArquivo(ARQ_SOM_TORCIDA_VIBRA, VOL_TORCIDA_VIBRA);
            break;
        case EVENTO_TOMADA_BOLA:
            tocarSomArquivo(ARQ_SOM_INTERCEPTACAO, VOL_INTERCEPTACAO);
            break;
    }
    
    if (evento != EVENTO_TOMADA_BOLA) {
        resetJogadores(isGoal);
    }
    
    if (evento != EVENTO_FIM_JOGO) {
        bolaVX = 0.0f; 
        bolaVY = 0.0f;
        bolaEmJogo = true;
        inputsBloqueados = false;
    } else {
        bolaVX = 0.0f; 
        bolaVY = 0.0f;
        bolaX = 0.0f; 
        bolaY = 0.0f;
        bolaEmJogo = true;
        inputsBloqueados = false;
    }
}

void iniciarSequenciaSaida(TipoEvento evento) {
    bolaEmJogo = false;
    inputsBloqueados = true;
    cronometroRodando = false;
    ultimaSaidaX = bolaX;
    ultimaSaidaY = bolaY;
    
    if(ultimaSaidaX > MEIO_C) ultimaSaidaX = MEIO_C;
    if(ultimaSaidaX < -MEIO_C) ultimaSaidaX = -MEIO_C;
    if(ultimaSaidaY > MEIO_L) ultimaSaidaY = MEIO_L;
    if(ultimaSaidaY < -MEIO_L) ultimaSaidaY = -MEIO_L;

    int delay = (evento == EVENTO_FIM_JOGO) ? 3000 : 1500;
    glutTimerFunc(delay, resolverSaidaDeBola, evento);
}

void atualizaFisica(int value) {
    if (!inputsBloqueados && bolaEmJogo) {
        if (teclaW || teclaI || teclaCima)  bolaVY += BOLA_ACEL;
        if (teclaS || teclaK || teclaBaixo) bolaVY -= BOLA_ACEL;
        if (teclaA || teclaJ || teclaEsq)   bolaVX -= BOLA_ACEL;
        if (teclaD || teclaL || teclaDir)   bolaVX += BOLA_ACEL;

        if (teclaW || teclaI || teclaCima || teclaS || teclaK || teclaBaixo || 
            teclaA || teclaJ || teclaEsq || teclaD || teclaL || teclaDir) {
            cronometroRodando = true;
        }
    }

    bolaVX *= BOLA_ATRITO;
    bolaVY *= BOLA_ATRITO;

    float velAtual = sqrt(bolaVX * bolaVX + bolaVY * bolaVY);
    
    if (velAnterior < 0.5f && velAtual > 2.0f && !executandoChapeu) {
        tocarSomArquivo(ARQ_SOM_DISPARADA, VOL_DISPARADA);
    }
    atualizarSomBola(velAtual);
    velAnterior = velAtual;

    if (velAtual > BOLA_MAX_VEL) {
        bolaVX = (bolaVX / velAtual) * BOLA_MAX_VEL;
        bolaVY = (bolaVY / velAtual) * BOLA_MAX_VEL;
    }
    
    if (velAtual > 0.5f) {
        esperandoCobranca = false;
    }

    bolaX += bolaVX;
    bolaY += bolaVY;

    for (int i = 0; i < 22; i++) {
        if (!bolaEmJogo) continue;
        
        bool isEnemy = (jogadores[i].time == 0 && posseDireita) || (jogadores[i].time == 1 && !posseDireita);
        if (esperandoCobranca && isEnemy) continue;
        
        if (defendendoGolContra && ((!posseDireita && i == 0) || (posseDireita && i == 11))) continue;
        
        if (jogadores[i].role == 0) {
            float targetY = bolaY;
            if (targetY > GOL_L / 2.0f) targetY = GOL_L / 2.0f;
            if (targetY < -GOL_L / 2.0f) targetY = -GOL_L / 2.0f;
            float dy = targetY - jogadores[i].y;
            if (fabs(dy) > jogadores[i].speed) {
                jogadores[i].y += (dy > 0 ? jogadores[i].speed : -jogadores[i].speed);
                jogadores[i].animOffset += 0.2f;
            } else {
                jogadores[i].y = targetY;
                jogadores[i].animOffset = 0.0f;
            }
            jogadores[i].angle = (jogadores[i].time == 0) ? 0.0f : 180.0f;
        } else {
            float tx = jogadores[i].homeX;
            float ty = jogadores[i].homeY;

            bool inMyHalf = (jogadores[i].time == 0) ? (bolaX < 0) : (bolaX > 0);
            bool enemyPossession = (jogadores[i].time == 0) ? posseDireita : !posseDireita;

            if (inMyHalf && enemyPossession && !defendendoGolContra) {
                tx = bolaX;
                ty = bolaY;
            }

            float dx = tx - jogadores[i].x;
            float dy = ty - jogadores[i].y;
            float dist = sqrt(dx*dx + dy*dy);

            if (dist > jogadores[i].speed) {
                jogadores[i].x += (dx/dist) * jogadores[i].speed;
                jogadores[i].y += (dy/dist) * jogadores[i].speed;
                jogadores[i].animOffset += 0.2f;
                jogadores[i].angle = atan2(dy, dx) * 180.0f / PI;
            } else {
                jogadores[i].x = tx;
                jogadores[i].y = ty;
                jogadores[i].animOffset = 0.0f;
                if (tx == jogadores[i].homeX && ty == jogadores[i].homeY) {
                    jogadores[i].angle = (jogadores[i].time == 0) ? 0.0f : 180.0f;
                }
            }
        }
    }

    for (int i = 0; i < 22; i++) {
        for (int j = i + 1; j < 22; j++) {
            float dx = jogadores[i].x - jogadores[j].x;
            float dy = jogadores[i].y - jogadores[j].y;
            float dist = sqrt(dx*dx + dy*dy);
            if (dist < 24.0f && dist > 0.001f) {
                float overlap = (24.0f - dist) / 2.0f;
                float nx = (dx/dist) * overlap;
                float ny = (dy/dist) * overlap;
                jogadores[i].x += nx;
                jogadores[i].y += ny;
                jogadores[j].x -= nx;
                jogadores[j].y -= ny;
            }
        }
    }

    if (cronometroRodando) {
        tempoJogoVirtualSegundos += (16.0f / 1000.0f) * (90.0f / tempoPartidaMinutos);
        
        if (tempoJogoVirtualSegundos >= 45.0f * 60.0f && !segundoTempo) {
            iniciarSequenciaSaida(EVENTO_INTERVALO);
        } else if (tempoJogoVirtualSegundos >= 90.0f * 60.0f && !fimDeJogo) {
            iniciarSequenciaSaida(EVENTO_FIM_JOGO);
        }
    }

    bool pertoInimigo = false;
    if (!executandoChapeu) {
        for(int i = 0; i < 22; i++) {
            if(jogadores[i].time != (posseDireita ? 1 : 0)) {
                float dx = bolaX - jogadores[i].x;
                float dy = bolaY - jogadores[i].y;
                float dist = sqrt(dx*dx + dy*dy);
                if (dist < BOLA_RAIO * 8.0f) {
                    pertoInimigo = true;
                    if (dist < BOLA_RAIO * 3.0f && !inputsBloqueados && bolaEmJogo && !defendendoGolContra) {
                        posseDireita = !posseDireita;
                        resolverSaidaDeBola(EVENTO_TOMADA_BOLA);
                        break;
                    }
                }
            }
        }
    }

    if (executandoChapeu) {
        tempoAtualChapeu += 16.0f / 1000.0f;
        if (tempoAtualChapeu >= DURACAO_CHAPEU_SEC) {
            executandoChapeu = false;
            tempoAtualChapeu = 0.0f;
        }
    } else {
        if (velAtual > 0.5f && bolaEmJogo && !inputsBloqueados && pertoInimigo) {
            progressoChapeu += (16.0f / 1000.0f) / TEMPO_CHAPEU_SEC;
            if (progressoChapeu > 1.0f) progressoChapeu = 1.0f;
        }
    }

    if (!executandoChapeu) {
        float distGkEsq = sqrt(pow(bolaX - jogadores[0].x, 2) + pow(bolaY - jogadores[0].y, 2));
        float distGkDir = sqrt(pow(bolaX - jogadores[11].x, 2) + pow(bolaY - jogadores[11].y, 2));

        if (distGkEsq < RAIO_DEFESA_GK && bolaEmJogo && !defendendoGolContra && !posseDireita) {
            defendendoGolContra = true;
            inputsBloqueados = true;
            bolaVX = 0.0f; bolaVY = 0.0f;
            bolaX = jogadores[0].x + BOLA_RAIO * 2.0f; bolaY = jogadores[0].y;
            posseDireita = false;
            tocarSomArquivo(ARQ_SOM_DEFESA, VOL_DEFESA);
            glutTimerFunc(1500, chuteGoleiroEsquerda, 0);
        } else if (distGkDir < RAIO_DEFESA_GK && bolaEmJogo && !defendendoGolContra && posseDireita) {
            defendendoGolContra = true;
            inputsBloqueados = true;
            bolaVX = 0.0f; bolaVY = 0.0f;
            bolaX = jogadores[11].x - BOLA_RAIO * 2.0f; bolaY = jogadores[11].y;
            posseDireita = true;
            tocarSomArquivo(ARQ_SOM_DEFESA, VOL_DEFESA);
            glutTimerFunc(1500, chuteGoleiroDireita, 0);
        }
    }

    if (!executandoChapeu) {
        float px[4] = {-MEIO_C, -MEIO_C, MEIO_C, MEIO_C};
        float py[4] = {GOL_L / 2.0f, -GOL_L / 2.0f, GOL_L / 2.0f, -GOL_L / 2.0f};
        for (int i = 0; i < 4; i++) {
            float dx = bolaX - px[i];
            float dy = bolaY - py[i];
            float dist = sqrt(dx * dx + dy * dy);
            if (dist < BOLA_RAIO) {
                float nx = dx / dist;
                float ny = dy / dist;
                float dot = bolaVX * nx + bolaVY * ny;
                bolaVX = bolaVX - 2 * dot * nx;
                bolaVY = bolaVY - 2 * dot * ny;
                bolaX = px[i] + nx * (BOLA_RAIO + 0.1f);
                bolaY = py[i] + ny * (BOLA_RAIO + 0.1f);
            }
        }

        if (bolaX < -MEIO_C) {
            if (bolaY > GOL_L / 2.0f && bolaY - BOLA_RAIO < GOL_L / 2.0f && bolaX > -MEIO_C - GOL_PROF) {
                bolaY = GOL_L / 2.0f + BOLA_RAIO; bolaVY *= -1;
            } else if (bolaY < -GOL_L / 2.0f && bolaY + BOLA_RAIO > -GOL_L / 2.0f && bolaX > -MEIO_C - GOL_PROF) {
                bolaY = -GOL_L / 2.0f - BOLA_RAIO; bolaVY *= -1;
            } else if (bolaX > -MEIO_C - GOL_PROF - BOLA_RAIO && bolaX < -MEIO_C - GOL_PROF && bolaY > -GOL_L / 2.0f && bolaY < GOL_L / 2.0f) {
                bolaX = -MEIO_C - GOL_PROF - BOLA_RAIO; bolaVX *= -1;
            } else if (bolaY < GOL_L / 2.0f && bolaY > -GOL_L / 2.0f) {
                if (bolaX - BOLA_RAIO < -MEIO_C - GOL_PROF) {
                    bolaX = -MEIO_C - GOL_PROF + BOLA_RAIO; bolaVX = 0.0f; bolaVY = 0.0f;
                }
                if (bolaY + BOLA_RAIO > GOL_L / 2.0f) {
                    bolaY = GOL_L / 2.0f - BOLA_RAIO; bolaVX = 0.0f; bolaVY = 0.0f;
                }
                if (bolaY - BOLA_RAIO < -GOL_L / 2.0f) {
                    bolaY = -GOL_L / 2.0f + BOLA_RAIO; bolaVX = 0.0f; bolaVY = 0.0f;
                }
            }
        } else if (bolaX > MEIO_C) {
            if (bolaY > GOL_L / 2.0f && bolaY - BOLA_RAIO < GOL_L / 2.0f && bolaX < MEIO_C + GOL_PROF) {
                bolaY = GOL_L / 2.0f + BOLA_RAIO; bolaVY *= -1;
            } else if (bolaY < -GOL_L / 2.0f && bolaY + BOLA_RAIO > -GOL_L / 2.0f && bolaX < MEIO_C + GOL_PROF) {
                bolaY = -GOL_L / 2.0f - BOLA_RAIO; bolaVY *= -1;
            } else if (bolaX < MEIO_C + GOL_PROF + BOLA_RAIO && bolaX > MEIO_C + GOL_PROF && bolaY > -GOL_L / 2.0f && bolaY < GOL_L / 2.0f) {
                bolaX = MEIO_C + GOL_PROF + BOLA_RAIO; bolaVX *= -1;
            } else if (bolaY < GOL_L / 2.0f && bolaY > -GOL_L / 2.0f) {
                if (bolaX + BOLA_RAIO > MEIO_C + GOL_PROF) {
                    bolaX = MEIO_C + GOL_PROF - BOLA_RAIO; bolaVX = 0.0f; bolaVY = 0.0f;
                }
                if (bolaY + BOLA_RAIO > GOL_L / 2.0f) {
                    bolaY = GOL_L / 2.0f - BOLA_RAIO; bolaVX = 0.0f; bolaVY = 0.0f;
                }
                if (bolaY - BOLA_RAIO < -GOL_L / 2.0f) {
                    bolaY = -GOL_L / 2.0f + BOLA_RAIO; bolaVX = 0.0f; bolaVY = 0.0f;
                }
            }
        }
    }

    if (bolaEmJogo && !defendendoGolContra) {
        if (bolaY - BOLA_RAIO > MEIO_L) {
            iniciarSequenciaSaida(EVENTO_LATERAL_CIMA);
        } else if (bolaY + BOLA_RAIO < -MEIO_L) {
            iniciarSequenciaSaida(EVENTO_LATERAL_BAIXO);
        } else if (bolaX - BOLA_RAIO > MEIO_C) {
            if (bolaY < (GOL_L / 2.0f) && bolaY > -(GOL_L / 2.0f)) {
                iniciarSequenciaSaida(EVENTO_GOL_DIR);
            } else {
                if (bolaY > 0) iniciarSequenciaSaida(EVENTO_ESCANTEIO_DIR_CIMA);
                else iniciarSequenciaSaida(EVENTO_ESCANTEIO_DIR_BAIXO);
            }
        } else if (bolaX + BOLA_RAIO < -MEIO_C) {
            if (bolaY < (GOL_L / 2.0f) && bolaY > -(GOL_L / 2.0f)) {
                iniciarSequenciaSaida(EVENTO_GOL_ESQ);
            } else {
                if (bolaY > 0) iniciarSequenciaSaida(EVENTO_ESCANTEIO_ESQ_CIMA);
                else iniciarSequenciaSaida(EVENTO_ESCANTEIO_ESQ_BAIXO);
            }
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, atualizaFisica, 0); 
}

void geral() {
    glClear(GL_COLOR_BUFFER_BIT); 
    
    glPushMatrix();
    glTranslatef(OFFSET_CAMPO_X, OFFSET_CAMPO_Y, 0.0f);
    
    //desenhaTodosPlacares();
    desenhaArquibancada();
    campo();
    desenhaJogadores();
    desenhaBola();
    
    glPopMatrix(); 

    desenhaPlacarCentral();
    desenhaBotaoSair();

    
    glutSwapBuffers(); 
}

<<<<<<< HEAD
void cliqueMouse(int button, int state, int x, int y) { //adição nova (Jadiel) função de sair do programa.
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Pega as dimensões atuais da janela
        int larguraJanela = glutGet(GLUT_WINDOW_WIDTH);
        int alturaJanela = glutGet(GLUT_WINDOW_HEIGHT);

        // Converte as coordenadas da tela (janela) pro sistema Ortho2D
        float orthoX = ((float)x / larguraJanela) * (2 * TELA_LIMITE_X) - TELA_LIMITE_X;
        float orthoY = TELA_LIMITE_Y - ((float)y / alturaJanela) * (2 * TELA_LIMITE_Y);

        // Verifica se a conversão do clique está dentro da área do botão
        if (orthoX >= BOTAO_X_MIN && orthoX <= BOTAO_X_MAX &&
            orthoY >= BOTAO_Y_MIN && orthoY <= BOTAO_Y_MAX) {
            exit(0); // Fecha o jogo com sucesso
=======
void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float mx = ((float)x / glutGet(GLUT_WINDOW_WIDTH)) * (2.0f * TELA_LIMITE_X) - TELA_LIMITE_X;
        float my = ((1.0f - (float)y / glutGet(GLUT_WINDOW_HEIGHT))) * (2.0f * TELA_LIMITE_Y) - TELA_LIMITE_Y;

        float btnX1 = OPCOES_POS_X;
        float btnX2 = OPCOES_POS_X + OPCOES_LARGURA;
        float btnY1 = OPCOES_POS_Y;
        float btnY2 = OPCOES_POS_Y + OPCOES_ALTURA;

        if (mx >= btnX1 && mx <= btnX2 && my >= btnY1 && my <= btnY2) {
            menuAberto = !menuAberto;
        } else if (menuAberto && mx >= btnX1 && mx <= btnX2 && my >= btnY2 + 10.0f && my <= btnY2 + 220.0f) {
            float menuBase = btnY2 + 10.0f;
            float hItem = 42.0f;
            
            if (my >= menuBase + (hItem * 0) && my < menuBase + (hItem * 1)) {
                tempoPartidaMinutos = tempoOpcoes[0];
            } else if (my >= menuBase + (hItem * 1) && my < menuBase + (hItem * 2)) {
                tempoPartidaMinutos = tempoOpcoes[1];
            } else if (my >= menuBase + (hItem * 2) && my < menuBase + (hItem * 3)) {
                tempoPartidaMinutos = tempoOpcoes[2];
            } else if (my >= menuBase + (hItem * 3) && my < menuBase + (hItem * 4)) {
                tempoJogoVirtualSegundos = 0.0f;
                placarEsq = 0; placarDir = 0;
                bolaX = 0.0f; bolaY = 0.0f; bolaVX = 0.0f; bolaVY = 0.0f;
                segundoTempo = false; fimDeJogo = false;
                cronometroRodando = false; gramaInvertida = false;
                progressoChapeu = 0.0f;
                posseDireita = TIME_INICIAL_DIREITA;
                resetJogadores(true);
            } else if (my >= menuBase + (hItem * 4) && my <= menuBase + (hItem * 5)) {
                exit(0);
            }
            menuAberto = false;
>>>>>>> origin/mecanica
        }
    }
}

void tecladoAperta(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': case 'W': teclaW = true; break;
        case 's': case 'S': teclaS = true; break;
        case 'a': case 'A': teclaA = true; break;
        case 'd': case 'D': teclaD = true; break;
        case 'i': case 'I': teclaI = true; break;
        case 'k': case 'K': teclaK = true; break;
        case 'j': case 'J': teclaJ = true; break;
        case 'l': case 'L': teclaL = true; break;
        case ' ':
            if (progressoChapeu >= 1.0f && !executandoChapeu && bolaEmJogo && !inputsBloqueados) {
                executandoChapeu = true;
                progressoChapeu = 0.0f;
                tempoAtualChapeu = 0.0f;
                tocarSomArquivo(ARQ_SOM_CHAPEU, VOL_CHAPEU);
            }
            break;
    }
}

void tecladoSolta(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': case 'W': teclaW = false; break;
        case 's': case 'S': teclaS = false; break;
        case 'a': case 'A': teclaA = false; break;
        case 'd': case 'D': teclaD = false; break;
        case 'i': case 'I': teclaI = false; break;
        case 'k': case 'K': teclaK = false; break;
        case 'j': case 'J': teclaJ = false; break;
        case 'l': case 'L': teclaL = false; break;
    }
}

void teclasEspeciaisAperta(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP: teclaCima = true; break;
        case GLUT_KEY_DOWN: teclaBaixo = true; break;
        case GLUT_KEY_LEFT: teclaEsq = true; break;
        case GLUT_KEY_RIGHT: teclaDir = true; break;
    }
}

void teclasEspeciaisSolta(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP: teclaCima = false; break;
        case GLUT_KEY_DOWN: teclaBaixo = false; break;
        case GLUT_KEY_LEFT: teclaEsq = false; break;
        case GLUT_KEY_RIGHT: teclaDir = false; break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("Futebol - Copa 2026");
    glutFullScreen();
    
    init();
    
    glutDisplayFunc(geral);
    
    glutKeyboardFunc(tecladoAperta);
    glutKeyboardUpFunc(tecladoSolta);
    glutSpecialFunc(teclasEspeciaisAperta);
    glutSpecialUpFunc(teclasEspeciaisSolta);
<<<<<<< HEAD
    glutMouseFunc(cliqueMouse);
=======
    glutMouseFunc(mouseClick);
>>>>>>> origin/mecanica
    
    glutTimerFunc(16, atualizaFisica, 0);

    glutMainLoop();
    return 0;
}