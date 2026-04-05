#ifndef CLOCK
#define CLOCK

#include "Defs.h"

class Clock {
private:
  int minutes;

public:
  Clock() : minutes(0) {}
  Clock(int t) : minutes(t) {}
  bool operator<(const Clock &other) const { return minutes < other.minutes; }
  void printTime() {
    int hour = minutes / 60;
    int minute = minutes % 60;
    printf("%03d:%02d ", hour, minute);
  }
  void addTime(int t) { minutes += t; }
};

inline Clock clocktime;

#endif // !CLOCK
