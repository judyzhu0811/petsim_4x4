// detector.hh

#ifndef DETECTOR_HH
#define DETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "globals.hh"

#define GRID_SIZE 4 

class Detector : public G4VSensitiveDetector
{
public:
    Detector(G4String name);           // Constructor
    virtual ~Detector();               // Destructor
    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* history) override;
};

// <<< CHANGE THIS LINE <<<
// extern declares the array but does NOT define it
extern G4int _det[GRID_SIZE][GRID_SIZE];  

#endif
