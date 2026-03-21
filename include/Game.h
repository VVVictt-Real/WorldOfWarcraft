#ifndef GAME_H
#define GAME_H

#include "Defs.h"
#include "Headquarter.h"
#include "Warrior.h"

class Game {
private:
  int clocktime;
  Headquarter red_headquarter;
  Headquarter blue_headquarter;

public:
  Game(int lifeUnit)
      : red_headquarter("red", lifeUnit, RED_ORDER),
        blue_headquarter("blue", lifeUnit, BLUE_ORDER) {
    clocktime = 0;
  }
  void run() {
    while (!(red_headquarter.is_empty() && blue_headquarter.is_empty())) {
      if (!red_headquarter.is_empty()) {
        printf("%03d ", clocktime);
        if (!red_headquarter.buildWarrior()) {
          printf("red headquarter stops making warriors\n");
        }
      }
      if (!blue_headquarter.is_empty()) {
        printf("%03d ", clocktime);
        if (!blue_headquarter.buildWarrior()) {
          printf("blue headquarter stops making warriors\n");
        }
      }
      clocktime++;
    }
  }
};
void game();

#endif // !GAME_H
