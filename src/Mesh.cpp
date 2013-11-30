#include "Mesh.h"

using namespace std;

void Mesh::load( const char* filename )
{
	// 2.1.1. load() should populate bindVertices, currentVertices, and faces
    ifstream infile(filename);
    std::string line;
    while (getline(infile, line))
    {
        Vector3f v;
        string s;
        std::istringstream iss(line);
        iss >> s;
		if (s == "v") {  //vertices
		    iss >> v[0] >> v[1] >> v[2];
		    bindVertices.push_back(v);
		}else if (s == "f"){ //face
            Vector3f face = Vector3f(0,0,0);
		    iss >> face[0] >> face[1] >> face[2];
		    faces.push_back(face);
		}else{
		  //cout<<"wrong input"<<endl;
		}
    }

	// make a copy of the bind vertices as the current vertices
	currentVertices = bindVertices;
}

bool Mesh::intersect(Vector3f &pos, float minDistance){
    bool isIntersect = false;
    for(int i = 0; i < faces.size(); i++) {
        Vector3f face = faces.at(i);
        float v1 = face[0]-1;
        float v2 = face[1]-1;
        float v3 = face[2]-1;

        Vector3f ac = currentVertices[v1]-currentVertices[v3];
        Vector3f bc = currentVertices[v2]-currentVertices[v3];
        Vector3f cb = currentVertices[v3]-currentVertices[v2];
        Vector3f ab = currentVertices[v1]-currentVertices[v2];

        float sideLength[3] = {ac.abs(), bc.abs(), ab.abs()};
        float charL = *max_element(sideLength,sideLength+3);

        Vector3f n1 = Vector3f::cross(ac,cb);
        n1.normalize();
        //cout<<currentVertices[v3][0]<<currentVertices[v3][1]<<currentVertices[v3][2]<<endl;
        //cout<<currentVertices[v2][0]<<currentVertices[v2][1]<<currentVertices[v2][2]<<endl;
        //cout<<currentVertices[v1][0]<<currentVertices[v1][1]<<currentVertices[v1][2]<<endl;
        //cout<<n1[0]<<n1[1]<<n1[2]<<"a"<<endl;

        Vector3f dc = pos - currentVertices[v3];
        if (abs(Vector3f::dot(dc, n1))<minDistance){
            Matrix2f a = Matrix2f(Vector3f::dot(ac, ac), Vector3f::dot(ac, bc),
                                  Vector3f::dot(ac, bc), Vector3f::dot(bc, bc));
            Vector2f w12 = a.inverse() *Vector2f(Vector3f::dot(ac, dc),Vector3f::dot(bc, dc));
            float w1 = w12[0];
            float w2 = w12[1];
            float w3 = 1-w1-w2;
            float delta = minDistance/charL;
            if((w1>-delta && w1< 1 + delta)&&(w2>-delta && w2< 1 + delta)&&(w3>-delta && w3< 1 + delta)){
                isIntersect = true;
            }
        }
    }
    return isIntersect;
}

// Since these meshes don't have normals
// be sure to generate a normal per triangle.
// Notice that since we have per-triangle normals
// rather than the analytical normals from
// assignment 1, the appearance is "faceted".
void Mesh::draw()
{
	for(unsigned int i=0; i < faces.size(); i++) {
	    Vector3f face = faces[i];
        unsigned v1 = face[0]-1;
        unsigned v2 = face[1]-1;
        unsigned v3 = face[2]-1;

        Vector3f ab = currentVertices[v2]-currentVertices[v1];
        Vector3f cb = currentVertices[v2]-currentVertices[v3];
        Vector3f n1 = -Vector3f::cross(ab,cb);
        n1.normalize();

		glBegin(GL_TRIANGLES);
        glNormal3d(n1[0], n1[1], n1[2]);
	    glVertex3f(currentVertices[v1][0], currentVertices[v1][1], currentVertices[v1][2]);
        glVertex3f(currentVertices[v2][0], currentVertices[v2][1], currentVertices[v2][2]);
	    glVertex3f(currentVertices[v3][0], currentVertices[v3][1], currentVertices[v3][2]);
    	glEnd();
	}
}

void Mesh::loadAttachments( const char* filename, int numJoints )
{
	// 2.2. Implement this method to load the per-vertex attachment weights
	// this method should update m_mesh.attachments

    ifstream infile(filename);
    std::string line;
    while (getline(infile, line))
    {
        vector< float > weights;
        weights.push_back(0);
        std::istringstream iss(line);
        string s;
        for(int i=1; i<numJoints; i++){
            float w;
            iss >> w;
            weights.push_back(w);
        }
        attachments.push_back(weights);
    }
}
