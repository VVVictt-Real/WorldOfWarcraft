#include "Headquarter.h"
#include "Defs.h"
#include "Warrior.h"
#include <cstdio>
#include <iostream>

bool Headquarter::buildWarrior() {
  for (int i = 0; i < 5; i++) {
    Warrior_Type current_warrior = order[buildIndex];
    int consume_hp = initial_hp[current_warrior];
    if (lifeUnit >= consume_hp) {
      lifeUnit -= consume_hp;
      Warrior *newWarrior = nullptr;
      switch (current_warrior) {
      case DRAGON:
        newWarrior = new Dragon(warrior.size() + 1, this);
        break;
      case NINJA:
        newWarrior = new Ninja(warrior.size() + 1, this);
        break;
      case ICEMAN:
        newWarrior = new Iceman(warrior.size() + 1, this);
        break;
      case LION:
        newWarrior = new Lion(warrior.size() + 1, this);
        break;
      case WOLF:
        newWarrior = new Wolf(warrior.size() + 1, this);
        break;
      case WARRIOR_NUM:
        cout << "Error\n";
        break;
      }
      warrior.push_back(newWarrior);
      printf("%s %s %d born with strength %d,%d %s in %s headquarter\n",
             name.c_str(), WARRIOR_NAMES[current_warrior].c_str(),
             int(warrior.size()), initial_hp[current_warrior],
             newWarrior->get_warrior_num(),
             WARRIOR_NAMES[current_warrior].c_str(), name.c_str());
      switch (current_warrior) {
      case DRAGON:
        printf("It has a %s,and it's morale is %.2lf\n",
               WEAPON_NAMES[(static_cast<Dragon *>(newWarrior))->getWeapon()]
                   .c_str(),
               static_cast<Dragon *>(newWarrior)->getMorale());
        break;
      case NINJA:
        printf("It has a %s and a %s\n",
               WEAPON_NAMES[static_cast<Ninja *>(newWarrior)->getWeapon1()]
                   .c_str(),
               WEAPON_NAMES[static_cast<Ninja *>(newWarrior)->getWeapon2()]
                   .c_str());
        break;
      case ICEMAN:
        printf("It has a %s\n",
               WEAPON_NAMES[static_cast<Iceman *>(newWarrior)->getWeapon()]
                   .c_str());
        break;
      case LION:
        printf("It's loyalty is %d\n",
               static_cast<Lion *>(newWarrior)->getLoyalty());
        break;
      case WOLF:
        break;
      case WARRIOR_NUM:
        cout << "Error\n";
        break;
      }

      buildIndex = (buildIndex + 1) % WARRIOR_NUM;
      return true;
    }
    buildIndex = (buildIndex + 1) % WARRIOR_NUM;
  }
  isEmpty = true;
  return false;
}
