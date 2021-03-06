#ifndef MESH_H
#define MESH_H

#include <vector>
#include <vecmath.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include "extra.h"
#include <algorithm> 
#include <stdlib.h>
#include <utility> 

//#include "tuple.h"

struct Mesh
{
	// list of vertices from the OBJ file
	// in the "bind pose"
	std::vector< Vector3f > bindVertices;

	// each face has 3 indices
	// referencing 3 vertices
	std::vector< Vector3f > faces;

	// current vertex positions after animation
	std::vector< Vector3f > currentVertices;

	// list of vertex to joint attachments
	// each element of attachments is a vector< float > containing
	// one attachment weight per joint
	std::vector< std::vector< float > > attachments;

	std::vector< Vector3f > joints;

	// 2.1.1. load() should populate bindVertices, currentVertices, and faces
	void load(const char *filename);

	// 2.1.2. draw the current mesh.
	void draw();

	//check each face to see if input position intersects with mesh
	std::pair <int, float> intersect(int index, Vector3f &pos, Vector3f& v,Vector3f &normal, float minDistance, bool selfIntersect);

	// 2.2. Implement this method to load the per-vertex attachment weights
	// this method should update m_mesh.attachments
	void loadAttachments( const char* filename, int numJoints );
};

#endif
