#include <iostream>
#include <string>
#include <sstream>
#include <winsock2.h>

#include "../src/DataManager.h"

#pragma comment(lib, "ws2_32.lib")

int main() {

    // Start Winsock
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    bind(server_fd, (sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);

    std::cout << "Server running at http://localhost:8080\n";

    // Load F1 Data
    DataManager dm;

    dm.loadDrivers("database/drivers.csv");
    dm.loadRaces("database/races.csv");
    dm.loadResults("database/results.csv");

    while (true) {

        SOCKET client;
        sockaddr_in clientAddr;
        int addrlen = sizeof(clientAddr);

        client = accept(server_fd,
                        (sockaddr*)&clientAddr,
                        &addrlen);

        if (client == INVALID_SOCKET) continue;

        char buffer[2048] = {0};
        recv(client, buffer, sizeof(buffer), 0);

        // Build HTML
        std::stringstream html;

        html << "<html><body>";
        html << "<h1>F1 Analytics Dashboard</h1>";

        auto ranking = dm.getDriverRanking();

        html << "<h2>Driver Rankings</h2><ul>";

        for (auto& r : ranking) {

            html << "<li>"
                 << r.first
                 << " - "
                 << r.second
                 << " pts</li>";
        }

        html << "</ul>";

        html << "<h2>World Champion</h2>";
        html << "<p>" << ranking[0].first << "</p>";

        html << "</body></html>";

        std::string body = html.str();

        // HTTP response
        std::stringstream response;

        response << "HTTP/1.1 200 OK\r\n";
        response << "Content-Type: text/html\r\n";
        response << "Content-Length: " << body.size() << "\r\n";
        response << "Connection: close\r\n\r\n";
        response << body;

        std::string res = response.str();

        send(client, res.c_str(), res.size(), 0);

        closesocket(client);
    }

    WSACleanup();
    return 0;
}
