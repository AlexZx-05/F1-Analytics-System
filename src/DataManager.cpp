#include "DataManager.h"
#include "CSVReader.h"
#include "Race.h"
#include "Result.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cctype>

// ===== Helper Functions =====
// Get team name from driverId
std::string getTeamByDriver(int driverId) {

    if (driverId == 1 || driverId == 4)
        return "Mercedes";

    if (driverId == 2 || driverId == 3)
        return "RedBull";

    return "Unknown";
}


// Remove spaces from start & end
static std::string trim(const std::string& str) {

    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last  = str.find_last_not_of(" \t\n\r");

    if (first == std::string::npos)
        return "";

    return str.substr(first, last - first + 1);
}

// Convert string to lowercase
static std::string toLower(std::string s) {

    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) {
                       return std::tolower(c);
                   });

    return s;
}

// ================= DRIVERS =================

void DataManager::loadDrivers(const std::string& file) {

    auto data = CSVReader::readCSV(file);

    for (size_t i = 1; i < data.size(); i++) { // Skip header

        if (data[i].size() < 3) continue;

        int id = std::stoi(data[i][0]);
        std::string name = data[i][1];
        std::string nat  = data[i][2];

        drivers[id] = Driver(id, name, nat);
    }
}

void DataManager::showDrivers() {

    for (const auto& pair : drivers) {

        std::cout << pair.second.getId() << " | "
                  << pair.second.getName() << " | "
                  << pair.second.getNationality()
                  << std::endl;
    }
}

// ================= RACES =================

void DataManager::loadRaces(const std::string& file) {

    auto data = CSVReader::readCSV(file);

    for (size_t i = 1; i < data.size(); i++) {

        if (data[i].size() < 4) continue;

        int id   = std::stoi(data[i][0]);
        int year = std::stoi(data[i][1]);

        std::string name    = data[i][2];
        std::string circuit = data[i][3];

        races[id] = Race(id, year, name, circuit);
    }
}

void DataManager::showRaces() {

    for (const auto& pair : races) {

        std::cout << pair.second.getId() << " | "
                  << pair.second.getYear() << " | "
                  << pair.second.getName() << " | "
                  << pair.second.getCircuit()
                  << std::endl;
    }
}

// ================= RESULTS =================

void DataManager::loadResults(const std::string& file) {

    auto data = CSVReader::readCSV(file);

    for (size_t i = 1; i < data.size(); i++) {

        if (data[i].size() < 3) continue;

        int raceId   = std::stoi(data[i][0]);
        int driverId = std::stoi(data[i][1]);
        int pos      = std::stoi(data[i][2]);

        results.emplace_back(raceId, driverId, pos);
    }
}

void DataManager::showResults() {

    for (const auto& r : results) {

        std::cout << "Race " << r.getRaceId()
                  << " | Driver " << r.getDriverId()
                  << " | Position " << r.getPosition()
                  << std::endl;
    }
}

// ================= ANALYTICS =================

void DataManager::showAverageFinish() {

    std::map<int, int> totalPos;
    std::map<int, int> raceCount;

    for (const auto& r : results) {

        int driverId = r.getDriverId();

        totalPos[driverId] += r.getPosition();
        raceCount[driverId]++;
    }

    std::cout << "\n=== AVERAGE FINISH POSITION ===\n";

    for (const auto& pair : totalPos) {

        int driverId = pair.first;

        double avg =
            static_cast<double>(pair.second) / raceCount[driverId];

        if (drivers.count(driverId)) {

            std::cout << drivers[driverId].getName()
                      << " -> " << avg
                      << std::endl;
        }
    }
}

void DataManager::showTotalWins() {

    std::map<int, int> wins;

    for (const auto& r : results) {

        if (r.getPosition() == 1) {

            wins[r.getDriverId()]++;
        }
    }

    std::cout << "\n=== TOTAL WINS ===\n";

    for (const auto& pair : wins) {

        int driverId = pair.first;

        if (drivers.count(driverId)) {

            std::cout << drivers[driverId].getName()
                      << " -> " << pair.second << " wins"
                      << std::endl;
        }
    }
}

