#include "generator.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4OpticalPhoton.hh"        
#include "Randomize.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
    fParticleGun = new G4ParticleGun(1);

    // optical photon
    G4ParticleDefinition* particle = G4OpticalPhoton::OpticalPhotonDefinition();
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(6.98*eV);  

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
    int nPhotons = 20;  // photons per event
    for (int i = 0; i < nPhotons; i++)
    {
        // Random direction
        G4double x = G4UniformRand() - 0.5;
        G4double y = G4UniformRand() - 0.5;
        G4double z = G4UniformRand() - 0.5;
        G4ThreeVector mom(x, y, z);
        fParticleGun->SetParticleMomentumDirection(mom.unit());

        //polarization
        G4ThreeVector normal = mom.orthogonal();
        G4ThreeVector polarization =
            normal*std::cos(2*CLHEP::pi*G4UniformRand()) +
            mom.cross(normal)*std::sin(2*CLHEP::pi*G4UniformRand());
        fParticleGun->SetParticlePolarization(polarization);

        fParticleGun->GeneratePrimaryVertex(anEvent);
    }
}
