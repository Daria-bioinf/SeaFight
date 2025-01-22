#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int GRID_SIZE = 10; // Size of the grid
const char WATER = '~';  // Water
const char SHIP = 'S';   // Ship
const char HIT = 'X';    // Hit
const char MISS = 'O';   // Miss
const int MAX_SHIP_SIZE = 3; // Maximum ship size
const int MAX_SHIPS = 3;     // Number of ships per player

// Structure for coordinates
struct Coordinate {
    int x, y;
};

// Structure for a ship
struct Ship {
    Coordinate positions[MAX_SHIP_SIZE]; // Coordinates occupied by the ship
    int size;                           // Size of the ship
    bool isSunk;                        // Flag to check if the ship is sunk

    Ship() : size(0), isSunk(false) {}
};

// Class for the battlefield
class BattleField {
public:
    char grid[GRID_SIZE][GRID_SIZE]; // Grid for the battlefield
    Ship ships[MAX_SHIPS];          // Array of ships
    int shipCount;                  // Current number of ships

    // Constructor
    BattleField() : shipCount(0) {
        for (int y = 0; y < GRID_SIZE; ++y) {
            for (int x = 0; x < GRID_SIZE; ++x) {
                grid[y][x] = WATER;
            }
        }
    }

    // Check if a ship can be placed at the specified positions
    bool canPlaceShip(const Ship& ship) {
        for (int i = 0; i < ship.size; ++i) {
            const auto& pos = ship.positions[i];
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
        for (int i = 0; i < ship.size; ++i) {
            const auto& pos = ship.positions[i];
            grid[pos.y][pos.x] = SHIP;
        }
        ships[shipCount++] = ship;
    }

    // Check if a target coordinate is a hit
    bool isHit(const Coordinate& target) {
        for (int i = 0; i < shipCount; ++i) {
            Ship& ship = ships[i];
            for (int j = 0; j < ship.size; ++j) {
                if (ship.positions[j].x == target.x && ship.positions[j].y == target.y) {
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
        for (int i = 0; i < ship.size; ++i) {
            const auto& pos = ship.positions[i];
            if (grid[pos.y][pos.x] != HIT) {
                return; // If any cell is not hit, the ship is not sunk
            }
        }
        ship.isSunk = true;
    }

    // Check if there are any unsunk ships left
    bool hasShipsLeft() const {
        for (int i = 0; i < shipCount; ++i) {
            if (!ships[i].isSunk) return true;
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
    ship.size = size;
    bool isVertical = rand() % 2;
    Coordinate start = getRandomCoordinate();

    for (int i = 0; i < size; ++i) {
        if (isVertical) {
            ship.positions[i] = { start.x, start.y + i };
        }
        else {
            ship.positions[i] = { start.x + i, start.y };
        }
    }
    return ship;
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    // Player's and computer's battlefields
    BattleField playerField, computerField;

    // Place ships for both player and computer
    for (int i = 0; i < MAX_SHIPS; ++i) {
        Ship playerShip, computerShip;

        do {
            playerShip = generateRandomShip(MAX_SHIP_SIZE);
        } while (!playerField.canPlaceShip(playerShip));
        playerField.placeShip(playerShip);

        do {
            computerShip = generateRandomShip(MAX_SHIP_SIZE);
        } while (!computerField.canPlaceShip(computerShip));
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
