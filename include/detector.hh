// detector.hh

#ifndef DETECTOR_HH
#define DETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "globals.hh"
#include <vector>
#include <fstream>

#define GRID_SIZE 4 

class Detector : public G4VSensitiveDetector
{
public:
    Detector(G4String name);           // Constructor
    virtual ~Detector();               // Destructor
    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* history) override;
    void EndOfEvent();  // Option 1: no arguments
    void StartOfEvent() {  // initialize hits at start of event
        _eventHits.assign(GRID_SIZE, std::vector<int>(GRID_SIZE, 0));
    }
private:
    std::vector<std::vector<int>> _eventHits; // store hits per event
    std::ofstream outFile;                     // CSV output
};

extern G4int _det[GRID_SIZE][GRID_SIZE];  

#endif
