#include "detector.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"

// Define the global detector grid (only once)
G4int _det[GRID_SIZE][GRID_SIZE] = {{0}};

// Optional: size of each detector cell (adjust to your geometry)
constexpr G4double cellSize = 1.0 * cm;

// Constructor
Detector::Detector(G4String name)
 : G4VSensitiveDetector(name)
{
    // Initialize grid to zero
    for (int i = 0; i < GRID_SIZE; ++i)
        for (int j = 0; j < GRID_SIZE; ++j)
            _det[i][j] = 0;
}

// Destructor
Detector::~Detector() {}

// ProcessHits implementation: record hits in the grid
G4bool Detector::ProcessHits(G4Step* aStep, G4TouchableHistory* history)
{
    // Get the position of the particle hit
    G4ThreeVector pos = aStep->GetPreStepPoint()->GetPosition();

    // Map position to grid indices
    int i = static_cast<int>(pos.x() / cellSize + GRID_SIZE / 2);
    int j = static_cast<int>(pos.y() / cellSize + GRID_SIZE / 2);

    // Increment grid cell if within bounds
    if (i >= 0 && i < GRID_SIZE && j >= 0 && j < GRID_SIZE)
        _det[i][j]++;

    return true;
}
