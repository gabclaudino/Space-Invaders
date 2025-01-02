#ifndef __MOVIMENTOH__
#define __MOVIMENTOH__


void movimentarJogador(Jogador *jogador, ALLEGRO_KEYBOARD_STATE *estado_teclado);

void moverInimigos(Inimigo inimigos[], int totalInimigos, float VELOCIDADE_INIMIGOS);

void MovimentaNaveVermelha(NaveVermelha *nave);


#endif