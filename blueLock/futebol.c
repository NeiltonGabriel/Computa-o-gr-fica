#include <GL/glut.h>
#include <math.h>
#include <stdbool.h>

#define PI 3.14159265
#define PREENCHIDO 1
#define BORDA 0

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
    EVENTO_TOMADA_BOLA
} TipoEvento;

int placarEsq = 0;
int placarDir = 0;

float bolaX = 0.0f;
float bolaY = 0.0f;
float bolaVX = 0.0f;
float bolaVY = 0.0f;

const float BOLA_RAIO = 0.6f * ESCALA;
const float BOLA_ACEL = 0.35f;       
const float BOLA_ATRITO = 0.95f;    
const float BOLA_MAX_VEL = 20.0f;   

const float TEMPO_CHAPEU_SEC = 3.0f;
const float DURACAO_CHAPEU_SEC = 0.8f;
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

void init() {
    glClearColor(COR_FUNDO_R, COR_FUNDO_G, COR_FUNDO_B, 1.0f); 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-TELA_LIMITE_X, TELA_LIMITE_X, -TELA_LIMITE_Y, TELA_LIMITE_Y);
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

void desenhaTodosPlacares() {
    float x_outdoor_esq = -TELA_LIMITE_X + (PLACAR_LARGURA / 2.0f) + 20.0f;
    desenhaOutdoor(x_outdoor_esq, PLACAR_POS_Y, placarEsq);

    float x_outdoor_dir = TELA_LIMITE_X - (PLACAR_LARGURA / 2.0f) - 20.0f;
    desenhaOutdoor(x_outdoor_dir, PLACAR_POS_Y, placarDir);
}

