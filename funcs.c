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

// funcao para verificar colisao
int colisao(float x1, float y1, float largura1, float altura1, float x2, float y2, float largura2, float altura2) {
    return (x1 < x2 + largura2 && x1 + largura1 > x2 && y1 < y2 + altura2 && y1 + altura1 > y2);
}

// funcao para iniciar os osbstaculos
// recebe como parametro: um vetor do tipo Obstaculo e a quantidade de obstaculos
// tipo void
void inicializarObstaculos(Obstaculo obstaculos[], int quantidade) {
    for (int i = 0; i < quantidade; i++) 
    {
        obstaculos[i].x = i * 325 + 50;                             // posiciona no eixo x
        obstaculos[i].y = ALTURA_TELA - JOGADOR_ALTURA - 200;       // posiciona no eixo y
        obstaculos[i].vida = 10;                                    // define a vida
    }
}

// funcao para iniciar os inimigos
// recebe como parametro: um vetor do tipo Inimigo, a quantidade de linhas e a quantidade de colunas
// tipo void
void inicializarInimigos(Inimigo inimigos[], int linhas, int colunas) {
    for (int linha = 0; linha < linhas; linha++) 
    {
        for (int coluna = 0; coluna < colunas; coluna++) 
        {
            int index = linha * colunas + coluna;               // calcula a posicao do inimigo no vetor
            inimigos[index].x = coluna * 80 + 50;               // posiciona o inimigo no eixo x
            inimigos[index].y = linha * 50 + 120;               // posiciona o inimigo no exio y
            inimigos[index].ativo = 1;                          // define inimigo como ativo
            inimigos[index].direcao = 1.0;                      // define a direcao, comeca indo para direita
            inimigos[index].indice_imagem = 0;                  // comeca com a imagem 0

            // definindo o tipo de inimigo com base na linha
            if (linha == 0) 
            {
                inimigos[index].tipo = FORTE;                   // se for na primeira linha eh um inimigo FORTE
            } else if (linha == 1 || linha == 2) 
            {
                inimigos[index].tipo = INTERMEDIARIO;           // se for na segunda ou na terceira eh INTERMEDIARIO
            } else 
            {
                inimigos[index].tipo = FRACO;                   // se for na quarta ou quinta eh FRACO
            }
        }
    }
}

void inicializarNaveVermelha(NaveVermelha *navev){
    navev->ativo = 0;
    navev->tempo = 0;
    navev->x = 30;
    navev->y = 200;
}

void VerificaNaveVermelha(NaveVermelha *nave){
    if(al_get_time() - nave->tempo >= 30)
    {
        nave->x = NAVE_VERMELHA_LARGURA;
        nave->y = NAVE_VERMELHA_LARGURA + 20;
        nave->ativo = 1;
        nave->tempo = al_get_time();
    }
}

// funcao para verificar se algum inimigo atingiu o solo
void VerificaInimigoSolo(Inimigo inimigos[], EstadoJogo *estado_jogo){
    for (int i = 0; i < 45; i++) 
    {
        if (inimigos[i].ativo && inimigos[i].y >= ALTURA_TELA) {    // se algum inimigo atingiu o solo
            *estado_jogo = GAME_OVER;                               // entao GAME OVER
            break; 
        }
    }
}

// funcao para disparar um tiro do jogador
void dispararTiroJogador(Jogador *jogador, Tiro *tiros, ALLEGRO_KEYBOARD_STATE *estado_teclado) {
    // verifica se a tecla espaco foi acionada se se o jogador ja pode efetuar outro disparo
    if ((al_key_down(estado_teclado, ALLEGRO_KEY_SPACE)) && (al_get_time() - jogador->tempoUltimoDisparo >= 0.75))
    {
        for (int i = 0; i < MAX_TIROS; i++)                                     // verifica se ha outro tiro na mesma coluna
        {
            if (!tiros[i].ativo) 
            {
                int colunaTiro = jogador->x;                                    // considera a posicao de x como coluna
                for (int j = 0; j < MAX_TIROS; j++) 
                {
                    if (tiros[j].ativo && tiros[j].coluna == colunaTiro) {      // se ha outro tiro, entao da um return
                        return;
                    }
                }
                tiros[i].x = jogador->x + JOGADOR_LARGURA / 2;                  // se a coluna esta livre, entao o jogador pode disparar
                tiros[i].y = jogador->y;                                        // posiciona o tiro
                tiros[i].ativo = 1;                                             // define como ativo
                tiros[i].coluna = colunaTiro;                                   // define a coluna do tiro
                jogador->tempoUltimoDisparo = al_get_time();                    // pega o tempo do ultimo disparo
                break;
            }
        }
    }
}

