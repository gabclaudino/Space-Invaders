#ifndef __FUNCSH__
#define __FUNCSH__

#define MAX_TIROS 50
#define LARGURA_TELA 1200
#define ALTURA_TELA 800
#define JOGADOR_ALTURA 30
#define JOGADOR_LARGURA 70
#define INIMIGO_ALTURA 50
#define INIMIGO_LARGURA 50
#define VELOCIDADE_TIRO 20.0
#define VELOCIDADE_TIRO_INIMIGOS 9.0
#define OBSTACULO_LARGURA 100
#define OBSTACULO_ALTURA 100
#define INTERVALO_TROCA_IMAGEM 1.0
#define NAVE_VERMELHA_ALTURA 30
#define NAVE_VERMELHA_LARGURA 60
#define VELOCIDADE_NAVE_VERMELHA 8.0

// enum para representar o tipo do inimigo
typedef enum {
    FORTE,
    INTERMEDIARIO,
    FRACO
} TipoInimigo;

// struct para representar um jogador
typedef struct {
    float x, y;                 // posicao
    int atirando;               // se esta atirando ou nao
    int pontuacao;              // pontuacao total
    int vidas;                  // quantidade de vidas
    double tempoUltimoDisparo;  // tempo que o ultimo disparo foi efetuado
} Jogador;

// struct para representar um inimigo
typedef struct {
    float x, y;             // posicao
    int ativo;              // se esta ativo
    TipoInimigo tipo;       // tipo do inimigo
    float direcao;          // 1 se para direita, -1 para esquerda
    int indice_imagem;      // qual imagem esta mostrando no momento
} Inimigo;

typedef struct {
    float x, y;
    int ativo;
    double tempo;
} NaveVermelha;

// struct para representar um tiro
typedef struct {
    float x, y;     // posicao
    int ativo;      // se esta ativo
    int coluna;
} Tiro;

// struct para representar o tiro do inimigo
typedef struct {
    float x, y;     // posicao
    int ativo;      // se esta ativo
    int tipo;       // tipo do tiro, correspondente ao tipo do inimigo
} TiroInimigo;

// enum para representar os estados do jogo
typedef enum {
    MENU,
    JOGO,
    GAME_OVER
} EstadoJogo;

// struct para representar um obstáculo
typedef struct {
    float x, y;     // posicao
    int vida;       // vida
} Obstaculo;


int colisao(float x1, float y1, float largura1, float altura1, float x2, float y2, float largura2, float altura2);

void inicializarObstaculos(Obstaculo obstaculos[], int quantidade);

void inicializarInimigos(Inimigo inimigos[], int linhas, int colunas);

void inicializarNaveVermelha(NaveVermelha *navev);

void VerificaNaveVermelha(NaveVermelha *nave);

void VerificaInimigoSolo(Inimigo inimigos[], EstadoJogo *estado_jogo);

void dispararTiroJogador(Jogador *jogador, Tiro *tiros, ALLEGRO_KEYBOARD_STATE *estado_teclado);

void atualizarTiro(Tiro *tiros, Inimigo *inimigos, NaveVermelha *nave, Obstaculo *obstaculos, Jogador *jogador, int *exibindo_explosao, float *posicao_x_explosao, float *posicao_y_explosao);

void atualizarTirosInimigos(TiroInimigo *tirosInimigos, Tiro *tiros, Inimigo *inimigos, Obstaculo *obstaculos, Jogador *jogador, double *tempoTiroInimigo, int *exibindo_explosao, float *posicao_x_explosao, float *posicao_y_explosao, EstadoJogo *estado_jogo);

void resetarRodada(Tiro *tiros, TiroInimigo *tirosInimigos, Inimigo *inimigos, NaveVermelha *navev , Obstaculo *obstaculos, Jogador *jogador, int *numeroRodada, float *VELOCIDADE_INIMIGOS);

#endif