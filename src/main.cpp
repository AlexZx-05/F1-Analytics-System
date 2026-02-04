#include <iostream>
#include <string>
#include "DataManager.h"

int main() {

    DataManager dm;

    // ================= LOAD DATA =================

    dm.loadDrivers("database/drivers.csv");
    dm.loadRaces("database/races.csv");
    dm.loadResults("database/results.csv");

    // ================= SHOW DATA =================

    std::cout << "================ DRIVERS ================\n";
    dm.showDrivers();

    std::cout << "\n================ RACES ================\n";
    dm.showRaces();

    std::cout << "\n================ RESULTS ================\n";
    dm.showResults();

    // ================= ANALYTICS =================

    std::cout << "\n================ ANALYTICS ================\n";

    dm.showAverageFinish();
    dm.showTotalWins();
    dm.showPodiums();
    dm.showDriverRanking();
    dm.predictNextRaceWinner();

    // ================= CIRCUIT SELECTION =================

    dm.showAvailableCircuits();

    int choice;

    std::cout << "\nSelect circuit number: ";
    std::cin >> choice;

    std::cin.ignore(); // Clear input buffer

    std::string circuit = dm.getCircuitByIndex(choice);

    if (circuit.empty()) {

        std::cout << "Invalid circuit selection!\n";
        return 0;   // STOP program safely
    }

    // Circuit prediction
    dm.predictWinnerByCircuit(circuit);

    // ================= WEATHER =================

    std::string weather;

    std::cout << "\nEnter weather (sunny/rainy/cloudy): ";
    std::getline(std::cin, weather);

    dm.predictWinnerWithWeather(circuit, weather);

    // ================= STRATEGY =================

    // Tyre Strategy
    dm.showTyreStrategy(circuit, weather);

    // Fuel Strategy
    dm.showFuelStrategy(circuit, weather);
    // EXPORT REPORT
    dm.exportReport(circuit, weather);
    std::cout << "\n=========== PROGRAM FINISHED ===========\n";

    // ================= SEASON SIMULATION =================

    dm.simulateSeason();
    dm.showTeamChampionship();

    return 0;
}
