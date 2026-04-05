#include "Game.h"
#include <iostream>

int K;

int main() {
  cin >> K;
  for (int i = 1; i <= K; i++) {
    printf("Case %d:\n", i);
    game();
  }
  return 0;
}
