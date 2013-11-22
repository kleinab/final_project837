#include "particleSystem.h"
ParticleSystem::ParticleSystem(int nParticles):m_numParticles(nParticles){
}

void ParticleSystem::toggleDisplayMode(){
    displayMesh = !displayMesh;
}

