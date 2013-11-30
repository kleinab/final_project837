#ifndef SPHERESYSTEM_H
#define SPHERESYSTEM_H

#include "extra.h"
#include <vector>

#include "particleSystem.h"

class SphereSystem: public ParticleSystem
{
public:
    SphereSystem(Vector3f &pos, float r);
    vector<Vector3f> evalF(vector<Vector3f> state);
	void draw();
    void move(Vector3f displacement);
    void moveOne(Vector3f displacement);
    bool intersect(Vector3f& pos, Vector3f& normal);
private:
    float radius;
    Vector3f center;
};


#endif
