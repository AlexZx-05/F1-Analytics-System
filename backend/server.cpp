#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <winsock2.h>

#include "../src/DataManager.h"

#pragma comment(lib, "ws2_32.lib")

// ================= SEND RESPONSE =================

void sendResponse(SOCKET client,
                  const std::string& content,
                  const std::string& type = "text/html") {

    std::ostringstream response;

    response << "HTTP/1.1 200 OK\r\n";
    response << "Access-Control-Allow-Origin: *\r\n";
    response << "Content-Type: " << type << "\r\n";
    response << "Content-Length: " << content.size() << "\r\n";
    response << "Connection: close\r\n\r\n";
    response << content;

    std::string res = response.str();
    send(client, res.c_str(), res.size(), 0);
}


// ================= MAIN =================

int main() {

    // WINSOCK INIT
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    bind(server_fd, (sockaddr*)&address, sizeof(address));
    listen(server_fd, 5);

    std::cout << "Server running at http://localhost:8080\n";


    // LOAD DATA
    DataManager dm;

    dm.loadDrivers("database/drivers.csv");
    dm.loadRaces("database/races.csv");
    dm.loadResults("database/results.csv");


    // SERVER LOOP
    while (true) {

        SOCKET client;
        sockaddr_in clientAddr;
        int addrlen = sizeof(clientAddr);

        client = accept(server_fd,
                        (sockaddr*)&clientAddr,
                        &addrlen);

        if (client == INVALID_SOCKET) continue;

        char buffer[4096] = {0};
        recv(client, buffer, sizeof(buffer), 0);

        std::string request(buffer);


        // ================= ROUTES =================


        // ---------- RANKING ----------
        if (request.find("GET /api/ranking") != std::string::npos) {

            auto ranking = dm.getDriverRanking();

            std::ostringstream json;

            json << "[";

            for (size_t i = 0; i < ranking.size(); i++) {

                json << "{"
                     << "\"name\":\"" << ranking[i].first << "\","
                     << "\"points\":" << ranking[i].second
                     << "}";

                if (i != ranking.size() - 1)
                    json << ",";
            }

            json << "]";

            sendResponse(client, json.str(), "application/json");
        }


        // ---------- CHAMPION ----------
        else if (request.find("GET /api/champion") != std::string::npos) {

            auto ranking = dm.getDriverRanking();

            std::string json =
                "{ \"champion\": \"" + ranking[0].first + "\" }";

            sendResponse(client, json, "application/json");
        }


        // ---------- PREDICT ----------
        else if (request.find("GET /api/predict") != std::string::npos) {

            auto ranking = dm.getDriverRanking();

            std::string json =
                "{ \"winner\": \"" + ranking[0].first +
                "\", \"confidence\": 85 }";

            sendResponse(client, json, "application/json");
        }


        // ---------- TYRE ----------
        else if (request.find("GET /api/tyre") != std::string::npos) {

            // Simple logic (later dynamic)
            std::string tyre = "Medium";
            int pit = 2;

            std::string json =
                "{ \"tyre\": \"" + tyre +
                "\", \"pit\": " + std::to_string(pit) + " }";

            sendResponse(client, json, "application/json");
        }


        // ---------- FUEL ----------
        else if (request.find("GET /api/fuel") != std::string::npos) {

            int fuel = 90;
            std::string save = "ON";

            std::string json =
                "{ \"fuel\": " + std::to_string(fuel) +
                ", \"save\": \"" + save + "\" }";

            sendResponse(client, json, "application/json");
        }


        // ---------- TEAM ----------
        else if (request.find("GET /api/team") != std::string::npos) {

            std::string team = "Mercedes";
            int pts = 93;

            std::string json =
                "{ \"winner\": \"" + team +
                "\", \"points\": " + std::to_string(pts) + " }";

            sendResponse(client, json, "application/json");
        }
        // ---------- TRACK ----------
else if (request.find("GET /api/track") != std::string::npos) {

    std::string track = "Monza";

    std::string json =
        "{ \"track\": \"" + track + "\" }";

    sendResponse(client, json, "application/json");
}


// ---------- WEATHER ----------
else if (request.find("GET /api/weather") != std::string::npos) {

    std::string weather = "Sunny";

    std::string json =
        "{ \"weather\": \"" + weather + "\" }";

    sendResponse(client, json, "application/json");
}


// ---------- SESSION (ALL INFO) ----------
else if (request.find("GET /api/session") != std::string::npos) {

    std::string json =
        "{"
        "\"track\":\"Monza\","
        "\"weather\":\"Sunny\","
        "\"laps\":58,"
        "\"status\":\"Running\""
        "}";

    sendResponse(client, json, "application/json");
}



        // ---------- HTML FALLBACK ----------
        else {

            auto ranking = dm.getDriverRanking();

            std::ostringstream html;

            html << "<html><body>";
            html << "<h1>F1 Analytics Dashboard</h1>";

            html << "<ul>";

            for (auto& r : ranking) {

                html << "<li>"
                     << r.first
                     << " - "
                     << r.second
                     << " pts</li>";
            }

            html << "</ul>";

            html << "</body></html>";

            sendResponse(client, html.str(), "text/html");
        }


        closesocket(client);
    }


    WSACleanup();
    return 0;
}
