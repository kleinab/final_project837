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
		    bindVertices.push_back(v*10);
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

float clamp(float w){
    if (w>1){
        return 1.0;
    }else if(w<0){
        return 0.0;
    }else{
        return w;
    }
}

pair<int, float> Mesh::intersect(int index, Vector3f &pos, Vector3f& v, Vector3f &normal, float minDistance, bool selfIntersect){
    int isIntersect = 0;
    float d = 0;
    for(int i = 0; i < faces.size(); i++) {
        Vector3f face = faces.at(i);
        float v1 = face[0]-1;
        float v2 = face[1]-1;
        float v3 = face[2]-1;

        if((selfIntersect && index!=v1 && index!=v2, index!=v3) || !selfIntersect){
            Vector3f a = currentVertices[v1];
            Vector3f b = currentVertices[v2];
            Vector3f c = currentVertices[v3];

            Vector3f ac = a-c;
            Vector3f bc = b-c;
            Vector3f cb = c-b;
            Vector3f ab = a-b;

            Vector3f n1 = Vector3f::cross(ac,cb);
            n1.normalize();
            Vector3f dc = pos - c;

            float isColliding = 0.01;
            if (selfIntersect){
                isColliding = 0.005;
                dc =  currentVertices[index] - c;
                if (Vector3f::dot(dc, n1)>0){
                    n1 = -n1;
                }
            }
            
            if (abs(Vector3f::dot(dc, n1))<minDistance){
                Vector3f projected_point = pos - Vector3f::dot(dc, n1)*normal;

                Matrix2f matrixa = Matrix2f(Vector3f::dot(ac, ac), Vector3f::dot(ac, bc),
                                      Vector3f::dot(ac, bc), Vector3f::dot(bc, bc));
                Vector2f w12 = matrixa.inverse() *Vector2f(Vector3f::dot(ac, projected_point-c),Vector3f::dot(bc, projected_point-c));
                float w1 = w12[0];
                float w2 = w12[1];
                float w3 = 1-w1-w2;
                //float delta = minDistance/charL;

                Vector3f closest = clamp(w1)*a +clamp(w2)*b +clamp(w3)*c;
                float dis = (pos - closest).abs();
                if (dis<minDistance){
                    if(dis<minDistance-isColliding){
                        isIntersect = 2;
                        normal = n1;   
                    }else{
                        isIntersect = 1;
                        normal = n1;
                        d = minDistance - Vector3f::dot((pos - w1*currentVertices[v1] -w2*currentVertices[v2] -w3*currentVertices[v3]),normal);
                    }
                }
            }
        }
    }
    pair<int, float> result (isIntersect,d);
    return result;
}

/*
int Mesh::intersectLine(Vector3f &pos3, Vector3f &pos4, Vector3f &normal, float minDistance){
    int isIntersect = false;
    for(int i = 0; i < faces.size(); i++) {
        Vector3f face = faces.at(i);
        float v1 = face[0]-1;
        float v2 = face[1]-1;
        float v3 = face[2]-1;
        Vector3f a = currentVertices[v1];
        Vector3f b = currentVertices[v2];
        Vector3f c = currentVertices[v3];

        Vector3f ac = a-c;
        Vector3f bc = b-c;
        Vector3f cb = c-b;
        Vector3f ab = a-b;

        Vector3f v43 = pos4-pos3;
        float sideLength[3] = {ac.abs(), bc.abs(), ab.abs()};
        float charL = *max_element(sideLength,sideLength+3);

        Vector3f n1 = Vector3f::cross(ac,cb);
        n1.normalize();


        if (Vector3f::cross(v43, ac).abs()>epsilon){
            Vector3f v31 = pos3 - a;
            Matrix2f m = Matrix2f(Vector3f::dot(ac, ac), Vector3f::dot(-ac, v43),
                                  Vector3f::dot(-ac, v43), Vector3f::dot(v43, v43));
            Vector2f wab = m.inverse() *Vector2f(Vector3f::dot(ac, v31),Vector3f::dot(-v43, v31));
            float a = wab[0];
            float b = wab[1];
            float delta = minDistance/charL;
            if((w1>-delta && w1< 1 + delta)&&(w2>-delta && w2< 1 + delta)&&(w3>-delta && w3< 1 + delta)){
                isIntersect = true;
                normal = -n1;
            }
        }else if (Vector3f::cross(v43, bc).abs()>epsilon){
            Vector3f v31 = pos3 - b;
            Matrix2f m = Matrix2f(Vector3f::dot(bc, bc), Vector3f::dot(-bc, v43),
                                  Vector3f::dot(-bc, v43), Vector3f::dot(v43, v43));
            Vector2f wab = m.inverse() *Vector2f(Vector3f::dot(bc, v31),Vector3f::dot(-v43, v31));
            float a = wab[0];
            float b = wab[1];
            float delta = minDistance/charL;
            if((w1>-delta && w1< 1 + delta)&&(w2>-delta && w2< 1 + delta)&&(w3>-delta && w3< 1 + delta)){
                isIntersect = true;
                normal = -n1;
            }
        }else if (Vector3f::cross(v43, ab).abs()>epsilon){
            Vector3f v31 = pos3 - c;
            Matrix2f m = Matrix2f(Vector3f::dot(ab, ab), Vector3f::dot(-ab, v43),
                                  Vector3f::dot(-ab, v43), Vector3f::dot(v43, v43));
            Vector2f wab = m.inverse() *Vector2f(Vector3f::dot(ab, v31),Vector3f::dot(-v43, v31));
            float a = wab[0];
            float b = wab[1];
            float delta = minDistance/charL;
            if((w1>-delta && w1< 1 + delta)&&(w2>-delta && w2< 1 + delta)&&(w3>-delta && w3< 1 + delta)){
                isIntersect = true;
                normal = -n1;
            }
        }
    }
    return isIntersect;
}*/


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
/*
void Mesh::loadSkeleton( const char* filename )
{
    ifstream infile(filename);
    std::string line;
    while (getline(infile, line))
    {
        std::istringstream iss(line);
        float a, b, c;
        iss >> a >> c>>c;
        Vector3f joint = Vector3f(a,b,c);
        joints.push_back(joint);
    }
}*/

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
