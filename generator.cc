#include "generator.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4Gamma.hh"        // gamma particles
#include "Randomize.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
    fParticleGun = new G4ParticleGun(1);

    // --- Use gamma ---
    G4ParticleDefinition* particle = G4Gamma::GammaDefinition();
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(511*keV);  // PET photon energy

    // --- Position at the center ---
    G4ThreeVector pos(0., 0., 0.);
    fParticleGun->SetParticlePosition(pos);
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
    delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    // Shoot in random direction
    G4double x = G4UniformRand() - 0.5;
    G4double y = G4UniformRand() - 0.5;
    G4double z = G4UniformRand() - 0.5;
    G4ThreeVector mom(x, y, z);
    fParticleGun->SetParticleMomentumDirection(mom.unit());

    fParticleGun->GeneratePrimaryVertex(anEvent);
}
