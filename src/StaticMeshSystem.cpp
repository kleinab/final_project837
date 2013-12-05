#include "StaticMeshSystem.h"
#include <iostream>
#include <math.h>
StaticMeshSystem::StaticMeshSystem(Mesh &m):ParticleSystem(1){
	//
	minDistance = 0.5;
    impulseDistance = 0.2;
    m_clothMesh = m;
}

pair<int, float> StaticMeshSystem::intersect(int index, Vector3f& pos, Vector3f& v,  Vector3f& normal, bool selfIntersect){
    return m_clothMesh.intersect(index, pos, v, normal, 0.05, selfIntersect);
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
    m_clothMesh.draw();
}
