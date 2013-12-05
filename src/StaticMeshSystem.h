#ifndef STATICMESHSYSTEM_H
#define STATICMESHSYSTEM_H

#include "extra.h"
#include "Mesh.h"

#include "particleSystem.h"

class StaticMeshSystem: public ParticleSystem
{
public:
    StaticMeshSystem(Mesh &m);
    vector<Vector3f> evalF(vector<Vector3f> state);
	void draw();
    void move(Vector3f displacement);
    void moveOne(Vector3f displacement);
    pair<int, float> intersect(int index,Vector3f& pos, Vector3f& v, Vector3f& normal, bool selfIntersect);
    int distanceToMesh(int i, Vector3f &pos,Vector3f &v){return 0;}
    vector<int> getConnections(int i){ vector<int> a; return a;}
    int distanceToEdgeMesh(int i, int b, Vector3f &posa, Vector3f & posb, Vector3f &va, Vector3f &vb){return 0;}
    string getClass(){return "StaticMeshSystem";}
private:
    Mesh m_clothMesh;
};


#endif
