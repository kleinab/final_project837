#ifndef CLOTHSYSTEM_H
#define CLOTHSYSTEM_H

#include "extra.h"
#include <vector>

#include "particleSystem.h"

class ClothSystem: public ParticleSystem
{
public:
    ClothSystem(int numParticles);
	//ClothSystem(int x, int y);
	vector<Vector3f> evalF(vector<Vector3f> state);
	int convert(int x, int y);
    bool isInGrid(int x, int y);
    Vector3f calcForce(int i, float g, float dragk, vector<Vector3f> state);
	void draw();
    void move(Vector3f displacement);
    void moveOne(Vector3f displacement);
    Vector3f calculateNormal(int i, int j, vector<Vector3f> states);
    bool intersect(Vector3f& pos, Vector3f& normal);
    bool selfIntersect(int index, Vector3f& normal, Vector3f& impulse);
private:
    int m_numX;
    int m_numY;
    vector<float> m_particleMasses;
    vector< vector < Vector3f > > m_vectorSprings;
    Vector3f m_moveClothForce;
    Vector3f m_moveOneClothForce;
    vector<Vector3f> m_normals;
    vector<bool> m_isIntersecting;
    bool stopMoving;
    bool stopOneMoving;

};


#endif
