#ifndef RACE_H
#define RACE_H

#include <string>

class Race {

private:
    int id;
    int year;
    std::string name;
    std::string circuit;

public:
    Race();
    Race(int id, int year, const std::string& name, const std::string& circuit);

    int getId() const;
    int getYear() const;
    std::string getName() const;
    std::string getCircuit() const;
};

#endif // RACE_H
