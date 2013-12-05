#include "SphereSystem.h"
#include <iostream>
#include <math.h>
SphereSystem::SphereSystem(Vector3f &pos, float r):ParticleSystem(1){
	//
	minDistance = 0.5;
    impulseDistance = 0.2;
    center = pos;
    radius = r;
}

int SphereSystem::intersect(Vector3f& pos, Vector3f& v, Vector3f& normal){
	float thickness = 0.02;
	if ((pos-center).abs()<radius+thickness){
		return 2;
	}else if ((pos-center).abs()<radius+thickness+0.01){
        return 1;
    }
	return 0;
}

vector<Vector3f> SphereSystem::evalF(vector<Vector3f> state)
{
	vector<Vector3f> f;
    vector<Vector3f> states = getState();
    for(int i=0; i< m_numParticles; i++){
        Vector3f newForce = Vector3f(0,0,0);
        f.push_back(newForce);
        f.push_back(newForce);
    }
}
void SphereSystem::move(Vector3f displacement){
    //return;
}

void SphereSystem::moveOne(Vector3f displacement){
    //return;
}

void SphereSystem::draw(){
	//return;
}