void DataManager::showPodiums() {

    std::map<int, int> podiums;

    for (const auto& r : results) {

        if (r.getPosition() <= 3) {

            podiums[r.getDriverId()]++;
        }
    }

    std::cout << "\n=== PODIUM FINISHES ===\n";

    for (const auto& pair : podiums) {

        int driverId = pair.first;

        if (drivers.count(driverId)) {

            std::cout << drivers[driverId].getName()
                      << " -> " << pair.second << " podiums"
                      << std::endl;
        }
    }
}

// ================= DRIVER RANKING =================

void DataManager::showDriverRanking() {

    // F1 Points System
    std::map<int, int> pointsTable = {
        {1, 25}, {2, 18}, {3, 15}, {4, 12}, {5, 10},
        {6, 8}, {7, 6}, {8, 4}, {9, 2}, {10, 1}
    };

    std::map<int, int> driverPoints;

    // Calculate points
    for (const auto& r : results) {

        int pos = r.getPosition();
        int driverId = r.getDriverId();

        if (pointsTable.count(pos)) {

            driverPoints[driverId] += pointsTable[pos];
        }
    }

    // Convert to vector for sorting
    std::vector<std::pair<int, int>> ranking;

    for (const auto& p : driverPoints) {

        ranking.push_back(p); // (driverId, points)
    }

    // Sort by points (descending)
    std::sort(ranking.begin(), ranking.end(),

        [](const auto& a, const auto& b) {
            return a.second > b.second;
        }
    );

    // Print ranking
    std::cout << "\n=== DRIVER RANKINGS ===\n";

    int pos = 1;

    for (const auto& r : ranking) {

        int driverId = r.first;
        int pts = r.second;

        if (drivers.count(driverId)) {

            std::cout << pos << ". "
                      << drivers[driverId].getName()
                      << " - " << pts << " pts"
                      << std::endl;

            pos++;
        }
    }
}
// ================= RACE WINNER PREDICTOR =================

void DataManager::predictNextRaceWinner() {

    std::map<int, int> totalPos;
    std::map<int, int> raceCount;
    std::map<int, int> wins;

    // Calculate averages and wins
    for (const auto& r : results) {

        int driverId = r.getDriverId();

        totalPos[driverId] += r.getPosition();
        raceCount[driverId]++;

        if (r.getPosition() == 1) {
            wins[driverId]++;
        }
    }

    // F1 Points System
    std::map<int, int> pointsTable = {
        {1,25},{2,18},{3,15},{4,12},{5,10},
        {6,8},{7,6},{8,4},{9,2},{10,1}
    };

    std::map<int, int> points;

    for (const auto& r : results) {

        int pos = r.getPosition();
        int driverId = r.getDriverId();

        if (pointsTable.count(pos)) {
            points[driverId] += pointsTable[pos];
        }
    }

    std::cout << "\n=== NEXT RACE WINNER PREDICTION ===\n";

    double bestScore = -1;
    int bestDriver = -1;

    // Calculate AI score
    for (const auto& d : drivers) {

        int driverId = d.first;

        if (raceCount[driverId] == 0) continue;

        double avgFinish =
            (double) totalPos[driverId] / raceCount[driverId];

        int winCount = wins[driverId];
        int pts = points[driverId];

        double score =
            (1.0 / avgFinish) * 40 +
            winCount * 10 +
            pts * 0.5;

        std::cout << d.second.getName()
                  << " -> Score: " << score
                  << std::endl;

        if (score > bestScore) {

            bestScore = score;
            bestDriver = driverId;
        }
    }

    // Show winner
    if (bestDriver != -1) {

        std::cout << "\n>>> PREDICTED WINNER: "
                  << drivers[bestDriver].getName()
                  << std::endl;
    }
}
// ================= PER-CIRCUIT PREDICTOR =================

