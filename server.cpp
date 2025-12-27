#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <thread>
#include <chrono>
#include <sstream>
#include <iomanip>

using namespace std;

class Logger {
private:
    string logFile;
    ofstream logStream;

    string getCurrentTimestamp() {
        auto now = chrono::system_clock::now();
        auto in_time_t = chrono::system_clock::to_time_t(now);
        auto ms = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()) % 1000;
        
        stringstream ss;
        ss << put_time(localtime(&in_time_t), "%Y-%m-%dT%H:%M:%S");
        ss << '.' << setfill('0') << setw(3) << ms.count() << 'Z';
        return ss.str();
    }

public:
    Logger(const string& filename) : logFile(filename) {
        // Initialize log file with session start
        ofstream outFile(logFile, ios::app);
        if (outFile.is_open()) {
            outFile << "\n" << string(80, '=') << "\n";
            
            auto now = time(nullptr);
            outFile << "NEW SESSION STARTED: " << ctime(&now);
            outFile << string(80, '=') << "\n";
            outFile.close();
        }
    }

    void log(const string& action, const string& details = "") {
        ofstream outFile(logFile, ios::app);
        if (outFile.is_open()) {
            outFile << "[" << getCurrentTimestamp() << "] " << action;
            if (!details.empty()) {
                outFile << " - " << details;
            }
            outFile << "\n";
            outFile.close();
        }
        
        // Also print to console
        cout << "[" << getCurrentTimestamp() << "] " << action;
        if (!details.empty()) {
            cout << " - " << details;
        }
        cout << endl;
    }
};

class MazeServer {
private:
    Logger logger;
    int port;
    bool isRunning;

public:
    MazeServer(int serverPort) : logger("user_activity_log.txt"), port(serverPort), isRunning(false) {}

    void start() {
        isRunning = true;
        logger.log("SERVER_STARTED", "C++ WebSocket server initialized on port " + to_string(port));
        
        cout << "\n=== Maze Server Backend (C++) ===" << endl;
        cout << "Server running on port " << port << endl;
        cout << "Waiting for client connections..." << endl;
        cout << "Press Ctrl+C to stop\n" << endl;
        
        // Simulate server running
        while (isRunning) {
            this_thread::sleep_for(chrono::seconds(1));
        }
    }

    void handleClientConnection(const string& clientId) {
        logger.log("CLIENT_CONNECTED", "Client ID: " + clientId);
    }

    void handleClientDisconnection(const string& clientId) {
        logger.log("CLIENT_DISCONNECTED", "Client ID: " + clientId);
    }

    void handleMazeGenerationRequest(int size) {
        logger.log("MAZE_GENERATION_REQUESTED", "Size: " + to_string(size) + "x" + to_string(size));
        
        auto startTime = chrono::high_resolution_clock::now();
        
        // Call maze_logic.exe to generate maze
        string command = "maze_logic.exe " + to_string(size);
        int result = system(command.c_str());
        
        auto endTime = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();
        
        if (result == 0) {
            logger.log("MAZE_GENERATED_SUCCESSFULLY", 
                "Size: " + to_string(size) + "x" + to_string(size) + 
                ", Execution Time: " + to_string(duration) + "ms");
        } else {
            logger.log("MAZE_GENERATION_FAILED", 
                "Size: " + to_string(size) + ", Error code: " + to_string(result));
        }
    }

    void handleVisualizationStart(int mazeSize) {
        logger.log("PATHFINDING_VISUALIZATION_STARTED", 
            "Algorithm: A*, Maze Size: " + to_string(mazeSize));
    }

    void handleVisualizationComplete(int duration) {
        logger.log("PATHFINDING_VISUALIZATION_COMPLETED", 
            "Duration: " + to_string(duration) + "ms");
    }

    void stop() {
        isRunning = false;
        logger.log("SERVER_STOPPED", "Server shutdown");
    }
};

int main() {
    MazeServer server(8081);
    
    // Initialize server
    server.start();
    
    return 0;
}
