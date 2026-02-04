\# 🏎️ Formula 1 Analytics \& Prediction System (C++)



A full-stack C++ project that analyzes Formula 1 data, predicts race winners,

simulates championships, and provides strategy recommendations.



\## 🚀 Features



\- Driver Rankings

\- Race Winner Prediction

\- Circuit-Based Prediction

\- Weather-Based Prediction

\- Tyre Strategy Engine

\- Fuel Strategy Engine

\- Season Simulator

\- Team Championship

\- Report Export (TXT)

\- Web Backend (HTTP Server)

\- Frontend Dashboard



\## 🛠️ Tech Stack



\- C++

\- STL

\- CSV Parsing

\- cpp-httplib (Backend)

\- HTML / CSS / JavaScript (Frontend)



\## ▶️ How to Run



\### Terminal App

```bash

g++ src/\*.cpp -o f1

./f1

Backend Server

g++ backend/server.cpp src/DataManager.cpp src/Driver.cpp src/Race.cpp src/Result.cpp src/CSVReader.cpp -lws2\_32 -o server

./server

Then open in browser:



http://localhost:8080

📂 Project Structure

src/        → Core analytics engine

backend/    → C++ HTTP server

frontend/   → Web interface

database/   → CSV datasets

👨‍💻 Author

Deepak (AlexZx-05)





Save it.



---



\## 🔹 STEP 2 — Push README to GitHub



Run:



```bash

git add README.md

git commit -m "Add project documentation"

git push

