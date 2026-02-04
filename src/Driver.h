#ifndef DRIVER_H
#define DRIVER_H

#include <string>

class Driver {

private:
    int id;
    std::string name;
    std::string nationality;

public:
    Driver();
    Driver(int id, const std::string& name, const std::string& nationality);

    int getId() const;
    std::string getName() const;
    std::string getNationality() const;
};

#endif // DRIVER_H
