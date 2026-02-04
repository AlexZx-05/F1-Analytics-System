#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <map>
#include <vector>
#include <string>
#include <utility>
#include "Driver.h"
#include "Race.h"
#include "Result.h"

class DataManager {
private:
    std::map<int, Driver> drivers;
    std::map<int, Race> races;
    std::vector<Result> results;

public:
    // Loaders
    void loadDrivers(const std::string& file);
    void loadRaces(const std::string& file);
    void loadResults(const std::string& file);

    // Show data
    void showDrivers();
    void showRaces();
    void showResults();

    // Analytics
    void showAverageFinish();
    void showTotalWins();
    void showPodiums();
    void showDriverRanking();

    // Predictions
    void predictNextRaceWinner();
    void predictWinnerByCircuit(const std::string& circuitName);
    void predictWinnerWithWeather(const std::string& circuit,
                                  const std::string& weather);

    // Menu helpers
    void showAvailableCircuits();
    std::string getCircuitByIndex(int index);
    void showTyreStrategy(const std::string& circuit,
                      const std::string& weather);


    void showFuelStrategy(const std::string& circuit,
                      const std::string& weather);
     
    void exportReport(const std::string& circuit,
                  const std::string& weather);
    void showTeamChampionship();
     // Return driver ranking (name, points)
std::vector<std::pair<std::string, int>> getDriverRanking();
       
    // Simulator
    void simulateSeason();
};

#endif
