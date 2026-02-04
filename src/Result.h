#ifndef RESULT_H
#define RESULT_H

class Result {

private:
    int raceId;
    int driverId;
    int position;

public:
    Result();
    Result(int raceId, int driverId, int position);

    int getRaceId() const;
    int getDriverId() const;
    int getPosition() const;
};

#endif // RESULT_H
