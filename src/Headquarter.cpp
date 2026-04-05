#include "Headquarter.h"
#include "City.h"
#include "Clock.h"
#include "Defs.h"
#include "Warrior.h"
#include <cstdio>
#include <iostream>

bool Headquarter::buildWarrior() {
  Warrior_Type current_warrior = order[buildIndex];
  int consume_hp = initial_hp[current_warrior];
  if (lifeUnit >= consume_hp) {
    lifeUnit -= consume_hp;
    Warrior *newWarrior = nullptr;
    switch (current_warrior) {
    case DRAGON:
      newWarrior = new Dragon(totalWarriorNum + 1, this);
      break;
    case NINJA:
      newWarrior = new Ninja(totalWarriorNum + 1, this);
      break;
    case ICEMAN:
      newWarrior = new Iceman(totalWarriorNum + 1, this);
      break;
    case LION:
      newWarrior = new Lion(totalWarriorNum + 1, this);
      break;
    case WOLF:
      newWarrior = new Wolf(totalWarriorNum + 1, this);
      break;
    case WARRIOR_NUM:
      cout << "Error\n";
      break;
    }
    totalWarriorNum++;
    clocktime.printTime();
    printf("%s %s %d born\n", name.c_str(),
           WARRIOR_NAMES[current_warrior].c_str(), totalWarriorNum);
    if (current_warrior == LION) {
      printf("Its loyalty is %d\n",
             static_cast<Lion *>(newWarrior)->getLoyalty());
    }
    if (name == "red") {
      base_city->redWarrior = newWarrior;
    }
    if (name == "blue") {
      base_city->blueWarrior = newWarrior;
    }
    buildIndex = (buildIndex + 1) % WARRIOR_NUM;
    return true;
  }
  isEmpty = true;
  return false;
}
Headquarter::~Headquarter() {}
