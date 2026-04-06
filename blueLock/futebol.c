#include <GL/glut.h>
#include <math.h>

#define PI 3.14159265
#define PREENCHIDO 1
#define BORDA 0

const float L_CAMPO = 200.0f;
const float A_CAMPO = 129.5f;
const float M_X = 100.5f;
const float M_Y = 80.0f;

void init(){
    glClearColor(0.0, 0.0, 0.0, 0.0); //define a cor do fundo
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-115.0, 115.0, -80.0, 150.0);
}

void arquibancada(){
    float inicioY = 75.0f; // Um pouco acima da linha lateral do campo
    float alturaDegrau = 20.0f;
    int qtd = 5;

    for (int i = 0; i < qtd; i++) {
        // Alterna tons de cinza para parecer degraus
        if (i % 2 == 0) glColor3f(0.35f, 0.35f, 0.35f);
        else glColor3f(0.25f, 0.25f, 0.25f);

        float y_base = inicioY + (i * alturaDegrau);
        glRectf(-130.0f, y_base, 130.0f, y_base + alturaDegrau);
    }
}

void desenhaCirculo(float x_centro, float y_centro, float raio, int segmentos, float angulo_inicio, float angulo_fim, float eixo_a, float eixo_b, int caminho){
    
    if (caminho == BORDA){
        glBegin(GL_LINE_STRIP);
            for (int i = 0; i <= segmentos; i++){
                float angulo = (angulo_inicio + (angulo_fim - angulo_inicio) * i / segmentos) * PI / 180.0;
                glVertex2f((x_centro + raio * cos(angulo)) / eixo_a, (y_centro + raio * sin(angulo)) / eixo_b);
            }
        glEnd();
    }
    else if (caminho == PREENCHIDO){
        glBegin(GL_POLYGON);
            for (int i = 0; i < segmentos; i++) {
            // Ângulo em radianos: (i / total) * 2 * PI
                double angulo = 2.0 * PI * i / segmentos;
        
                double x = (raio * cos(angulo)) + x_centro;
                double y = (raio * sin(angulo)) + y_centro;
                glVertex2d(x/eixo_a, y/eixo_b);
            }
        glEnd();
    }
}

void campo(){

    // 1. Gramado (Fundo Verde)
    glColor3f(0.133f, 0.545f, 0.133f);
    glRectf(-M_X, -M_Y + 10, M_X, M_Y);

    // 2. Linhas Brancas
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.0f);

    // Borda Externa
    glBegin(GL_LINE_LOOP);
        glVertex2f(-M_X, -M_Y + 10);
        glVertex2f( M_X, -M_Y + 10);
        glVertex2f( M_X,  M_Y);
        glVertex2f(-M_X,  M_Y);
    glEnd();

    // Linha de Meio Campo
    glBegin(GL_LINES);
        glVertex2f(0.0f, -M_Y + 10);
        glVertex2f(0.0f,  M_Y);
    glEnd();

    // Círculo Central Perfeitamente Redondo (Raio 17.4px, 100 segmentos)
    desenhaCirculo(0, 0, 17.4f, 100, 0, 360, 1.75, 1, BORDA);
    
    // Ponto Central
    desenhaCirculo(0, 0, 2, 60, 0, 360, 1.75, 1, PREENCHIDO); 

    // --- LADO ESQUERDO ---
    // Grande Área (31.4 x 76.8)
    glBegin(GL_LINE_STRIP);
        glVertex2f(-100.0f,  38.4f);
        glVertex2f(-68.6f,   38.4f);
        glVertex2f(-68.6f,  -38.4f);
        glVertex2f(-100.0f, -38.4f);
    glEnd();

    // Pequena Área (10.5 x 34.9)
    glBegin(GL_LINE_STRIP);
        glVertex2f(-100.0f,  17.45f);
        glVertex2f(-89.5f,   17.45f);
        glVertex2f(-89.5f,  -17.45f);
        glVertex2f(-100.0f, -17.45f);
    glEnd();

    // Marca do Pênalti
    desenhaCirculo(-140, 0, 2, 60, 0, 360, 1.75, 1, PREENCHIDO); 

    // Meia-Lua Esquerda (Centro no Pênalti: -79.0, Raio 17.4)
    desenhaCirculo(-79.0f, 0, 17.4f, 50, -53, 53, 1, 1, BORDA);

    // Gol Esquerdo (Profundidade 4.6, Largura traves 13.9)
    glBegin(GL_LINE_STRIP);
        glVertex2f(-100.0f, -6.95f);
        glVertex2f(-104.6f, -6.95f);
        glVertex2f(-104.6f,  6.95f);
        glVertex2f(-100.0f,  6.95f);
    glEnd();

    // --- LADO DIREITO (Espelhado) ---
    // Grande Área
    glBegin(GL_LINE_STRIP);
        glVertex2f(100.0f,  38.4f);
        glVertex2f(68.6f,   38.4f);
        glVertex2f(68.6f,  -38.4f);
        glVertex2f(100.0f, -38.4f);
    glEnd();

    // Pequena Área
    glBegin(GL_LINE_STRIP);
        glVertex2f(100.0f,  17.45f);
        glVertex2f(89.5f,   17.45f);
        glVertex2f(89.5f,  -17.45f);
        glVertex2f(100.0f, -17.45f);
    glEnd();

    // Marca do Pênalti
    desenhaCirculo(140, 0, 2, 60, 0, 360, 1.75, 1, PREENCHIDO); 

    // Meia-Lua Direita
    desenhaCirculo(79.0f, 0, 17.4f, 50, 127, 233, 1, 1, BORDA);

    // Gol Direito
    glBegin(GL_LINE_STRIP);
        glVertex2f(100.0f, -6.95f);
        glVertex2f(104.6f, -6.95f);
        glVertex2f(104.6f,  6.95f);
        glVertex2f(100.0f,  6.95f);
    glEnd();

    // Escanteios (Raio 1.9)
    desenhaCirculo(-100.5, -67.75, 5, 15, -10, 90, 1, 1, BORDA);    // Inf. Esq
    desenhaCirculo( 100, -64.75, 1.9, 15, 90, 180, 1, 1, BORDA);  // Inf. Dir
    desenhaCirculo( 100,  64.75, 1.9, 15, 180, 270, 1, 1, BORDA); // Sup. Dir
    desenhaCirculo(-100,  64.75, 1.9, 15, 270, 360, 1, 1, BORDA); // Sup. Esq

    glFlush();

}

void geral(){
    
    glClear(GL_COLOR_BUFFER_BIT); //deixa tudo verde
    //arquibancada();
    campo();

    
    glFlush();
}

int main(int argc, char** argv){

    glutInit(&argc, argv);

    
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutCreateWindow("futebol");
    glutFullScreen();

    init();

    glutDisplayFunc(geral);

    glutMainLoop();
    return 0;
}
