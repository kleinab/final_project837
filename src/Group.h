#ifndef GROUP_H
#define GROUP_H

//#include "Object3D.h"
#include "particleSystem.h"
#include <iostream>
#include <vector>

using  namespace std;

class Group
{
public:

    Group( int num_objects ){
	this->objects.resize(num_objects);
	this->num_objects = num_objects;
    }

    ~Group(){
	objects.clear();
    }

    virtual bool intersect(Vector3f& pos, Vector3f& normal) {
	bool intersection = false;
	for (int i=0; i<num_objects; i++) {
	    if (objects[i]->intersect(pos, normal)) {
		intersection = true;
	    }
	}
	return intersection;
    }

    void addObject( int index , ParticleSystem* system ){
	objects[index] = system;
    }

    int getGroupSize(){ 
	return num_objects;
    }

private:
    vector<ParticleSystem*> objects;
    int num_objects;
};

#endif