void DataManager::predictWinnerByCircuit(const std::string& circuitName) {

    std::map<int, int> totalPos;
    std::map<int, int> raceCount;
    std::map<int, int> wins;

    // Step 1: Find race IDs for this circuit
    std::vector<int> circuitRaces;

    for (const auto& r : races) {

        if (r.second.getCircuit() == circuitName) {

            circuitRaces.push_back(r.first);
        }
    }

    if (circuitRaces.empty()) {

        std::cout << "\nNo data for circuit: "
                  << circuitName << std::endl;
        return;
    }

    // Step 2: Process results of those races
    for (const auto& res : results) {

        int raceId = res.getRaceId();

        if (std::find(circuitRaces.begin(),
                      circuitRaces.end(),
                      raceId) != circuitRaces.end()) {

            int driverId = res.getDriverId();

            totalPos[driverId] += res.getPosition();
            raceCount[driverId]++;

            if (res.getPosition() == 1) {
                wins[driverId]++;
            }
        }
    }

    // Step 3: Points System
    std::map<int, int> pointsTable = {
        {1,25},{2,18},{3,15},{4,12},{5,10},
        {6,8},{7,6},{8,4},{9,2},{10,1}
    };

    std::map<int, int> points;

    for (const auto& res : results) {

        int raceId = res.getRaceId();

        if (std::find(circuitRaces.begin(),
                      circuitRaces.end(),
                      raceId) != circuitRaces.end()) {

            int pos = res.getPosition();

            if (pointsTable.count(pos)) {

                points[res.getDriverId()] +=
                    pointsTable[pos];
            }
        }
    }

    // Step 4: AI Scoring
    std::cout << "\n=== CIRCUIT PREDICTION: "
              << circuitName << " ===\n";

    double bestScore = -1;
    int bestDriver = -1;

    for (const auto& d : drivers) {

        int driverId = d.first;

        if (raceCount[driverId] == 0) continue;

        double avg =
            (double) totalPos[driverId] / raceCount[driverId];

        int w = wins[driverId];
        int pts = points[driverId];

        double score =
            (1.0 / avg) * 40 +
            w * 10 +
            pts * 0.5;

        std::cout << d.second.getName()
                  << " -> Score: " << score
                  << std::endl;

        if (score > bestScore) {

            bestScore = score;
            bestDriver = driverId;
        }
    }

    // Step 5: Winner
    if (bestDriver != -1) {

        std::cout << "\n>>> PREDICTED WINNER ON "
                  << circuitName << ": "
                  << drivers[bestDriver].getName()
                  << std::endl;
    }
}
// ================= WEATHER BASED PREDICTION =================

void DataManager::predictWinnerWithWeather(const std::string& circuit,
                                           const std::string& weather) {

    std::map<int, double> scores;

    std::string circ = toLower(trim(circuit));
    std::string weat = toLower(trim(weather));

    // Step 1: Base score from results
    for (const auto& r : results) {

        int driverId = r.getDriverId();

        double base = 0;

        // Better position = more points
        base += (11 - r.getPosition()) * 5;

        // Win bonus
        if (r.getPosition() == 1)
            base += 20;

        scores[driverId] += base;
    }

    // Step 2: Circuit bonus
    for (const auto& pair : races) {

        const Race& race = pair.second;

        if (toLower(race.getCircuit()) == circ) {

            int raceId = race.getId();

            for (const auto& r : results) {

                if (r.getRaceId() == raceId &&
                    r.getPosition() <= 3) {

                    scores[r.getDriverId()] += 15;
                }
            }
        }
    }

    // Step 3: Weather adjustment
    for (auto& pair : scores) {

        int driverId = pair.first;

        // Rain favors experienced drivers
        if (weat == "rainy") {

            if (drivers[driverId].getName() ==
                "Lewis Hamilton" ||
                drivers[driverId].getName() ==
                "Fernando Alonso") {

                pair.second += 20;
            }
            else {
                pair.second -= 5;
            }
        }

        // Sunny favors aggressive drivers
        else if (weat == "sunny") {

            if (drivers[driverId].getName() ==
                "Max Verstappen") {

                pair.second += 15;
            }
        }

        // Cloudy = balanced
        else if (weat == "cloudy") {

            pair.second += 5;
        }
    }

    // Step 4: Find best driver
    int bestDriver = -1;
    double bestScore = -1;

    for (const auto& pair : scores) {

        if (pair.second > bestScore) {

            bestScore = pair.second;
            bestDriver = pair.first;
        }
    }

    // Step 5: Print result
    std::cout << "\n=== WEATHER PREDICTION ===\n";
    std::cout << "Circuit : " << circuit << "\n";
    std::cout << "Weather : " << weather << "\n\n";

    for (const auto& pair : scores) {

        std::cout << drivers[pair.first].getName()
                  << " -> Score: "
                  << pair.second
                  << std::endl;
    }

    if (bestDriver != -1) {

        std::cout << "\n>>> PREDICTED WINNER: "
                  << drivers[bestDriver].getName()
                  << std::endl;
    }
}

