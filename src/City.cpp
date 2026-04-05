#include "City.h"
#include "Clock.h"
#include "Defs.h"
#include "Warrior.h"
#include "Weapon.h"
#include <cstdio>
#include <utility>

void City::runBattle() {
  Warrior *attacker = (id % 2 == 1) ? redWarrior : blueWarrior;
  Warrior *target = (id % 2 == 1) ? blueWarrior : redWarrior;
  attacker->sortWeapons();
  target->sortWeapons();
  while (true) {
    attacker->attack(target);
    if (attacker->isDead() || target->isDead())
      break;
    if (!attacker->hasWeapons() && !target->hasWeapons())
      break;
    swap(attacker, target);
  }
  if (redWarrior->isDead() && blueWarrior->isDead()) {
    clocktime.printTime();
    printf("both red %s %d and blue %s %d died in city %d\n",
           WARRIOR_NAMES[redWarrior->getType()].c_str(), redWarrior->get_id(),
           WARRIOR_NAMES[blueWarrior->getType()].c_str(), blueWarrior->get_id(),
           id);
    delete redWarrior;
    delete blueWarrior;
    redWarrior = nullptr;
    blueWarrior = nullptr;
    return;
  }
  if (redWarrior->isDead()) {
    blueWarrior->seizeWeapon(redWarrior);
    clocktime.printTime();
    printf("blue %s %d killed red %s %d in city %d remaining %d elements\n",
           WARRIOR_NAMES[blueWarrior->getType()].c_str(), blueWarrior->get_id(),
           WARRIOR_NAMES[redWarrior->getType()].c_str(), redWarrior->get_id(),
           id, blueWarrior->get_hp());
    if (blueWarrior->getType() == DRAGON) {
      clocktime.printTime();
      printf("blue dragon %d yelled in city %d\n", blueWarrior->get_id(), id);
    }
    delete redWarrior;
    redWarrior = nullptr;
    return;
  }
  if (blueWarrior->isDead()) {
    redWarrior->seizeWeapon(blueWarrior);
    clocktime.printTime();
    printf("red %s %d killed blue %s %d in city %d remaining %d elements\n",
           WARRIOR_NAMES[redWarrior->getType()].c_str(), redWarrior->get_id(),
           WARRIOR_NAMES[blueWarrior->getType()].c_str(), blueWarrior->get_id(),
           id, redWarrior->get_hp());
    if (redWarrior->getType() == DRAGON) {
      clocktime.printTime();
      printf("red dragon %d yelled in city %d\n", redWarrior->get_id(), id);
    }
    delete blueWarrior;
    blueWarrior = nullptr;
    return;
  }
  clocktime.printTime();
  printf("both red %s %d and blue %s %d were alive in city %d\n",
         WARRIOR_NAMES[redWarrior->getType()].c_str(), redWarrior->get_id(),
         WARRIOR_NAMES[blueWarrior->getType()].c_str(), blueWarrior->get_id(),
         id);
  if (redWarrior->getType() == DRAGON) {
    clocktime.printTime();
    printf("red dragon %d yelled in city %d\n", redWarrior->get_id(), id);
  }
  if (blueWarrior->getType() == DRAGON) {
    clocktime.printTime();
    printf("blue dragon %d yelled in city %d\n", blueWarrior->get_id(), id);
  }
}

void City::checkLionEscape() {
  if (redWarrior != nullptr && redWarrior->getType() == LION) {
    if (static_cast<Lion *>(redWarrior)->getLoyalty() <= 0) {
      clocktime.printTime();
      printf("red lion %d ran away\n", redWarrior->get_id());
      delete redWarrior;
      redWarrior = nullptr;
    }
  }
  if (blueWarrior != nullptr && blueWarrior->getType() == LION) {
    if (static_cast<Lion *>(blueWarrior)->getLoyalty() <= 0) {
      clocktime.printTime();
      printf("blue lion %d ran away\n", blueWarrior->get_id());
      delete blueWarrior;
      blueWarrior = nullptr;
    }
  }
}

void City::checkWolf() {
  if (redWarrior->getType() == WOLF && blueWarrior->getType() == WOLF) {
    return;
  }
  if (redWarrior->getType() == WOLF) {
    static_cast<Wolf *>(redWarrior)->getWeapon(blueWarrior, id);
  }
  if (blueWarrior->getType() == WOLF) {
    static_cast<Wolf *>(blueWarrior)->getWeapon(redWarrior, id);
  }
}
void City::checkWarrior() {
  if (redWarrior)
    redWarrior->report();
  if (blueWarrior)
    blueWarrior->report();
}
