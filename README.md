# A* Maze Pathfinding Visualizer

A web-based interactive visualizer for the A* pathfinding algorithm. This project demonstrates how the A* algorithm efficiently finds the shortest path through a maze with multiple possible routes. The maze generation and pathfinding logic is implemented in C++ for optimal performance, while the visualization is handled through a modern web interface.

## 🎯 Features

- **Interactive Maze Generation**: Generate mazes of customizable sizes (5x5 to 50x50)
- **Real-time A* Visualization**: Watch the algorithm explore nodes and find the optimal path step-by-step
- **Educational Content**: Built-in information brochure explaining how A* works
- **WebSocket Communication**: Seamless communication between the web frontend and C++ backend
- **Activity Logging**: Comprehensive logging of user interactions and algorithm performance
- **Responsive Design**: Clean, modern UI that works across different screen sizes

## 🏗️ Architecture

The project consists of three main components:

1. **Frontend (Web Interface)**:
   - `index.html`: Main HTML structure with interactive controls
   - `style.css`: Modern CSS styling with gradients and animations
   - `script.js`: JavaScript for UI interactions and WebSocket communication

2. **Backend (Node.js Server)**:
   - `server.js`: WebSocket server that handles client connections and executes the C++ program

3. **Core Logic (C++ Program)**:
   - `maze_logic.cpp`: Implements maze generation using randomized DFS and A* pathfinding algorithm
   - Outputs JSON data for the frontend to visualize

## 🛠️ Technologies Used

- **Frontend**: HTML5, CSS3, JavaScript (ES6+)
- **Backend**: Node.js with WebSocket (ws library)
- **Core Algorithm**: C++11 with STL containers
- **Communication**: WebSocket protocol for real-time data exchange

## 📋 Prerequisites

- Node.js (v14 or higher)
- C++ compiler (GCC/MinGW on Windows, GCC on Linux/Mac)
- Modern web browser with WebSocket support

## 🚀 Installation

1. **Clone or download the project**:
   ```bash
   cd MazeProject
   ```

2. **Install Node.js dependencies**:
   ```bash
   npm install
   ```

3. **Compile the C++ program**:
   ```bash
   # On Windows with MinGW:
   g++ maze_logic.cpp -o maze_logic.exe -std=c++11

   # On Linux/Mac:
   g++ maze_logic.cpp -o maze_logic -std=c++11
   ```

## 🎮 Usage

1. **Start the server**:
   ```bash
   npm start
   # or
   node server.js
   ```

2. **Open the application**:
   - Open `index.html` in your web browser
   - Or serve it through a local web server

3. **Generate and visualize**:
   - Adjust the maze size using the input field (5-50)
   - Click "Generate Maze" to create a new random maze
   - Click "Visualize A* Algorithm" to watch the pathfinding in action
   - Use the info button (ℹ️) to learn how A* works

## 📊 How It Works

### Maze Generation
The maze is generated using a randomized Depth-First Search algorithm that creates a perfect maze (one solution path between any two points).

### A* Pathfinding
The A* algorithm uses the formula `f(n) = g(n) + h(n)` where:
- `g(n)`: Cost from start to current node
- `h(n)`: Heuristic estimate to goal (Manhattan distance)
- `f(n)`: Total estimated cost

The algorithm explores nodes with the lowest f-score first, guaranteeing the shortest path in grid-based mazes.

### Visualization
- **Walls**: Black cells
- **Open paths**: White cells
- **Start**: Green cell
- **Goal**: Red cell
- **Explored nodes**: Blue/purple cells (during visualization)
- **Final path**: Pink cells

## 📁 Project Structure

```
MazeProject/
├── index.html          # Main web interface
├── style.css           # CSS styling
├── script.js           # Frontend JavaScript
├── server.js           # Node.js WebSocket server
├── maze_logic.cpp      # C++ maze generation and A* algorithm
├── maze_logic.exe      # Compiled C++ executable (Windows)
├── package.json        # Node.js dependencies
├── user_activity_log.txt # Activity logs
└── runcmd.txt          # Quick start instructions
```

## 🔧 Development

### Adding New Features
- Frontend changes: Modify `script.js` and `index.html`
- Algorithm changes: Update `maze_logic.cpp` and recompile
- Styling: Edit `style.css`

### Logging
All user activities are logged to `user_activity_log.txt` including:
- Server start/stop events
- Maze generation requests and performance metrics
- Pathfinding visualization events

## 🤝 Contributing

This project was developed as part of an Object-Oriented Programming course project. For educational purposes, contributions are welcome to improve the algorithm visualization or add new features.

## 📄 License

ISC License - See package.json for details.

## 👥 Authors

Group 5 - OOP Course Project
- Developed using C++ for performance-critical algorithms
- Web technologies for interactive visualization
- WebSocket for real-time communication

---

**Note**: Make sure the C++ executable has proper permissions and is in the same directory as the server files. The application runs on `localhost:8081` for WebSocket communication.</content>
<parameter name="filePath">d:\VIT\3-SEM\OOP\OOP_CP_SEDA_B_GROUP_5\OOP_CP_CODE\MazeProject\README.md