// ================= CHAMPIONSHIP SIMULATOR =================

void DataManager::simulateSeason() {

    std::cout << "\n================ SEASON SIMULATION ================\n";

    std::map<int, int> championshipPoints;

    // Initialize all drivers with 0 points
    for (const auto& d : drivers) {
        championshipPoints[d.first] = 0;
    }

    // F1 Points System (Top 5)
    std::vector<int> points = {25, 18, 15, 12, 10};

    // Loop through races
    for (const auto& racePair : races) {

        int raceId = racePair.first;
        std::string circuit = racePair.second.getCircuit();

        std::cout << "\nRace " << raceId
                  << " (" << circuit << ")\n";

        // Store driver scores
        std::vector<std::pair<int, double>> scores;

        for (const auto& d : drivers) {

            int driverId = d.first;

            double score = 0;

            // Use previous analytics
            for (const auto& r : results) {

                if (r.getDriverId() == driverId) {

                    score += (20 - r.getPosition());
                }
            }

            scores.push_back({driverId, score});
        }

        // Sort by score (desc)
        std::sort(scores.begin(), scores.end(),
                  [](auto& a, auto& b) {
                      return a.second > b.second;
                  });

        // Award points
        for (size_t i = 0; i < scores.size() && i < points.size(); i++) {

            int driverId = scores[i].first;

            championshipPoints[driverId] += points[i];

            std::cout << i+1 << ". "
                      << drivers[driverId].getName()
                      << " +" << points[i] << " pts\n";
        }
    }

    // Final Standings
    std::cout << "\n================ FINAL STANDINGS ================\n";

    std::vector<std::pair<int, int>> finalTable;

    for (const auto& p : championshipPoints) {
        finalTable.push_back(p);
    }

    std::sort(finalTable.begin(), finalTable.end(),
              [](auto& a, auto& b) {
                  return a.second > b.second;
              });

    for (size_t i = 0; i < finalTable.size(); i++) {

        int driverId = finalTable[i].first;

        std::cout << i+1 << ". "
                  << drivers[driverId].getName()
                  << " - " << finalTable[i].second
                  << " pts\n";
    }

    // Champion
    if (!finalTable.empty()) {

        int champId = finalTable[0].first;

        std::cout << "\n*** WORLD CHAMPION: "
                  << drivers[champId].getName()
                  << " ***\n";

    }
}
void DataManager::showAvailableCircuits() {

    std::cout << "\n=== AVAILABLE CIRCUITS ===\n";

    int i = 1;

    for (const auto& pair : races) {

        std::cout << i << ". "
                  << pair.second.getCircuit()
                  << std::endl;

        i++;
    }
}
std::string DataManager::getCircuitByIndex(int index) {

    int i = 1;

    for (const auto& pair : races) {

        if (i == index) {

            return pair.second.getCircuit();
        }

        i++;
    }

    return ""; // invalid
}
// ================= TYRE STRATEGY ENGINE =================

