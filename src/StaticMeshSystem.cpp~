#include "StaticMeshSystem.h"
#include <iostream>
#include <math.h>
StaticMeshSystem::StaticMeshSystem(Mesh &m):ParticleSystem(1){
	//
	minDistance = 0.5;
    impulseDistance = 0.2;
    m_mesh = m;
}

bool StaticMeshSystem::intersect(Vector3f& pos, Vector3f& normal){
    return m_mesh.intersect(pos, 0.05);
}

vector<Vector3f> StaticMeshSystem::evalF(vector<Vector3f> state)
{
	vector<Vector3f> f;
    vector<Vector3f> states = getState();
    for(int i=0; i< m_numParticles; i++){
        Vector3f newForce = Vector3f(0,0,0);
        f.push_back(newForce);
        f.push_back(newForce);
    }
}
void StaticMeshSystem::move(Vector3f displacement){
}

void StaticMeshSystem::moveOne(Vector3f displacement){
}

void StaticMeshSystem::draw(){
    m_mesh.draw();
}