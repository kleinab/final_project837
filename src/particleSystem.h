#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <vector>
#include <vecmath.h>
#include <utility> 
#include <algorithm>
#include <iostream>
#include <math.h>
#include "Mesh.h"
using namespace std;

class ParticleSystem
{
public:

	ParticleSystem(int numParticles=0);

	int m_numParticles;
	
	// for a given state, evaluate derivative f(X,t)
	virtual vector<Vector3f> evalF(vector<Vector3f> state) = 0;
	
	// getter method for the system's state
	vector<Vector3f> getState(){ return m_vVecState; };
	
	// setter method for the system's state
	void setState(const vector<Vector3f>  & newState) { m_vVecState = newState; };
	void addGroup(ParticleSystem * g){group.push_back(g);}
	
	virtual void draw() = 0;
    void toggleDisplayMode();
	virtual void move(Vector3f displacement)=0;
    virtual void moveOne(Vector3f displacement)=0;
	virtual pair<int, float> intersect(int index, Vector3f& pos, Vector3f& v, Vector3f& normal, bool selfIntersect)=0;
	virtual int distanceToMesh(int i, Vector3f &pos, Vector3f &v) =0;
	virtual int distanceToEdgeMesh(int i, int b, Vector3f &posa, Vector3f & posb, Vector3f &va, Vector3f &vb) =0;
	virtual vector<int> getConnections(int i) = 0;
	virtual string getClass() = 0;

	
protected:
	vector<Vector3f> m_vVecState;
    bool displayMesh;
    float minDistance;
    float impulseDistance;
    vector<ParticleSystem *> group;
	
};

#endif
