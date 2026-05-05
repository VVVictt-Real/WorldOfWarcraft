#include "City.h"
#include "Clock.h"
#include "Defs.h"
#include "Warrior.h"
#include "Weapon.h"
#include <cmath>
#include <cstdio>
#include <utility>

bool City::checkRedFirst() {
  if (flag == -1)
    return (id % 2 == 1) ? true : false;
  else
    return (flag == 1) ? true : false;
}

void City::runBattle() {
  if (redWarrior->isDead() && !blueWarrior->isDead()) {
    afterBattle(blueWarrior, redWarrior, !checkRedFirst());
    return;
  }
  if (blueWarrior->isDead() && !redWarrior->isDead()) {
    afterBattle(redWarrior, blueWarrior, checkRedFirst());
    return;
  }

  Warrior *attacker = (checkRedFirst()) ? redWarrior : blueWarrior;
  Warrior *target = (checkRedFirst()) ? blueWarrior : redWarrior;
  attacker->attack(target);
  clocktime.printTime();
  printf(
      "%s %s %d attacked %s %s %d in city %d with %d elements and force %d\n",
      attacker->getColor().c_str(), WARRIOR_NAMES[attacker->getType()].c_str(),
      attacker->get_id(), target->getColor().c_str(),
      WARRIOR_NAMES[target->getType()].c_str(), target->get_id(), id,
      attacker->get_hp(), attacker->get_ap());
  // attacker->sortWeapons();
  // target->sortWeapons();
  // while (true) {
  //   attacker->attack(target);
  //   if (attacker->isDead() || target->isDead())
  //     break;
  //   if (!attacker->hasWeapons() && !target->hasWeapons())
  //     break;
  //   swap(attacker, target);
  // }
  // if (redWarrior->isDead() && blueWarrior->isDead()) {
  //   clocktime.printTime();
  //   printf("both red %s %d and blue %s %d died in city %d\n",
  //          WARRIOR_NAMES[redWarrior->getType()].c_str(),
  //          redWarrior->get_id(),
  //          WARRIOR_NAMES[blueWarrior->getType()].c_str(),
  //          blueWarrior->get_id(), id);
  //   delete redWarrior;
  //   delete blueWarrior;
  //   redWarrior = nullptr;
  //   blueWarrior = nullptr;
  //   return;
  // }
  if (target->isDead()) {
    afterBattle(attacker, target, true);
    return;
    // attacker->seizeWeapon(target);
    // clocktime.printTime();
    // printf("%s %s %d was killed in city %d\n", target->getColor().c_str(),
    //        WARRIOR_NAMES[target->getType()].c_str(), target->get_id(), id);
    // if (attacker->getType() == DRAGON) {
    //   static_cast<Dragon *>(attacker)->changeMorale(0.2);
    //   if (static_cast<Dragon *>(attacker)->getMorale() > 0.8) {
    //     clocktime.printTime();
    //     printf("%s dragon %d yelled in city %d\n",
    //     attacker->getColor().c_str(),
    //            redWarrior->get_id(), id);
    //   }
    // }
    // if (target->getColor() == "red")
    //   redWarrior = nullptr;
    // else
    //   blueWarrior = nullptr;
    // delete target;
    // return;
  }
  target->counterAttack(attacker);
  clocktime.printTime();
  printf("%s %s %d fought back against %s %s %d in city %d\n",
         attacker->getColor().c_str(),
         WARRIOR_NAMES[attacker->getType()].c_str(), attacker->get_id(),
         target->getColor().c_str(), WARRIOR_NAMES[target->getType()].c_str(),
         target->get_id(), id);
  swap(target, attacker);
  if (target->isDead()) {
    afterBattle(attacker, target, false);
    return;
    // attacker->seizeWeapon(target);
    // clocktime.printTime();
    // printf("%s %s %d was killed in city %d\n", target->getColor().c_str(),
    //        WARRIOR_NAMES[target->getType()].c_str(), target->get_id(), id);
    // if (attacker->getType() == DRAGON) {
    //   static_cast<Dragon *>(attacker)->changeMorale(0.2);
    //   // if (static_cast<Dragon *>(attacker)->getMorale() > 0.8) {
    //   //   clocktime.printTime();
    //   //   printf("%s dragon %d yelled in city %d\n",
    //   //   attacker->getColor().c_str(),
    //   //          redWarrior->get_id(), id);
    //   // }
    // }
    // if (target->getColor() == "red")
    //   redWarrior = nullptr;
    // else
    //   blueWarrior = nullptr;
    // delete target;
    // return;
  }
  // clocktime.printTime();
  // printf("both red %s %d and blue %s %d were alive in city %d\n",
  //        WARRIOR_NAMES[redWarrior->getType()].c_str(),
  //        redWarrior->get_id(),
  //        WARRIOR_NAMES[blueWarrior->getType()].c_str(),
  //        blueWarrior->get_id(), id);
  // if (redWarrior->getType() == DRAGON) {
  //   clocktime.printTime();
  //   printf("red dragon %d yelled in city %d\n", redWarrior->get_id(),
  //   id);
  // }
  // if (blueWarrior->getType() == DRAGON) {
  //   clocktime.printTime();
  //   printf("blue dragon %d yelled in city %d\n", blueWarrior->get_id(),
  //   id);
  // }
}

