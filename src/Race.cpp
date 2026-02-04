#include "Race.h"

// Default constructor
Race::Race() {
    id = 0;
    year = 0;
    name = "Unknown";
    circuit = "Unknown";
}

// Parameter constructor
Race::Race(int id, int year, const std::string& name, const std::string& circuit) {

    this->id = id;
    this->year = year;
    this->name = name;
    this->circuit = circuit;
}

// Getters
int Race::getId() const {
    return id;
}

int Race::getYear() const {
    return year;
}

std::string Race::getName() const {
    return name;
}

std::string Race::getCircuit() const {
    return circuit;
}
