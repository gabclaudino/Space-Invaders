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
#include "movimento.h"




// funcao para movimentar o jogador para direita ou para esquerda
// recebe como parametro: ponteiro do tipo Jogador e um ponteiro para o estado do teclado
// tipo void
void movimentarJogador(Jogador *jogador, ALLEGRO_KEYBOARD_STATE *estado_teclado) {
    // moviemnta para esquerda de pressionado a seta para esquerda
    if (al_key_down(estado_teclado, ALLEGRO_KEY_LEFT) && jogador->x > 0)                                   
        jogador->x -= 15;
    // moviemnta para direita de pressionado a seta para direita
    if (al_key_down(estado_teclado, ALLEGRO_KEY_RIGHT) && jogador->x < LARGURA_TELA - JOGADOR_LARGURA) 
        jogador->x += 15;
}

// funcao para movimentar os inimigos
// recebe como parametro: vetor do tipo Inimigo, a quantiade total de inimigos e sua velocidade
void moverInimigos(Inimigo inimigos[], int totalInimigos, float VELOCIDADE_INIMIGOS) {
    int atingiuBorda = 0;                                                               // var para verificar se atingiu a borda

    for (int i = 0; i < totalInimigos; i++) 
    {           
        if (inimigos[i].ativo)                                                          // veririca se o inimigo esta ativo
        {                                                        
            inimigos[i].x += VELOCIDADE_INIMIGOS * inimigos[i].direcao;                 // reposiciona o inimigo para direcao

            if (inimigos[i].x < 0 || inimigos[i].x > LARGURA_TELA - INIMIGO_LARGURA)    // verifica se o inimigo atingiu a borda
            {
                atingiuBorda = 1;
                break;
            }
        }
    }

    if (atingiuBorda) 
    {                                                                 // verifica se a borda foi atingida
        for (int i = 0; i < totalInimigos; i++) 
        {   
            if (inimigos[i].ativo) 
            {
                inimigos[i].y += 20;                                                    // desloca para baixo
                inimigos[i].direcao *= -1.0;                                            // inverte a direcao
            }
        }
    }

}

void MovimentaNaveVermelha(NaveVermelha *nave){
    if(nave->ativo)
    {
        nave->x += VELOCIDADE_NAVE_VERMELHA;

        if(nave->x > LARGURA_TELA - NAVE_VERMELHA_LARGURA)
        {
            nave->ativo = 0;
        }
    }
}