#include "Game.h"
#include "Defs.h"
#include <iostream>

void game() {
  int M;
  cin >> M;
  for (int i = 0; i < 5; i++) {
    cin >> initial_hp[i];
  }
  Game new_game(M);
  new_game.run();
}