// funcao para atualizar o tiro da nave
// recebe como parametro: um ponteiro do tipo Tiro, um ponteiro do tipo Obstaculo, um ponteiro do tipo Jogador,
//                        um ponteiro de inteiro para exibindo a explosao, um ponteido de float para posicao x da explosao e
//                        um ponteiro de float para posicao y da explosao
// tipo void
void atualizarTiro(Tiro *tiros, Inimigo *inimigos, NaveVermelha *nave, Obstaculo *obstaculos, Jogador *jogador, int *exibindo_explosao, float *posicao_x_explosao, float *posicao_y_explosao) {
    
    for(int i = 0; i < 50; i++)
    {
        if (tiros[i].ativo)                                               // verifica se o tiro esta ativo
        {
            tiros[i].y -= VELOCIDADE_TIRO;                                // atualiza posicao vertical do tiro
            if (tiros[i].y < 0)                                           // verifica se o tiro saiu da tela
            {
                tiros[i].ativo = 0;                                       // desativa o tiro
                jogador->atirando = 0;                                  // libera para novo tiro
            }

            for (int j = 0; j < 45; j++)                                // verifica colisoes com inimigos
            {
                if (inimigos[j].ativo && colisao(tiros[i].x, tiros[i].y, 4, 9, inimigos[j].x, inimigos[j].y, INIMIGO_LARGURA, INIMIGO_ALTURA)) 
                {
                    tiros[i].ativo = 0;                                   // se colisao com inimigo desativa o tiro
                    jogador->atirando = 0;                              // libera para novo tiro
                    inimigos[j].ativo = 0;                              // desativa o inimigo
                    switch (inimigos[j].tipo)                           // atualiza a pontuacao
                    {
                        case FORTE:                                     
                            jogador->pontuacao += 40;       
                            break;
                        case INTERMEDIARIO:
                            jogador->pontuacao += 20;
                            break;
                        case FRACO:
                            jogador->pontuacao += 10;
                            break;
                    }
                    *exibindo_explosao = 1;                             // exibe a explosao
                    *posicao_x_explosao = inimigos[j].x;                // marca a posicao x da explosao
                    *posicao_y_explosao = inimigos[j].y;                // marca a posicao y da explosao
                }   
            }

            for (int k = 0; k < 4; k++)                                 // verifica colisoes com obstaculos
            {
                if (obstaculos[k].vida > 0 && colisao(tiros[i].x, tiros[i].y, 4, 9, obstaculos[k].x, obstaculos[k].y, OBSTACULO_LARGURA, OBSTACULO_ALTURA)) 
                {
                    tiros[i].ativo = 0;                                 // se colidiu desativa o tiro
                    jogador->atirando = 0;                              // libera para novo tiro
                    obstaculos[k].vida -= 1;                            // atualiza vida do obstaculo
                }
            }

            // verifica colisao com nave vermelha
            if(nave->ativo && colisao(tiros[i].x, tiros[i].y, 4, 9, nave->x, nave->y, NAVE_VERMELHA_LARGURA, NAVE_VERMELHA_ALTURA))
            {
                tiros[i].ativo = 0;
                jogador->atirando = 0;
                jogador->pontuacao += 250;
                nave->ativo = 0;
                *exibindo_explosao = 1;
                *posicao_x_explosao = nave->x;
                *posicao_y_explosao = nave->y;
            }
        }
    }

}

