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


// funcao para desentar os obstaculos
// recebe como parametro: ponteiro do tipo Obstaculo, ponteiro para textura do obstaculo e ponteiro para fonte
// tipo void
void desenharObstaculos(Obstaculo *obstaculos, ALLEGRO_BITMAP *texturaObstaculo, ALLEGRO_FONT *fonte_menu) {
    for (int i = 0; i < 4; i++) {
        if (obstaculos[i].vida > 0) {                                                       // verifica se o obstaculo tem mais de 0 de vida
            al_draw_bitmap(texturaObstaculo, obstaculos[i].x, obstaculos[i].y, 0);          // desenha o obstaculo
            al_draw_textf(fonte_menu, al_map_rgb(255, 255, 255), obstaculos[i].x + 55, obstaculos[i].y + 105, ALLEGRO_ALIGN_CENTRE, "HP: %d", obstaculos[i].vida);   // escreve na tela a vida
        }
    }
}

// funcao para verificar se esta no hora de trocar a imagem dos inimigos
// recebe como parametro ponteiro do tipo Inimigo, ponteido de double para tempo da ultima troca, e o tempo de intervalo da troca
// tipo void
void verificarTrocaImagemInimigos(Inimigo *inimigos, double *tempo_ultima_troca) {
    if (al_get_time() - *tempo_ultima_troca >= INTERVALO_TROCA_IMAGEM)      // verifica se ja esta na hora de trocar as imagens
    {
        for (int i = 0; i < 45; i++)                                        // troca as imagens de todos inimigos
        {
            if (inimigos[i].ativo)                                          // verifica se esta ativo
            {
                if(inimigos[i].indice_imagem == 1)                          // alterna entre imagem 0 e 1
                    inimigos[i].indice_imagem = 0;
                else
                    inimigos[i].indice_imagem = 1;
                *tempo_ultima_troca = al_get_time();                        // atualiza o tempo de ultima troca
            }
        }
    }
}

// funcao para desenhar os inimigos
// recebe como paremtro: ponteiro do tipo Inimigo e as texturas 1 e 2 dos inimigos
void desenharInimigos(Inimigo *inimigos, ALLEGRO_BITMAP *texturaInimigoForte1, ALLEGRO_BITMAP *texturaInimigoForte2,
                      ALLEGRO_BITMAP *texturaInimigoIntermediario1, ALLEGRO_BITMAP *texturaInimigoIntermediario2,
                      ALLEGRO_BITMAP *texturaInimigoFraco1, ALLEGRO_BITMAP *texturaInimigoFraco2) {
    for (int i = 0; i < 45; i++)                                            // percorre todos os inimigos              
    {
        if (inimigos[i].ativo)                                              // verifica se esta ativo
        {   
            ALLEGRO_BITMAP *texturaInimigo;                                 // bitmap para textura do inimigo

            switch (inimigos[i].tipo)                                       // verifica o tipo do inimigo
            {
                case FORTE:
                    if(inimigos[i].indice_imagem == 1)                      // verifica qual imagem deve ser colocada
                        texturaInimigo = texturaInimigoForte1;
                    else
                        texturaInimigo = texturaInimigoForte2;
                    break;
                case INTERMEDIARIO:
                    if(inimigos[i].indice_imagem == 1)                      // verifica qual imagem deve ser colocada
                        texturaInimigo = texturaInimigoIntermediario1;
                    else
                        texturaInimigo = texturaInimigoIntermediario2;
                    break;
                case FRACO:
                    if(inimigos[i].indice_imagem == 1)                      // verifica qual imagem deve ser colocada
                        texturaInimigo = texturaInimigoFraco1;
                    else
                        texturaInimigo = texturaInimigoFraco2;
                    break;
            }

            al_draw_bitmap(texturaInimigo, inimigos[i].x, inimigos[i].y, 0);    // desenha o inimigo
        }
    }
}

// funcao para desenhar a explosao
// recebe como parametro: a textura da explosao, a posicao em x, a posicao em y, int para verificar a exibicao e int para o tempo
// tipo void
void desenharExplosao(ALLEGRO_BITMAP *imagemExplosao, float posicao_x_explosao, float posicao_y_explosao, int *exibindo_explosao, double *tempo_explosao) {
    if (*exibindo_explosao)                                                             //verifica se pode exibir a explosao
    {
        al_draw_bitmap(imagemExplosao, posicao_x_explosao, posicao_y_explosao, 0);      // desenha a explosao

        *tempo_explosao += 1.0 / 60;                                                    // atualiza o tempo da explosao

        if (*tempo_explosao >= 0.3)                                                     // se ja deu o tmepo, pare a explosao
        {
            *exibindo_explosao = 0;                                                     // para a exibicao
            *tempo_explosao = 0.0;                                                      // reinicia o temporizador
        }
    }
}

// funcao para desenhar os tiros da nave
// recebe como parametro os Tiros
// tipo void
void desenharTirosNave(Tiro *tiros) {
    for(int i = 0; i < 50; i++){
        if (tiros[i].ativo) {                          // verifica se o tiro esta ativo
            al_draw_filled_rectangle(tiros[i].x, tiros[i].y, tiros[i].x + 4, tiros[i].y + 9, al_map_rgb(255, 255, 255));    // se sim desenha na tela
        }
    }
}

// funcao para desenhar os tiros inimigos
// recebe como parametro: vetor dos Tiros inimigos, e suas texturas
// tipo void
void desenharTirosInimigos(TiroInimigo tirosInimigos[], ALLEGRO_BITMAP *texturaTiroForte, ALLEGRO_BITMAP *texturaTiroIntermediario, ALLEGRO_BITMAP *texturaTiroFraco) {
    for (int i = 0; i < 2; i++) 
    {
        if (tirosInimigos[i].ativo)                             // verifica se o tiro esta ativo
        {
            ALLEGRO_BITMAP* imagemTiro = NULL;
            switch (tirosInimigos[i].tipo)                      // pega a textura adequada ao tipo de tiro
            {
                case 1:
                    imagemTiro = texturaTiroForte;
                    break;
                case 2:
                    imagemTiro = texturaTiroIntermediario;
                    break;
                case 3:
                    imagemTiro = texturaTiroFraco;
                    break;
            }
            al_draw_bitmap(imagemTiro, tirosInimigos[i].x, tirosInimigos[i].y, 0);  // desenha o tiro
        }
    }
}

void desenharNaveVermelha(NaveVermelha *nave, ALLEGRO_BITMAP *texturaNaveVermelha){
    if(nave->ativo)
    {
        al_draw_bitmap(texturaNaveVermelha, nave->x, nave->y, 0);
    }
}