void DataManager::showTyreStrategy(const std::string& circuit,
                                   const std::string& weather) {

    std::string tyre;
    int pitStops = 0;

    std::string c = toLower(circuit);
    std::string w = toLower(weather);

    // Rain logic
    if (w == "rainy" || w == "rain") {

        tyre = "Wet";
        pitStops = 1;
    }

    // Dry logic
    else {

        if (c.find("monza") != std::string::npos) {

            tyre = "Medium";
            pitStops = 2;
        }
        else if (c.find("silverstone") != std::string::npos) {

            tyre = "Soft";
            pitStops = 2;
        }
        else if (c.find("monaco") != std::string::npos) {

            tyre = "Soft";
            pitStops = 3;
        }
        else {

            tyre = "Hard";
            pitStops = 1;
        }
    }

    std::cout << "\n=== TYRE STRATEGY ENGINE ===\n";
    std::cout << "Circuit : " << circuit << std::endl;
    std::cout << "Weather : " << weather << std::endl;

    std::cout << "\nRecommended Tyre : " << tyre << std::endl;
    std::cout << "Pit Stops       : " << pitStops << std::endl;
}

// ================= FUEL STRATEGY ENGINE =================

void DataManager::showFuelStrategy(const std::string& circuit,
                                   const std::string& weather) {

    std::string raceLength;
    int fuelLoad = 0;
    std::string savingMode;

    std::string c = toLower(circuit);
    std::string w = toLower(weather);

    // Determine race length
    if (c.find("monaco") != std::string::npos) {
        raceLength = "Short";
        fuelLoad = 85;
    }
    else if (c.find("monza") != std::string::npos) {
        raceLength = "Medium";
        fuelLoad = 90;
    }
    else if (c.find("silverstone") != std::string::npos ||
             c.find("bahrain") != std::string::npos) {
        raceLength = "Long";
        fuelLoad = 95;
    }
    else {
        raceLength = "Medium";
        fuelLoad = 90;
    }

    // Weather impact
    if (w == "rainy" || w == "rain") {
        fuelLoad -= 5;      // Less aggressive driving
        savingMode = "OFF";
    }
    else if (w == "cloudy") {
        savingMode = "ON";
    }
    else {
        savingMode = "ON";
    }

    std::cout << "\n=== FUEL STRATEGY ENGINE ===\n";
    std::cout << "Circuit : " << circuit << std::endl;
    std::cout << "Weather : " << weather << std::endl;

    std::cout << "\nRace Length : " << raceLength << std::endl;
    std::cout << "Fuel Load   : " << fuelLoad << " kg" << std::endl;
    std::cout << "Saving Mode : " << savingMode << std::endl;
}
// ================= EXPORT REPORT =================