// funcao para atualizar os tiros dos inimigos
// recebe como parametro: um ponteiro do tipo TiroInimigo, um ponteiro do tipo Obstaculo, um ponteiro do tipo Jogador,
//                        um ponteiro de double para o tempo do tio inimigo, um ponteiro de int para exibindo explosao
//                        um ponteiro de float para posicao x da explosao, um ponteiro de float para posicao y da explosao
//                        um ponteiro para o estado do jogo
// tipo void
void atualizarTirosInimigos(TiroInimigo *tirosInimigos, Tiro *tiros, Inimigo *inimigos, Obstaculo *obstaculos, Jogador *jogador, double *tempoTiroInimigo, int *exibindo_explosao, float *posicao_x_explosao, float *posicao_y_explosao, EstadoJogo *estado_jogo) {
    if (*tempoTiroInimigo >= 1.0)                                      // verifica se ja esta na hora de atirar novamente
    {
        int inimigosAtirando[2];        // vetor para guardar os inimigos atirantes
        inimigosAtirando[0] = -1;       // inicializa com -1 para dizer que nao ha atirador no momento
        inimigosAtirando[1] = -1;

        float distanciaXInimigos[45];                                       // vetor par aarmazenar as distancias dos inimigos ate a nave
        for (int j = 0; j < 45; ++j) {
            if (inimigos[j].ativo) {
                distanciaXInimigos[j] = fabs(jogador->x - inimigos[j].x);   // pega o modulo da distancia
            }
        }

        float menorDistancia = FLT_MAX;                                         // seleciona o inimigo mais perto, define a menor distancia como infinito de inicio
        for (int j = 0; j < 45; ++j) {
            if (inimigos[j].ativo && distanciaXInimigos[j] < menorDistancia) { 
                menorDistancia = distanciaXInimigos[j];
                inimigosAtirando[0] = j;
            }
        }

        for(int i = inimigosAtirando[0] + 1; i < 45; i++)   // seleciona um inimigo a direita do primeiro atirador
        {
            if(inimigos[i].ativo)
            {
                inimigosAtirando[1] = i;
                break;
            }
        } 

        if(inimigosAtirando[1] == -1)                           // se nao ha inimigo a direita, entao escolhe um da direita
        {
            for(int i = inimigosAtirando[0] - 1; i >= 0; i--)
            {
                if(inimigos[i].ativo)
                {
                    inimigosAtirando[1] = i;
                    break;
                }
            }
        }


        for (int k = 0; k < 2; k++)         // inimigos atiam
        {
            if (inimigosAtirando[k] != -1) 
            {
                for (int j = 0; j < 2; j++)                                                            
                {
                    if (!tirosInimigos[j].ativo) 
                    {
                        tirosInimigos[j].x = inimigos[inimigosAtirando[k]].x + INIMIGO_LARGURA / 2;     // posiciona o tiro no eixo x
                        tirosInimigos[j].y = inimigos[inimigosAtirando[k]].y + INIMIGO_ALTURA;          // posiciona o tiro no eixo y
                        tirosInimigos[j].ativo = 1;                                                     // define como ativo
                        switch (inimigos[inimigosAtirando[k]].tipo)                                     // define o tipo do tiro
                        {
                            case FORTE:
                                tirosInimigos[j].tipo = 3;
                                break;
                            case INTERMEDIARIO:
                                tirosInimigos[j].tipo = 2;
                                break;
                            case FRACO:
                                tirosInimigos[j].tipo = 1;
                                break;
                        }
                        break;
                    }
                }
            }
        }
        *tempoTiroInimigo = 0;                          // reinicia o temporizador
    }

    // atualizando posicao dos tiros de inimigos e verificando colisoes
    for (int i = 0; i < 2; i++) 
    {
        if (tirosInimigos[i].ativo) 
        {
            tirosInimigos[i].y += VELOCIDADE_TIRO_INIMIGOS;             // atualiza a posicao vertical do tiro inimigo
            if (tirosInimigos[i].y > ALTURA_TELA)                       // verifica se o tiro saiu da tela
            {
                tirosInimigos[i].ativo = 0;                             // se sim desativa o tiro
            } 
            else if (colisao(tirosInimigos[i].x, tirosInimigos[i].y, 10, 25, jogador->x, jogador->y, JOGADOR_LARGURA, JOGADOR_ALTURA)) 
            {
                // colisao da nave com tiro de inimigo
                tirosInimigos[i].ativo = 0;             // desativa o tiro
                jogador->vidas--;                       // diminui a vida do jogador
                *exibindo_explosao = 1;                 // ativa a exibicao da exlosao
                *posicao_x_explosao = jogador->x;       // marca a posicao x da explosao
                *posicao_y_explosao = jogador->y;       // marca a posicao y da explosao

                if (jogador->vidas <= 0)                // se acabou as vidas, entao GAME OVER
                    *estado_jogo = GAME_OVER;           // atualiza o estado do jogo para GAME OVER
            }

            // verificando colisoes com obstaculos
            for (int j = 0; j < 4; j++) 
            {
                if (obstaculos[j].vida > 0 && colisao(tirosInimigos[i].x, tirosInimigos[i].y, 10, 25, obstaculos[j].x, obstaculos[j].y, OBSTACULO_LARGURA, OBSTACULO_ALTURA)) 
                {
                    tirosInimigos[i].ativo = 0;                                         // se colidiu com obstaculo, desativa o tiro
                    if (tirosInimigos[i].tipo == 1)                                     // verifica o tipo do tiro para diminuir a vida do obstaculo
                    {
                        obstaculos[j].vida -= 1;                                        // diminui a vida do obstaculo
                    }
                    else if(tirosInimigos[i].tipo == 2 || tirosInimigos[i].tipo == 3)
                    {
                        obstaculos[j].vida -= 2;                                        // diminui a vida do obstaculo
                    }
                }
            }

            // verificando colisoes com tiros
            for(int j = 0; j < MAX_TIROS; j++)
            {
                if(tiros[j].ativo && tirosInimigos[i].ativo && colisao(tirosInimigos[i].x, tirosInimigos[i].y, 10, 25, tiros[j].x, tiros[j].y, 4, 9))
                {
                    tirosInimigos[i].ativo = 0;
                    tiros[j].ativo = 0;
                }

            }
        }
    }
}

