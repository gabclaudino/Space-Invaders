#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <math.h>
#include <float.h>
#include "funcs.h"
#include "desenho.h"
#include "movimento.h"

int main() {
    // declaracao das variaveis da allegro
    ALLEGRO_DISPLAY *tela = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_KEYBOARD_STATE estado_teclado;
    ALLEGRO_BITMAP *texturaInimigoForte1 = NULL;
    ALLEGRO_BITMAP *texturaInimigoForte2 = NULL;
    ALLEGRO_BITMAP *texturaInimigoIntermediario1 = NULL;
    ALLEGRO_BITMAP *texturaInimigoIntermediario2 = NULL;
    ALLEGRO_BITMAP *texturaInimigoFraco1 = NULL;
    ALLEGRO_BITMAP *texturaInimigoFraco2 = NULL;
    ALLEGRO_BITMAP *texturaNave = NULL;
    ALLEGRO_BITMAP *texturaObstaculo = NULL;
    ALLEGRO_BITMAP *texturaTiroForte = NULL;
    ALLEGRO_BITMAP *texturaTiroIntermediario = NULL;
    ALLEGRO_BITMAP *texturaTiroFraco = NULL;
    ALLEGRO_BITMAP *imagemExplosao = NULL;
    ALLEGRO_BITMAP *texturaNaveVermelha = NULL;
    
    double tempoTiroNave = 0;           // var para iniciar o tempo do tiro da nave
    double tempoTiroInimigo = 0;        // var para iniciar o tempo do tiro inimigo
    int numeroRodada = 1;               // var para numero da rodada
    double tempo_ultima_troca = 0.0;    // var para o tempo de ultima troca das imagens dos inimigos
    float VELOCIDADE_INIMIGOS = 2.2;    // var para velociade dos inimigos 

    // inicializando Allegro
    if (!al_init()) {
        fprintf(stderr, "Falha ao inicializar o Allegro.\n");
        return -1;
    }

    // inicializando modulos adicionais
    al_init_image_addon();
    al_install_keyboard();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    // criando a tela
    tela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!tela) {
        fprintf(stderr, "Falha ao criar a tela.\n");
        return -1;
    }

    // criando a fila de eventos
    fila_eventos = al_create_event_queue();
    if (!fila_eventos) {
        fprintf(stderr, "Falha ao criar a fila de eventos.\n");
        al_destroy_display(tela);
        return -1;
    }

    // criando o timer
    timer = al_create_timer(1.0 / 20);
    if (!timer) {
        fprintf(stderr, "Falha ao criar o timer.\n");
        al_destroy_event_queue(fila_eventos);
        al_destroy_display(tela);
        return -1;
    }

    // carregando a fonte
    ALLEGRO_FONT *fonte_menu = al_create_builtin_font();
    if (!fonte_menu) {
        fprintf(stderr, "Falha ao carregar a fonte.\n");
        al_destroy_timer(timer);
        al_destroy_event_queue(fila_eventos);
        al_destroy_display(tela);
        return -1;
    }

    // Registrando fontes de eventos
    al_register_event_source(fila_eventos, al_get_display_event_source(tela));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());

    // Inicializando variáveis do jogo
    Jogador jogador = {LARGURA_TELA / 2 - JOGADOR_LARGURA / 2, ALTURA_TELA - JOGADOR_ALTURA - 10, 0, 0, 3, 0.0};     // iniciando jogador
    Inimigo inimigos[45];                     // vetor de inimigos, tamanho de 5 linhas por 9 colunas
    Tiro *tiros;                              // tiro da nave
    TiroInimigo tirosInimigos[2];             // tiro dos inimigos
    tempo_ultima_troca = al_get_time();       // ultima troca das imagens
    
    // inicia nave vermelha
    NaveVermelha NaveVermelha1;
    inicializarNaveVermelha(&NaveVermelha1);

    // inicializando obstaculos
    Obstaculo obstaculos[4];                  // vetor para 4 obstaculos
    inicializarObstaculos(obstaculos, 4);

    // inicializando inimigos
    inicializarInimigos(inimigos, 5, 9);

    // inicializando tiro
    tiros = (Tiro *) malloc (sizeof(Tiro) * 50);
    for (int i = 0; i < 50; i++){
        tiros[i].ativo = 0;
    }

    // inicializando tiros de inimigos
    for (int i = 0; i < 2; ++i) {
        tirosInimigos[i].ativo = 0;
    }

    // Inicializando o timer
    al_start_timer(timer);

    // Estado inicial do jogo
    EstadoJogo estado_jogo = MENU;

    // carregando texturas
    texturaInimigoForte1 = al_load_bitmap("imagens/forte1.png");
    texturaInimigoForte2 = al_load_bitmap("imagens/forte2.png");
    texturaInimigoIntermediario1 = al_load_bitmap("imagens/medio1.png");
    texturaInimigoIntermediario2 = al_load_bitmap("imagens/medio2.png");
    texturaInimigoFraco1 = al_load_bitmap("imagens/fraco1.png");
    texturaInimigoFraco2 = al_load_bitmap("imagens/fraco2.png");
    texturaNave = al_load_bitmap("imagens/nave.png");
    texturaObstaculo = al_load_bitmap("imagens/obstaculo.png");
    texturaTiroForte = al_load_bitmap("imagens/tiroForte.png");
    texturaTiroIntermediario = al_load_bitmap("imagens/tiroIntermediario.png");
    texturaTiroFraco = al_load_bitmap("imagens/tiroFraco.png");
    imagemExplosao = al_load_bitmap("imagens/explosao.png");
    texturaNaveVermelha = al_load_bitmap("imagens/NaveVermelha.png");

    // verifica se todas as texturas foram carregadas
    if (!texturaInimigoForte1 || !texturaInimigoIntermediario1 || !texturaInimigoFraco1 || !texturaNave || !texturaObstaculo || 
        !texturaInimigoForte2 || !texturaInimigoIntermediario2 || !texturaInimigoFraco2 || !texturaTiroForte || 
        !texturaTiroIntermediario || !texturaTiroFraco || !imagemExplosao || !texturaNaveVermelha) {
        fprintf(stderr, "Falha ao carregar as texturas.\n");
        return -1;
    }

    // loop principal do jogo
    while (1) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);
        int exibindo_explosao = 0;
        float posicao_x_explosao;
        float posicao_y_explosao;   
        double tempo_explosao = 0.0; 

        if (evento.type == ALLEGRO_EVENT_TIMER) {
            al_get_keyboard_state(&estado_teclado);

            tempoTiroInimigo += 1.0 / 120;
            tempoTiroNave += 1.0 / 120;

            switch (estado_jogo) {
                case MENU:
                    if (al_key_down(&estado_teclado, ALLEGRO_KEY_ENTER)) {
                        estado_jogo = JOGO;
                    }
                    break;

                case JOGO:
                    // verifica se esta na hora de aparecer a nave vermelha
                    VerificaNaveVermelha(&NaveVermelha1);

                    // movimenta nave vermelha
                    MovimentaNaveVermelha(&NaveVermelha1);

                    // movimentando inimigos
                    moverInimigos(inimigos, 45, VELOCIDADE_INIMIGOS);

                    // movimentando jogador
                    movimentarJogador(&jogador, &estado_teclado);

                    // atirando
                    dispararTiroJogador(&jogador, tiros, &estado_teclado);

                    // atualizando posicao do tiro e verificando colisoes
                    atualizarTiro(tiros, inimigos, &NaveVermelha1, obstaculos, &jogador, &exibindo_explosao, &posicao_x_explosao, &posicao_y_explosao);
                    
                    // inimigos atirando
                    atualizarTirosInimigos(tirosInimigos, tiros, inimigos, obstaculos, &jogador, &tempoTiroInimigo, &exibindo_explosao, &posicao_x_explosao, &posicao_y_explosao, &estado_jogo);

                    // verifica se algum inimigo atingiu o solo
                    VerificaInimigoSolo(inimigos, &estado_jogo);

                    if (estado_jogo == JOGO) 
                    {
                        int inimigosRestantes = 0;

                        // verificando se ainda ha inimigos ativos
                        for (int i = 0; i < 45; i++) 
                        {
                            if (inimigos[i].ativo) 
                            {
                                inimigosRestantes = 1;
                                break;
                            }
                        }

                        // se nao houver inimigos restantes, iniciar uma nova rodada
                        if (!inimigosRestantes) 
                        {
                            resetarRodada(tiros, tirosInimigos, inimigos, &NaveVermelha1, obstaculos, &jogador, &numeroRodada, &VELOCIDADE_INIMIGOS);
                        }
                    }
                    
                    break;

                case GAME_OVER:
                    break;
            }

            // desenhando elementos na tela
            al_clear_to_color(al_map_rgb(0, 0, 0));

            switch (estado_jogo) {
                case MENU:
                    // desenhando as escritas do menu
                    al_draw_text(fonte_menu, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 2, ALLEGRO_ALIGN_CENTRE, "Pressione ENTER para iniciar");
                    al_draw_text(fonte_menu, al_map_rgb(0, 255, 0), LARGURA_TELA / 2, (ALTURA_TELA / 2) - 100, ALLEGRO_ALIGN_CENTRE, "Space Invaders");
                    break;

                case JOGO:

                    // desenhando obstaculos e exibindo vida
                    desenharObstaculos(obstaculos, texturaObstaculo, fonte_menu);

                    // desenhando nave do jogador
                    al_draw_bitmap(texturaNave, jogador.x, jogador.y, 0);

                    // desenhando placar
                    al_draw_textf(fonte_menu, al_map_rgb(255, 255, 255), 10, 10, ALLEGRO_ALIGN_LEFT, "SCORE: ");
                    al_draw_textf(fonte_menu, al_map_rgb(0, 255, 0), 80, 10, ALLEGRO_ALIGN_LEFT, "%d", jogador.pontuacao);

                    // exibir o numero da rodada no centro da tela
                    al_draw_textf(fonte_menu, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA - 790, ALLEGRO_ALIGN_CENTRE, "Rodada %d", numeroRodada);

                    // exibir o numero de vidas
                    al_draw_textf(fonte_menu, al_map_rgb(255, 255, 255), LARGURA_TELA - 10, 10, ALLEGRO_ALIGN_RIGHT, "Vidas: %d", jogador.vidas);

                    // verificar se esta na hora de trocar as imagens dos inimigos
                    verificarTrocaImagemInimigos(inimigos, &tempo_ultima_troca);

                    // desenhando inimigos com imagens
                    desenharInimigos(inimigos, texturaInimigoForte1, texturaInimigoForte2,
                                    texturaInimigoIntermediario1, texturaInimigoIntermediario2,
                                    texturaInimigoFraco1, texturaInimigoFraco2);
                

                    // desenhando explosoes
                    desenharExplosao(imagemExplosao, posicao_x_explosao, posicao_y_explosao, &exibindo_explosao, &tempo_explosao);

                    // desenhando tiros
                    desenharTirosNave(tiros);

                    // desenhando tiros dos inimigos
                    desenharTirosInimigos(tirosInimigos, texturaTiroForte, texturaTiroIntermediario, texturaTiroFraco);

                    // desenhando nave mermelha
                    desenharNaveVermelha(&NaveVermelha1, texturaNaveVermelha);

                    // atualizando a tela
                    al_flip_display();
                    break;

                case GAME_OVER:
                    // desenhando escritas do GAME OVER
                    al_draw_text(fonte_menu, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 2, ALLEGRO_ALIGN_CENTRE, "Game Over");
                    al_draw_textf(fonte_menu, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 2 + 10, ALLEGRO_ALIGN_CENTRE, "SCORE: %d", jogador.pontuacao);
                    break;
            }

            // atualizando a tela
            al_flip_display();

        } 
        else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) 
        {
            break;
        }
    }

    // destruindo
    free(tiros);
    al_destroy_bitmap(texturaInimigoForte1);
    al_destroy_bitmap(texturaInimigoForte2);
    al_destroy_bitmap(texturaInimigoIntermediario1);
    al_destroy_bitmap(texturaInimigoIntermediario2);
    al_destroy_bitmap(texturaInimigoFraco1);
    al_destroy_bitmap(texturaInimigoFraco2);
    al_destroy_bitmap(texturaNave);
    al_destroy_bitmap(texturaObstaculo);
    al_destroy_bitmap(texturaTiroIntermediario);
    al_destroy_bitmap(texturaTiroForte);
    al_destroy_bitmap(texturaTiroFraco);
    al_destroy_bitmap(imagemExplosao);
    al_destroy_font(fonte_menu);
    al_destroy_timer(timer);
    al_destroy_event_queue(fila_eventos);
    al_destroy_display(tela);

    return 0;
}