void campo() {
    float largura_faixa = CAMPO_C / QTD_FAIXAS_GRAMA;
    
    for (int i = 0; i < QTD_FAIXAS_GRAMA; i++) {
        if (i % 2 == 0) {
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
        escalaChapeu = 1.0f + 1.2f * sin(proporcao * PI);
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
    glColor3f(1.0f, 1.0f, 1.0f);
    desenhaCirculo(0.0f, 0.0f, BOLA_RAIO, 30, 0, 360, PREENCHIDO);
    glPopMatrix();
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    float larguraBarra = BOLA_RAIO * 4.0f;
    float alturaBarra = 4.0f;
    float deslocamentoY = BOLA_RAIO * 2.5f;
    
    glColor4f(0.1f, 0.1f, 0.1f, 0.6f);
    glRectf(bolaX - larguraBarra/2.0f, bolaY + deslocamentoY, bolaX + larguraBarra/2.0f, bolaY + deslocamentoY + alturaBarra);
    
    if (progressoChapeu >= 1.0f) {
        glColor4f(0.0f, 1.0f, 0.0f, 0.8f);
    } else {
        glColor4f(1.0f, 0.8f, 0.0f, 0.8f);
    }
    
    float larguraAtual = larguraBarra * progressoChapeu;
    glRectf(bolaX - larguraBarra/2.0f, bolaY + deslocamentoY, bolaX - larguraBarra/2.0f + larguraAtual, bolaY + deslocamentoY + alturaBarra);
    
    glDisable(GL_BLEND);
}

void liberarInputs(int value) {
    inputsBloqueados = false;
}

void bloquearInputsTempo(int tempo_ms) {
    inputsBloqueados = true;
    glutTimerFunc(tempo_ms, liberarInputs, 0);
}

void resolverSaidaDeBola(int evento) {
    switch (evento) {
        case EVENTO_GOL_ESQ:
            placarDir++;
            bolaX = 0.0f; bolaY = 0.0f;
            break;
        case EVENTO_GOL_DIR:
            placarEsq++;
            bolaX = 0.0f; bolaY = 0.0f;
            break;
        case EVENTO_LATERAL_CIMA:
            bolaX = ultimaSaidaX; bolaY = MEIO_L - BOLA_RAIO - 1.0f;
            break;
        case EVENTO_LATERAL_BAIXO:
            bolaX = ultimaSaidaX; bolaY = -MEIO_L + BOLA_RAIO + 1.0f;
            break;
        case EVENTO_ESCANTEIO_ESQ_CIMA:
            bolaX = -MEIO_C + BOLA_RAIO + 1.0f; bolaY = MEIO_L - BOLA_RAIO - 1.0f;
            break;
        case EVENTO_ESCANTEIO_ESQ_BAIXO:
            bolaX = -MEIO_C + BOLA_RAIO + 1.0f; bolaY = -MEIO_L + BOLA_RAIO + 1.0f;
            break;
        case EVENTO_ESCANTEIO_DIR_CIMA:
            bolaX = MEIO_C - BOLA_RAIO - 1.0f; bolaY = MEIO_L - BOLA_RAIO - 1.0f;
            break;
        case EVENTO_ESCANTEIO_DIR_BAIXO:
            bolaX = MEIO_C - BOLA_RAIO - 1.0f; bolaY = -MEIO_L + BOLA_RAIO + 1.0f;
            break;
        case EVENTO_TOMADA_BOLA:
            break;
    }
    bolaVX = 0.0f; 
    bolaVY = 0.0f;
    bolaEmJogo = true;
    inputsBloqueados = false;
}

void iniciarSequenciaSaida(TipoEvento evento) {
    bolaEmJogo = false;
    inputsBloqueados = true;
    ultimaSaidaX = bolaX;
    ultimaSaidaY = bolaY;
    
    if(ultimaSaidaX > MEIO_C) ultimaSaidaX = MEIO_C;
    if(ultimaSaidaX < -MEIO_C) ultimaSaidaX = -MEIO_C;
    if(ultimaSaidaY > MEIO_L) ultimaSaidaY = MEIO_L;
    if(ultimaSaidaY < -MEIO_L) ultimaSaidaY = -MEIO_L;

    glutTimerFunc(1500, resolverSaidaDeBola, evento);
}

void atualizaFisica(int value) {
    if (!inputsBloqueados) {
        if (teclaW || teclaI || teclaCima)  bolaVY += BOLA_ACEL;
        if (teclaS || teclaK || teclaBaixo) bolaVY -= BOLA_ACEL;
        if (teclaA || teclaJ || teclaEsq)   bolaVX -= BOLA_ACEL;
        if (teclaD || teclaL || teclaDir)   bolaVX += BOLA_ACEL;
    }

    bolaVX *= BOLA_ATRITO;
    bolaVY *= BOLA_ATRITO;

    float velAtual = sqrt(bolaVX * bolaVX + bolaVY * bolaVY);
    if (velAtual > BOLA_MAX_VEL) {
        bolaVX = (bolaVX / velAtual) * BOLA_MAX_VEL;
        bolaVY = (bolaVY / velAtual) * BOLA_MAX_VEL;
    }

    bolaX += bolaVX;
    bolaY += bolaVY;

    if (executandoChapeu) {
        tempoAtualChapeu += 16.0f / 1000.0f;
        if (tempoAtualChapeu >= DURACAO_CHAPEU_SEC) {
            executandoChapeu = false;
            tempoAtualChapeu = 0.0f;
        }
    } else {
        if (velAtual > 0.5f && bolaEmJogo && !inputsBloqueados) {
            progressoChapeu += (16.0f / 1000.0f) / TEMPO_CHAPEU_SEC;
            if (progressoChapeu > 1.0f) progressoChapeu = 1.0f;
        }
    }

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
        if (bolaY < GOL_L / 2.0f && bolaY > -GOL_L / 2.0f) {
            if (bolaX - BOLA_RAIO < -MEIO_C - GOL_PROF) {
                bolaX = -MEIO_C - GOL_PROF + BOLA_RAIO;
                bolaVX = 0.0f; bolaVY = 0.0f;
            }
            if (bolaY + BOLA_RAIO > GOL_L / 2.0f) {
                bolaY = GOL_L / 2.0f - BOLA_RAIO;
                bolaVX = 0.0f; bolaVY = 0.0f;
            }
            if (bolaY - BOLA_RAIO < -GOL_L / 2.0f) {
                bolaY = -GOL_L / 2.0f + BOLA_RAIO;
                bolaVX = 0.0f; bolaVY = 0.0f;
            }
        }
    } else if (bolaX > MEIO_C) {
        if (bolaY < GOL_L / 2.0f && bolaY > -GOL_L / 2.0f) {
            if (bolaX + BOLA_RAIO > MEIO_C + GOL_PROF) {
                bolaX = MEIO_C + GOL_PROF - BOLA_RAIO;
                bolaVX = 0.0f; bolaVY = 0.0f;
            }
            if (bolaY + BOLA_RAIO > GOL_L / 2.0f) {
                bolaY = GOL_L / 2.0f - BOLA_RAIO;
                bolaVX = 0.0f; bolaVY = 0.0f;
            }
            if (bolaY - BOLA_RAIO < -GOL_L / 2.0f) {
                bolaY = -GOL_L / 2.0f + BOLA_RAIO;
                bolaVX = 0.0f; bolaVY = 0.0f;
            }
        }
    }

    if (bolaEmJogo) {
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
    
    desenhaTodosPlacares();
    
    glPushMatrix();
    glTranslatef(OFFSET_CAMPO_X, OFFSET_CAMPO_Y, 0.0f);
    
    campo();
    desenhaBola();
    
    glPopMatrix(); 
    
    glutSwapBuffers(); 
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
    
    glutTimerFunc(16, atualizaFisica, 0);

    glutMainLoop();
    return 0;
}