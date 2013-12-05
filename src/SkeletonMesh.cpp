#include "SkeletonMesh.h"

using namespace std;

void SkeletonMesh::load( const char* filename )
{
	// 2.1.1. load() should populate bindVertices, currentVertices, and faces

	// Add your code here.

	ifstream in(filename);
	string line;
	while (getline(in, line)) {
	    stringstream ss(line);
	    char s;
	    ss >> s;
	    if (s == 'v') {
		Vector4f v;
		ss >> v[0] >> v[1] >> v[2];
		v[3] = 1;
		bindVertices.push_back(v);
	    }
	    else if (s == 'f') {
		Tuple3u newface;
		ss >> newface[0] >> newface[1] >> newface[2];
		faces.push_back(newface);
	    }
	}

	// make a copy of the bind vertices as the current vertices
	currentVertices = bindVertices;
}

void SkeletonMesh::draw()
{
	// Since these meshes don't have normals
	// be sure to generate a normal per triangle.
	// Notice that since we have per-triangle normals
	// rather than the analytical normals from
	// assignment 1, the appearance is "faceted".

	glBegin(GL_TRIANGLES);
	for (unsigned int it=0; it<faces.size(); it++) {
	    unsigned int a = faces[it][0];
	    unsigned int b = faces[it][1];
	    unsigned int c = faces[it][2];
	    Vector3f normal = Vector3f::cross(currentVertices[a-1].xyz()-currentVertices[b-1].xyz(), currentVertices[b-1].xyz()-currentVertices[c-1].xyz()).normalized();
	    glNormal3d(normal[0], normal[1], normal[2]);
	    glVertex3d(currentVertices[a-1][0], currentVertices[a-1][1], currentVertices[a-1][2]);
	    glVertex3d(currentVertices[b-1][0], currentVertices[b-1][1], currentVertices[b-1][2]);
	    glVertex3d(currentVertices[c-1][0], currentVertices[c-1][1], currentVertices[c-1][2]);
	}
	glEnd();
}

void SkeletonMesh::loadAttachments( const char* filename, int numJoints )
{
	// 2.2. Implement this method to load the per-vertex attachment weights
	// this method should update m_mesh.attachments

	ifstream in(filename);
	string line;
	while (getline(in, line)) {
	    stringstream ss(line);
	    vector<float> jointWeights(numJoints);
	    jointWeights[0] = 0;
	    for (int i=1; i<numJoints; i++) {
		ss >> jointWeights[i];
	    }
	    attachments.push_back(jointWeights);
	}
}
