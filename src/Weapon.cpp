#include "Weapon.h"
#include "Warrior.h"

void Sword::change_owner(Warrior *newOwner) {
  attackPower = newOwner->get_ap() / 5;
}

void Bomb::change_owner(Warrior *newOwner) {
  attackPower = newOwner->get_ap() * 2 / 5;
}

void Arrow::change_owner(Warrior *newOwner) {
  attackPower = newOwner->get_ap() * 3 / 10;
}