// funcao para resetar a rodada caso elimine todos os inimigos
// recebe como parametro: ponteiro do tipo TiroInimigo, ponteiro do tipo Obstaculo, ponteiro do tipo Jogador
//                        ponteiro de int para o numero da rodada, ponteiro para float para velociade dos inimigos
void resetarRodada(Tiro *tiros, TiroInimigo *tirosInimigos, Inimigo *inimigos, NaveVermelha *navev , Obstaculo *obstaculos, Jogador *jogador, int *numeroRodada, float *VELOCIDADE_INIMIGOS) {
    for(int i = 0; i < MAX_TIROS; i++)
    {
        tiros[i].ativo = 0;
    } 

    tirosInimigos[0].ativo = 0;         // desativa os tiros inimigos
    tirosInimigos[1].ativo = 0;

    (*numeroRodada)++;                  // incrementa o numero da rodada

    if (jogador->vidas < 5)             // adiciona vidas ao jogador
        jogador->vidas++;

    (*VELOCIDADE_INIMIGOS) += 0.6;      // aumenta a velocidade dos inimigos

    navev->tempo = al_get_time();       // reseta o tempo para aparecer a nave inimiga
    navev->ativo = 0;

    for (int linha = 0; linha < 5; linha++)                     // reinicializa os inimigos
    {
        for (int coluna = 0; coluna < 9; coluna++)              
        {   
            int index = linha * 9 + coluna;                     // calcula posicao do inimigo no vetor
            inimigos[index].x = coluna * 70 + 50;               // posiciona em x
            inimigos[index].y = linha * 50 + 120;               // posiciona em y
            inimigos[index].ativo = 1;                          // ativa o inimigo
            inimigos[index].direcao = 1.0;                      // define direcao, comeca para direita
            inimigos[index].indice_imagem = 0;                  // define a imagem como 0

            // definindo o tipo de inimigo com base na linha
            if (linha == 0) 
            {
                inimigos[index].tipo = FORTE;                   // se linha 1 eh FORTE
            } else if (linha == 1 || linha == 2) 
            {
                inimigos[index].tipo = INTERMEDIARIO;           // se linha 2 ou 3 eh INTERMEDIARIO
            } else 
            {
                inimigos[index].tipo = FRACO;                   // se linha 4 ou 5 eh FRACO
            }
        }
    }
}