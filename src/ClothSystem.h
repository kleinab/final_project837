#ifndef CLOTHSYSTEM_H
#define CLOTHSYSTEM_H

#include "extra.h"
#include <map>
#include "particleSystem.h"

class ClothSystem: public ParticleSystem
{
public:
    ClothSystem(int numParticles);
	//ClothSystem(int x, int y);
	vector<Vector3f> evalF(vector<Vector3f> state);
	int convert(int x, int y);
    bool isInGrid(int x, int y);
    map<int, float> getPatch(int index, int size);
    Vector3f calcForce(int i, float g, float dragk, vector<Vector3f> state);
	void draw();
    void move(Vector3f displacement);
    void moveOne(Vector3f displacement);
    Vector3f calculateNormal(int i, int j, vector<Vector3f> states);
    pair<int, float> intersect(int index, Vector3f& pos, Vector3f& v, Vector3f& normal, bool selfIntersect);
    int distanceToMesh(int i, Vector3f &pos, Vector3f &v);
    int distanceToEdgeMesh(int i, int b, Vector3f &posa, Vector3f & posb, Vector3f &va, Vector3f &vb);
    void setState(const vector<Vector3f>  & newState);
    vector<int> getConnections(int i){return m_connections.at(i);}
    string getClass(){return "ClothSystem";}
private:
    int m_numX;
    int m_numY;
    vector<float> m_particleMasses;
    map<int, float> m_patch;
    vector< vector < Vector3f > > m_vectorSprings;
    vector< vector < int > > m_connections;
    Vector3f m_moveClothForce;
    Vector3f m_moveOneClothForce;
    vector<Vector3f> m_normals;
    vector<bool> m_isIntersecting;
    bool stopMoving;
    bool stopOneMoving;
    Mesh m_clothMesh;

};


#endif