void City::afterBattle(Warrior *attacker, Warrior *target, bool isFirst) {
  attacker->seizeWeapon(target);
  clocktime.printTime();
  printf("%s %s %d was killed in city %d\n", target->getColor().c_str(),
         WARRIOR_NAMES[target->getType()].c_str(), target->get_id(), id);
  if (attacker->getType() == DRAGON) {
    static_cast<Dragon *>(attacker)->changeMorale(0.2);
    if (isFirst && static_cast<Dragon *>(attacker)->getMorale() > 0.8) {
      clocktime.printTime();
      printf("%s dragon %d yelled in city %d\n", attacker->getColor().c_str(),
             redWarrior->get_id(), id);
    }
  }
  attacker->add_hp(8); // 问题：先处理近的。缺少获取生命元。缺少插旗逻辑。
  if (target->getColor() == "red")
    redWarrior = nullptr;
  else
    blueWarrior = nullptr;
  delete target;
  return;
}

pair<int, int> City::preRunBattle() {
  Warrior tempRed = *redWarrior;
  Warrior tempBlue = *blueWarrior;
  if (tempBlue.isDead() || tempRed.isDead()) {
    return make_pair(0, 0);
  }
  if (checkRedFirst()) {
    tempRed.attack(&tempBlue);
    if (tempBlue.isDead()) {
      return make_pair(0, 1);
    }
    tempBlue.counterAttack(&tempRed);
    if (tempRed.isDead()) {
      return make_pair(1, 0);
    }
  } else {
    tempBlue.attack(&tempRed);
    if (tempRed.isDead()) {
      return make_pair(1, 0);
    }
    tempRed.counterAttack(&tempBlue);
    if (tempBlue.isDead()) {
      return make_pair(0, 1);
    }
  }
  return make_pair(0, 0);
}

void City::redShootNext(City *next) {
  if (redWarrior && next->blueWarrior) {
    if (redWarrior->shoot(next->blueWarrior)) {
      // next->blueWarrior = nullptr;
    }
  }
}

void City::blueShootNext(City *next) {
  if (blueWarrior && next->redWarrior) {
    if (blueWarrior->shoot(next->redWarrior)) {
      // next->redWarrior = nullptr;
    }
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

// void City::checkWolf() {
//   if (redWarrior->getType() == WOLF && blueWarrior->getType() == WOLF) {
//     return;
//   }
//   if (redWarrior->getType() == WOLF) {
//     static_cast<Wolf *>(redWarrior)->getWeapon(blueWarrior, id);
//   }
//   if (blueWarrior->getType() == WOLF) {
//     static_cast<Wolf *>(blueWarrior)->getWeapon(redWarrior, id);
//   }
// }
void City::checkWarrior() {
  if (redWarrior)
    redWarrior->report();
  if (blueWarrior)
    blueWarrior->report();
}
