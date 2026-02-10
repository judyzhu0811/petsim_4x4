#include "run.hh"
#include "G4AnalysisManager.hh"
#include "detector.hh"   // Include for _det
#include <sstream>

MyRunAction::MyRunAction() : G4UserRunAction() {}
MyRunAction::~MyRunAction() {}

void MyRunAction::BeginOfRunAction(const G4Run*)
{
    G4AnalysisManager* man = G4AnalysisManager::Instance();

    // Reset all counters to zero at the start of a run
    for (G4int i = 0; i < GRID_SIZE; i++) {
        for (G4int j = 0; j < GRID_SIZE; j++) {
            _det[i][j] = 0;
        }
    }

    // Open output file
    man->OpenFile("output14mm.csv");

    // Create ntuple (ID = 0)
    man->CreateNtuple("Hits", "Hits");
    for (G4int i = 0; i < GRID_SIZE; i++) {
        for (G4int j = 0; j < GRID_SIZE; j++) {
            std::stringstream name;
            name << "det" << i << j;   // Columns: det00, det01, ...
            man->CreateNtupleIColumn(name.str());
        }
    }
    man->FinishNtuple(0);
}

void MyRunAction::EndOfRunAction(const G4Run*)
{
    G4AnalysisManager* man = G4AnalysisManager::Instance();

    // Fill ntuple with counts
    for (G4int i = 0; i < GRID_SIZE; i++) {
        for (G4int j = 0; j < GRID_SIZE; j++) {
            man->FillNtupleIColumn(i * GRID_SIZE + j, _det[i][j]);
        }
    }

    man->AddNtupleRow(0);  
    man->Write();
    man->CloseFile();       
    G4cout << "Detector hits written to CSV" << G4endl;
}
