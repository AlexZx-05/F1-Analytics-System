document.addEventListener("DOMContentLoaded", () => {

    // ================= ELEMENTS =================

    const startScreen = document.getElementById("start-screen");
    const splash = document.getElementById("splash");
    const main = document.getElementById("main");

    const startBtn = document.getElementById("start-btn");
    const engineSound = document.getElementById("engineSound");

    const BASE_URL = "http://localhost:8080";


    // ================= START BUTTON =================

    startBtn.addEventListener("click", () => {

        // Hide start screen
        startScreen.style.display = "none";

        // Show splash
        splash.style.display = "flex";

        // Play engine sound
        engineSound.volume = 0.4;

        engineSound.play().catch(err => {
            console.log("Sound blocked:", err);
        });


        // After splash animation
        setTimeout(() => {

            // Stop sound
            engineSound.pause();
            engineSound.currentTime = 0;

            // Hide splash
            splash.style.display = "none";

            // Show dashboard
            main.style.display = "block";

            // Load everything
            loadAllData();

        }, 2500); // match CSS animation time
    });


    // ================= LOAD ALL DATA =================

    function loadAllData() {

        loadSession();
        loadChampion();
        loadPrediction();
        loadRanking();
        loadTyre();
        loadFuel();
        loadTeam();
    }


    // ================= SESSION =================

    function loadSession() {

        fetch(BASE_URL + "/api/session")
            .then(r => r.json())
            .then(d => {

                document.getElementById("track").innerText =
                    d.track;

                document.getElementById("weather").innerText =
                    d.weather;

                document.getElementById("laps").innerText =
                    d.laps;

                document.getElementById("status").innerText =
                    d.status;
            })
            .catch(() => {
                console.log("Session Error");
            });
    }


    // ================= CHAMPION =================

    function loadChampion() {

        fetch(BASE_URL + "/api/champion")
            .then(r => r.json())
            .then(d => {

                document.getElementById("champion").innerText =
                    d.champion;
            })
            .catch(() => {

                document.getElementById("champion").innerText =
                    "Error";
            });
    }


    // ================= PREDICTION =================

    function loadPrediction() {

        fetch(BASE_URL + "/api/predict")
            .then(r => r.json())
            .then(d => {

                document.getElementById("winner").innerText =
                    "Winner: " + d.winner;

                document.getElementById("confidence").innerText =
                    "Confidence: " + d.confidence + "%";
            })
            .catch(() => {

                document.getElementById("winner").innerText =
                    "Error";
            });
    }


    // ================= RANKING + CHART =================

    function loadRanking() {

        fetch(BASE_URL + "/api/ranking")
            .then(r => r.json())
            .then(data => {

                const list =
                    document.getElementById("ranking");

                list.innerHTML = "";

                let names = [];
                let points = [];

                data.forEach(d => {

                    // List
                    const li =
                        document.createElement("li");

                    li.innerText =
                        d.name + " - " + d.points + " pts";

                    list.appendChild(li);

                    // Chart data
                    names.push(d.name);
                    points.push(d.points);
                });

                drawChart(names, points);
            })
            .catch(() => {

                document.getElementById("ranking").innerHTML =
                    "<li>Error</li>";
            });
    }


    // ================= TYRE =================

    function loadTyre() {

        fetch(BASE_URL + "/api/tyre")
            .then(r => r.json())
            .then(d => {

                document.getElementById("tyre").innerText =
                    "Tyre: " + d.tyre;

                document.getElementById("pit").innerText =
                    "Pit Stops: " + d.pit;
            })
            .catch(() => {

                document.getElementById("tyre").innerText =
                    "Error";
            });
    }


    // ================= FUEL =================

    function loadFuel() {

        fetch(BASE_URL + "/api/fuel")
            .then(r => r.json())
            .then(d => {

                document.getElementById("fuel").innerText =
                    "Fuel: " + d.fuel + " kg";

                document.getElementById("save").innerText =
                    "Saving: " + d.save;
            })
            .catch(() => {

                document.getElementById("fuel").innerText =
                    "Error";
            });
    }


    // ================= TEAM =================

    function loadTeam() {

        fetch(BASE_URL + "/api/team")
            .then(r => r.json())
            .then(d => {

                document.getElementById("team").innerText =
                    d.winner + " (" + d.points + " pts)";
            })
            .catch(() => {

                document.getElementById("team").innerText =
                    "Error";
            });
    }


    // ================= CHART =================

    function drawChart(names, points) {

        if (!document.getElementById("pointsChart")) return;

        const ctx =
            document.getElementById("pointsChart")
                .getContext("2d");

        new Chart(ctx, {

            type: "bar",

            data: {

                labels: names,

                datasets: [{
                    label: "Points",
                    data: points,
                    backgroundColor: "#ff1e1e"
                }]
            },

            options: {

                responsive: true,

                plugins: {
                    legend: {
                        display: false
                    }
                },

                scales: {
                    y: {
                        beginAtZero: true
                    }
                }
            }
        });
    }

});
