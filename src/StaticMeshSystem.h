#ifndef STATICMESHSYSTEM_H
#define STATICMESHSYSTEM_H

#include "extra.h"
#include <vector>
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
    bool intersect(Vector3f& pos, Vector3f& normal);
private:
    Mesh m_mesh;
};


#endif
