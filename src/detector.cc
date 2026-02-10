#include "detector.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include "G4OpticalPhoton.hh"
#include <algorithm> // for std::fill
#include <iostream>

constexpr G4double cellSize = 1.0*cm; // size of each detector cell

// Global detector array definition (for run-level accumulation)
G4int _det[GRID_SIZE][GRID_SIZE] = {{0}};

// Constructor
Detector::Detector(G4String name)
 : G4VSensitiveDetector(name)
{
    // Initialize per-event hit grid
    _eventHits.assign(GRID_SIZE, std::vector<int>(GRID_SIZE, 0));

    // Open CSV for writing per-event hits
    outFile.open("hits_per_event.csv");
    if (!outFile.is_open()) {
        G4cerr << "ERROR: Could not open hits_per_event.csv!" << G4endl;
    }
}

// Destructor
Detector::~Detector() {
    if (outFile.is_open()) outFile.close();
}

// ProcessHits: record each optical photon
G4bool Detector::ProcessHits(G4Step* aStep, G4TouchableHistory*) {
    G4Track* track = aStep->GetTrack();

    // Only count optical photons
    if (track->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition())
        return false;

    // Get hit position
    G4ThreeVector pos = aStep->GetPreStepPoint()->GetPosition();
    int i = static_cast<int>(pos.x()/cellSize + GRID_SIZE/2);
    int j = static_cast<int>(pos.y()/cellSize + GRID_SIZE/2);

    // Increment if inside grid
    if (i >= 0 && i < GRID_SIZE && j >= 0 && j < GRID_SIZE) {
        _eventHits[i][j]++;  // per-event hits
        _det[i][j]++;        // global hits

        // Optional debug for first 10 hits
        static int nHits = 0;
        if (nHits++ < 10)
            G4cout << "Photon hit at pos=" << pos
                   << " mapped to grid[" << i << "][" << j << "]" << G4endl;
    }

    return true;
}

// Write per-event hits to CSV at end of event
void Detector::EndOfEvent() {
    if (!outFile.is_open()) return;

    for (G4int i=0; i<GRID_SIZE; i++){
        for (G4int j=0; j<GRID_SIZE; j++){
            outFile << _eventHits[i][j];
            if (!(i==GRID_SIZE-1 && j==GRID_SIZE-1)) outFile << ",";
        }
    }
    outFile << "\n";
    outFile.flush();

    // Reset per-event hits
    for (auto &row : _eventHits) std::fill(row.begin(), row.end(), 0);
}
