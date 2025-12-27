const WebSocket = require('ws');
const { exec } = require('child_process');
const fs = require('fs');
const path = require('path');

// Create WebSocket server on port 8081
const wss = new WebSocket.Server({ port: 8081 });

const logFile = path.join(__dirname, 'user_activity_log.txt');

// Function to log user actions to file
function logAction(action, details = '') {
    const timestamp = new Date().toISOString();
    const logEntry = `[${timestamp}] ${action}${details ? ' - ' + details : ''}\n`;
    
    fs.appendFile(logFile, logEntry, (err) => {
        if (err) {
            console.error('Error writing to log file:', err);
        }
    });
    
    console.log(logEntry.trim());
}

// Initialize log file with session start
const sessionStart = `\n${'='.repeat(80)}\nNEW SESSION STARTED: ${new Date().toLocaleString()}\n${'='.repeat(80)}\n`;
fs.appendFileSync(logFile, sessionStart);

console.log('Server started on ws://localhost:8081');
logAction('SERVER_STARTED', 'WebSocket server initialized on port 8081');

wss.on('connection', (ws) => {
    const clientId = Math.random().toString(36).substring(7);
    console.log('Client connected');
    logAction('CLIENT_CONNECTED', `Client ID: ${clientId}`);
    
    ws.on('close', () => {
        logAction('CLIENT_DISCONNECTED', `Client ID: ${clientId}`);
    });

    ws.on('message', (message) => {
        const data = JSON.parse(message);
        
        if (data.type === 'generate') {
            const size = data.size || 15;
            console.log(`Generating maze of size ${size}...`);
            logAction('MAZE_GENERATION_REQUESTED', `Size: ${size}x${size}`);

            // EXECUTE THE C++ PROGRAM
            // Ensure 'maze_logic' (or 'maze_logic.exe' on Windows) is in the same folder
            const command = process.platform === 'win32' ? 'maze_logic.exe' : './maze_logic';
            
            const startTime = Date.now();
            exec(`${command} ${size}`, (error, stdout, stderr) => {
                if (error) {
                    console.error(`Error: ${error.message}`);
                    logAction('MAZE_GENERATION_FAILED', `Size: ${size}, Error: ${error.message}`);
                    return;
                }
                
                const executionTime = Date.now() - startTime;
                
                // Parse the maze data to log statistics
                try {
                    const mazeData = JSON.parse(stdout);
                    const pathLength = mazeData.solution ? mazeData.solution.length : 0;
                    const exploredNodes = mazeData.explored ? mazeData.explored.length : 0;
                    
                    logAction('MAZE_GENERATED_SUCCESSFULLY', 
                        `Size: ${size}x${size}, Execution Time: ${executionTime}ms, ` +
                        `Path Length: ${pathLength} cells, Explored Nodes: ${exploredNodes}`);
                } catch (e) {
                    logAction('MAZE_GENERATED_SUCCESSFULLY', `Size: ${size}x${size}, Execution Time: ${executionTime}ms`);
                }
                
                // Send the JSON output from C++ back to the frontend
                ws.send(stdout);
            });
        } else if (data.type === 'visualize') {
            logAction('PATHFINDING_VISUALIZATION_STARTED', 
                `Algorithm: A*, Maze Size: ${data.mazeSize || 'unknown'}`);
        } else if (data.type === 'visualization_complete') {
            logAction('PATHFINDING_VISUALIZATION_COMPLETED', 
                `Duration: ${data.duration || 'unknown'}ms`);
        }
    });
});