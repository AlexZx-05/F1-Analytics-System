#include "Driver.h"

// Default constructor
Driver::Driver() {
    id = 0;
    name = "Unknown";
    nationality = "Unknown";
}

// Parameter constructor
Driver::Driver(int id, const std::string& name, const std::string& nationality) {
    this->id = id;
    this->name = name;
    this->nationality = nationality;
}

// Getters
int Driver::getId() const {
    return id;
}

std::string Driver::getName() const {
    return name;
}

std::string Driver::getNationality() const {
    return nationality;
}