void DataManager::exportReport(const std::string& circuit,
                               const std::string& weather) {

    std::ofstream file("f1_report.txt");

    if (!file.is_open()) {

        std::cout << "Error: Could not create report file!\n";
        return;
    }

    file << "====== F1 ANALYTICS REPORT ======\n\n";

    // ================= RANKINGS =================

    file << "=== DRIVER RANKINGS ===\n";

    std::vector<std::pair<std::string, int>> ranking;

    for (auto& pair : drivers) {

        int id = pair.first;
        int pts = 0;

        for (auto& r : results) {

            if (r.getDriverId() == id) {

                if (r.getPosition() == 1) pts += 25;
                else if (r.getPosition() == 2) pts += 18;
                else if (r.getPosition() == 3) pts += 15;
                else if (r.getPosition() == 4) pts += 12;
            }
        }

        ranking.push_back({ pair.second.getName(), pts });
    }

    std::sort(ranking.begin(), ranking.end(),
              [](auto& a, auto& b) {
                  return a.second > b.second;
              });

    int pos = 1;

    for (auto& r : ranking) {

        file << pos++ << ". "
             << r.first << " - "
             << r.second << " pts\n";
    }

    // ================= PREDICTION =================

    file << "\n=== NEXT RACE PREDICTION ===\n";

    std::string winner = ranking[0].first;

    file << "Predicted Winner: " << winner << "\n";


    // ================= SELECTED RACE =================

    file << "\n=== SELECTED RACE ===\n";
    file << "Circuit : " << circuit << "\n";
    file << "Weather : " << weather << "\n";


    // ================= TYRE =================

    file << "\n=== TYRE STRATEGY ===\n";

    std::string tyre = "Hard";
    int pit = 1;

    std::string c = toLower(circuit);
    std::string w = toLower(weather);

    if (w == "rainy" || w == "rain") {

        tyre = "Wet";
        pit = 1;
    }
    else if (c.find("monaco") != std::string::npos) {

        tyre = "Soft";
        pit = 3;
    }
    else if (c.find("monza") != std::string::npos) {

        tyre = "Medium";
        pit = 2;
    }

    file << "Tyre     : " << tyre << "\n";
    file << "PitStops : " << pit << "\n";


    // ================= FUEL =================

    file << "\n=== FUEL STRATEGY ===\n";

    int fuel = 90;
    std::string save = "ON";

    if (c.find("monaco") != std::string::npos) fuel = 85;
    if (c.find("silverstone") != std::string::npos) fuel = 95;
    if (w == "rainy") save = "OFF";

    file << "Fuel Load   : " << fuel << " kg\n";
    file << "Saving Mode : " << save << "\n";


    // ================= TEAM CHAMPIONSHIP =================

    file << "\n=== TEAM CHAMPIONSHIP ===\n";

    std::map<std::string, int> teamPoints;

    // Static mapping (can be dynamic later)
    teamPoints["Mercedes"] += 93;
    teamPoints["RedBull"]  += 58;

    std::string bestTeam;
    int bestPts = 0;

    for (auto& t : teamPoints) {

        file << t.first << " -> "
             << t.second << " pts\n";

        if (t.second > bestPts) {

            bestPts = t.second;
            bestTeam = t.first;
        }
    }

    file << "Winner   : " << bestTeam << "\n";


    // ================= CHAMPION =================

    file << "\n=== WORLD CHAMPION ===\n";
    file << winner << "\n";


    file.close();

    std::cout << "\nReport saved as f1_report.txt\n";
}

// ================= TEAM CHAMPIONSHIP =================

void DataManager::showTeamChampionship() {

    std::map<std::string, int> teamPoints;

    // F1 Points System
    std::map<int, int> points = {
        {1,25},{2,18},{3,15},{4,12},
        {5,10},{6,8},{7,6},{8,4},{9,2},{10,1}
    };

    // Calculate team points
    for (const auto& r : results) {

        int pos = r.getPosition();
        int driverId = r.getDriverId();

        if (points.count(pos)) {

            std::string team = getTeamByDriver(driverId);

            teamPoints[team] += points[pos];
        }
    }

    std::cout << "\n=== TEAM CHAMPIONSHIP ===\n";

    std::string winner;
    int maxPts = 0;

    for (const auto& pair : teamPoints) {

        std::cout << pair.first
                  << " -> " << pair.second << " pts\n";

        if (pair.second > maxPts) {

            maxPts = pair.second;
            winner = pair.first;
        }
    }

    std::cout << "\n>>> WINNER: " << winner << std::endl;
}
// ================= GET DRIVER RANKING =================

std::vector<std::pair<std::string, int>> DataManager::getDriverRanking() {

    std::vector<std::pair<std::string, int>> ranking;

    for (auto& d : drivers) {

        int id = d.first;
        int pts = 0;

        for (auto& r : results) {

            if (r.getDriverId() == id) {

                if (r.getPosition() == 1) pts += 25;
                else if (r.getPosition() == 2) pts += 18;
                else if (r.getPosition() == 3) pts += 15;
                else if (r.getPosition() == 4) pts += 12;
            }
        }

        ranking.push_back({
            d.second.getName(),
            pts
        });
    }

    // Sort by points (descending)
    std::sort(ranking.begin(), ranking.end(),
              [](auto& a, auto& b) {
                  return a.second > b.second;
              });

    return ranking;
}
