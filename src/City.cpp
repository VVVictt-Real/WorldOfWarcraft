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
    // if (id == 3 && redWarrior->get_id() == 10 && blueWarrior->get_id() == 5)
    // {
    //   printf("[DEBUG] attack:%s, weapon:%s, damage:%d, self_damage:%d",
    //          WARRIOR_NAMES[attacker->getType()].c_str(),
    //          WEAPON_NAMES[attacker->weapons[attacker->currentWeapon]->getType()]
    //              .c_str(),
    //          attacker->getAttackOther(), attacker->getAttackSelf());
    // }
    attacker->attack(target);
    // if (id == 3 && redWarrior->get_id() == 10 && blueWarrior->get_id() == 5)
    //   printf(", self_remaining:%d, target_remaining:%d\n",
    //   attacker->get_hp(),
    //          target->get_hp());
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

  // Warrior *first = (id % 2 == 1) ? redWarrior : blueWarrior;
  // Warrior *last = (id % 2 == 1) ? blueWarrior : redWarrior;
  // first->sortWeapons();
  // last->sortWeapons();
  // while (true) {
  //   first->attack(last);
  //   if (first->isDead() && last->isDead()) {
  //     clocktime.printTime();
  //     printf("both red %s %d and blue %s %d died in city %d\n",
  //            WARRIOR_NAMES[redWarrior->getType()].c_str(),
  //            redWarrior->get_id(),
  //            WARRIOR_NAMES[blueWarrior->getType()].c_str(),
  //            blueWarrior->get_id(), id);
  //     delete redWarrior;
  //     delete blueWarrior;
  //     break;
  //   }
  //   if (last->isDead()) {
  //     first->seizeWeapon(last);
  //     clocktime.printTime();
  //     printf("%s %s %d killed %s %s %d in city %d remaining %d elements\n",
  //            first->getColor().c_str(),
  //            WARRIOR_NAMES[first->getType()].c_str(), first->get_id(),
  //            last->getColor().c_str(),
  //            WARRIOR_NAMES[last->getType()].c_str(), last->get_id(), id,
  //            first->get_hp());
  //     if (first->getType() == DRAGON) {
  //       clocktime.printTime();
  //       printf("%s %s %d yelled in city %d\n", first->getColor().c_str(),
  //              WARRIOR_NAMES[first->getType()].c_str(), first->get_id(), id);
  //     }
  //     delete last;
  //     break;
  //   }
  //   if (first->isDead()) {
  //     last->seizeWeapon(first);
  //     clocktime.printTime();
  //     printf("%s %s %d killed %s %s %d in city %d remaining %d elements\n",
  //            last->getColor().c_str(),
  //            WARRIOR_NAMES[last->getType()].c_str(), last->get_id(),
  //            first->getColor().c_str(),
  //            WARRIOR_NAMES[first->getType()].c_str(), first->get_id(), id,
  //            last->get_hp());
  //     if (last->getType() == DRAGON) {
  //       clocktime.printTime();
  //       printf("%s %s %d yelled in city %d\n", last->getColor().c_str(),
  //              WARRIOR_NAMES[last->getType()].c_str(), last->get_id(), id);
  //     }
  //     delete first;
  //     break;
  //   }
  //   if (!first->hasWeapons() && !last->hasWeapons()) {
  //     clocktime.printTime();
  //     printf("both red %s %d and blue %s %d were alive in city %d\n",
  //            WARRIOR_NAMES[redWarrior->getType()].c_str(),
  //            redWarrior->get_id(),
  //            WARRIOR_NAMES[blueWarrior->getType()].c_str(),
  //            blueWarrior->get_id(), id);
  //     if (first->getType() == DRAGON)
  //     if (first->getType() == DRAGON) {
  //       clocktime.printTime();
  //       printf("%s %s %d yelled in city %d\n", first->getColor().c_str(),
  //              WARRIOR_NAMES[first->getType()].c_str(), first->get_id(), id);
  //     }
  //     if (last->getType() == DRAGON) {
  //       clocktime.printTime();
  //       printf("%s %s %d yelled in city %d\n", last->getColor().c_str(),
  //              WARRIOR_NAMES[last->getType()].c_str(), last->get_id(), id);
  //     }
  //     break;
  //   }
  //   swap(first, last);
  // }
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
