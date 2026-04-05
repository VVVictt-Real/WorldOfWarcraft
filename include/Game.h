#ifndef GAME_H
#define GAME_H

#include "City.h"
#include "Clock.h"
#include "Headquarter.h"
#include "Warrior.h"
#include <vector>

class Game {
private:
  Headquarter red_headquarter;
  Headquarter blue_headquarter;
  vector<City> cities;
  int cityNum;
  Clock endtime;

public:
  Game(int lifeUnit, int N, int T);
  void run();
  bool move();
  ~Game();
};
void game();

#endif // !GAME_H
