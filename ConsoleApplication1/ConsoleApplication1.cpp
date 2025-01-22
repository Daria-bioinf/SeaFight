#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

const int GRID_SIZE = 10; // Size of the grid
const char WATER = '~';  // Water
const char SHIP = 'S';   // Ship
const char HIT = 'X';    // Hit
const char MISS = 'O';   // Miss

// Structure for coordinates
struct Coordinate {
    int x, y;
};

// Structure for a ship
struct Ship {
    vector<Coordinate> positions; // Coordinates occupied by the ship
    bool isSunk = false;          // Flag to check if the ship is sunk
};

// Class for the battlefield
class BattleField {
public:
    vector<vector<char>> grid;   // Grid for the battlefield
    vector<Ship> ships;          // List of ships on the battlefield

    // Constructor
    BattleField() {
        grid = vector<vector<char>>(GRID_SIZE, vector<char>(GRID_SIZE, WATER));
    }

    // Check if a ship can be placed at the specified positions
    bool canPlaceShip(const vector<Coordinate>& positions) {
        for (const auto& pos : positions) {
            if (pos.x < 0 || pos.x >= GRID_SIZE || pos.y < 0 || pos.y >= GRID_SIZE) {
                return false; // Outside the grid boundaries
            }
            if (grid[pos.y][pos.x] == SHIP) {
                return false; // Cell already occupied
            }
        }
        return true;
    }

    // Place a ship on the battlefield
    void placeShip(const Ship& ship) {
        for (const auto& pos : ship.positions) {
            grid[pos.y][pos.x] = SHIP;
        }
        ships.push_back(ship);
    }

    // Check if a target coordinate is a hit
    bool isHit(const Coordinate& target) {
        for (auto& ship : ships) {
            for (const auto& pos : ship.positions) {
                if (pos.x == target.x && pos.y == target.y) {
                    grid[target.y][target.x] = HIT; // Mark as hit
                    checkIfSunk(ship);             // Check if the ship is sunk
                    return true;
                }
            }
        }
        grid[target.y][target.x] = MISS; // Mark as miss
        return false;
    }

    // Check if a ship is sunk
    void checkIfSunk(Ship& ship) {
        for (const auto& pos : ship.positions) {
            if (grid[pos.y][pos.x] != HIT) {
                return; // If any cell is not hit, the ship is not sunk
            }
        }
        ship.isSunk = true;
    }

    // Check if there are any unsunk ships left
    bool hasShipsLeft() const {
        for (const auto& ship : ships) {
            if (!ship.isSunk) return true;
        }
        return false;
    }

    // Print the battlefield
    void print(bool showShips = false) const {
        cout << "  ";
        for (int i = 0; i < GRID_SIZE; ++i) cout << i << " ";
        cout << endl;

        for (int y = 0; y < GRID_SIZE; ++y) {
            cout << y << " ";
            for (int x = 0; x < GRID_SIZE; ++x) {
                if (grid[y][x] == SHIP && !showShips) {
                    cout << WATER << " "; // Hide ships
                }
                else {
                    cout << grid[y][x] << " ";
                }
            }
            cout << endl;
        }
    }
};

// Generate random coordinates
Coordinate getRandomCoordinate() {
    return { rand() % GRID_SIZE, rand() % GRID_SIZE };
}

// Generate a random ship of the given size
Ship generateRandomShip(int size) {
    Ship ship;
    bool isVertical = rand() % 2;
    Coordinate start = getRandomCoordinate();

    for (int i = 0; i < size; ++i) {
        if (isVertical) {
            ship.positions.push_back({ start.x, start.y + i });
        }
        else {
            ship.positions.push_back({ start.x + i, start.y });
        }
    }
    return ship;
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    // Player's and computer's battlefields
    BattleField playerField, computerField;

    // Place ships for both player and computer
    for (int i = 0; i < 3; ++i) {
        Ship playerShip, computerShip;

        do {
            playerShip = generateRandomShip(3); // Ships of size 3 cells
        } while (!playerField.canPlaceShip(playerShip.positions));
        playerField.placeShip(playerShip);

        do {
            computerShip = generateRandomShip(3);
        } while (!computerField.canPlaceShip(computerShip.positions));
        computerField.placeShip(computerShip);
    }

    cout << "Welcome to Battleship!" << endl;

    // Main game loop
    while (playerField.hasShipsLeft() && computerField.hasShipsLeft()) {
        // Print fields
        cout << "\nYour field:" << endl;
        playerField.print(true);

        cout << "\nComputer's field:" << endl;
        computerField.print(false);

        // Player's turn
        Coordinate playerTarget;
        cout << "\nYour turn! Enter target coordinates (x y): ";
        cin >> playerTarget.x >> playerTarget.y;

        if (computerField.isHit(playerTarget)) {
            cout << "Hit!" << endl;
        }
        else {
            cout << "Miss!" << endl;
        }

        // Computer's turn
        Coordinate computerTarget = getRandomCoordinate();
        cout << "\nComputer's turn: (" << computerTarget.x << ", " << computerTarget.y << ")" << endl;

        if (playerField.isHit(computerTarget)) {
            cout << "The computer hit your ship!" << endl;
        }
        else {
            cout << "The computer missed!" << endl;
        }
    }

    // Determine the winner
    if (computerField.hasShipsLeft()) {
        cout << "\nThe computer wins!" << endl;
    }
    else {
        cout << "\nYou win!" << endl;
    }

    return 0;
}
