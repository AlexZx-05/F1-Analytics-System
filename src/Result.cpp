#include "Result.h"

// Default constructor
Result::Result() {
    raceId = 0;
    driverId = 0;
    position = 0;
}

// Parameter constructor
Result::Result(int r, int d, int p) {
    raceId = r;
    driverId = d;
    position = p;
}

// Getters (MUST be const)
int Result::getRaceId() const {
    return raceId;
}

int Result::getDriverId() const {
    return driverId;
}

int Result::getPosition() const {
    return position;
}
