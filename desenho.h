#ifndef __DESENHOH__
#define __DESENHOH__

void desenharObstaculos(Obstaculo *obstaculos, ALLEGRO_BITMAP *texturaObstaculo, ALLEGRO_FONT *fonte_menu);

void verificarTrocaImagemInimigos(Inimigo *inimigos, double *tempo_ultima_troca);

void desenharInimigos(Inimigo *inimigos, ALLEGRO_BITMAP *texturaInimigoForte1, ALLEGRO_BITMAP *texturaInimigoForte2, ALLEGRO_BITMAP *texturaInimigoIntermediario1, ALLEGRO_BITMAP *texturaInimigoIntermediario2, ALLEGRO_BITMAP *texturaInimigoFraco1, ALLEGRO_BITMAP *texturaInimigoFraco2);

void desenharExplosao(ALLEGRO_BITMAP *imagemExplosao, float posicao_x_explosao, float posicao_y_explosao, int *exibindo_explosao, double *tempo_explosao);

void desenharTirosNave(Tiro *tiros);

void desenharTirosInimigos(TiroInimigo tirosInimigos[], ALLEGRO_BITMAP *texturaTiroForte, ALLEGRO_BITMAP *texturaTiroIntermediario, ALLEGRO_BITMAP *texturaTiroFraco);

void desenharNaveVermelha(NaveVermelha *nave, ALLEGRO_BITMAP *texturaNaveVermelha);

#endif