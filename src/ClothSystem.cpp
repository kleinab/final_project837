#include "ClothSystem.h"
#include <iostream>
#include <math.h>

ClothSystem::ClothSystem(int numParticles):ParticleSystem(numParticles)
{  
    float TOTAL_GRID_SIZE = 2.0;
    displayMesh = true;
    m_numX = numParticles;
    m_numY = numParticles;
    m_numParticles = m_numX*m_numY;	
    m_moveClothForce = Vector3f(0,0,0);
    m_moveOneClothForce = Vector3f(0,0,0);
    stopMoving = true;
    stopOneMoving = true;
    float kConstantStruct = 8;
    float restLengthStruct = TOTAL_GRID_SIZE/(numParticles);
        
    float kConstantSheer = 4;
    float restLengthSheer = pow(2.0*pow(TOTAL_GRID_SIZE/(numParticles),2.0),0.5);

    float kConstantFlex = 4;
    float restLengthFlex = (2*TOTAL_GRID_SIZE*1.0)/(numParticles);
    vector< Vector3f > springs;
    	        
    vector<Vector3f> particleStates;

    //for each point on the grid
    for (int i = 0; i< m_numY; i++){
        for (int j = 0; j< m_numX; j++){
            m_particleMasses.push_back(3.0/float(m_numParticles));
            vector< Vector3f > particle_springs;
		
            //add struct springs
            if (isInGrid(i-1, j)){
                int index = convert(i-1, j);
                Vector3f spring = Vector3f(index, kConstantStruct, restLengthStruct);
                particle_springs.push_back(spring);
            }
            if (isInGrid(i, j-1)){
                int index = convert(i, j-1);
                Vector3f spring = Vector3f(index, kConstantStruct, restLengthStruct);
                particle_springs.push_back(spring);
            } 
            if (isInGrid(i+1, j)){
                int index = convert(i+1, j);
                Vector3f spring = Vector3f(index, kConstantStruct, restLengthStruct);
                particle_springs.push_back(spring);
            }   
            if (isInGrid(i, j+1)){
                int index = convert(i, j+1);
                Vector3f spring = Vector3f(index, kConstantStruct, restLengthStruct);
                particle_springs.push_back(spring);    
            }

            //add sheer springs
            if (isInGrid(i-1, j-1)){
                int index = convert(i-1, j-1);
                Vector3f spring = Vector3f(index, kConstantSheer, restLengthSheer);
                particle_springs.push_back(spring);
            }
            if (isInGrid(i-1, j+1)){
                int index = convert(i-1, j+1);
                Vector3f spring = Vector3f(index, kConstantSheer, restLengthSheer);
                particle_springs.push_back(spring);
            } 
            if (isInGrid(i+1, j-1)){
                int index = convert(i+1, j-1);
                Vector3f spring = Vector3f(index, kConstantSheer, restLengthSheer);
                particle_springs.push_back(spring);
            }   
            if (isInGrid(i+1, j+1)){
                int index = convert(i+1, j+1);
                Vector3f spring = Vector3f(index, kConstantSheer, restLengthSheer);
                particle_springs.push_back(spring);    
            }  


            //add flex springs
            if (isInGrid(i-2, j)){
                int index = convert(i-2, j);
                Vector3f spring = Vector3f(index, kConstantFlex, restLengthFlex);
                particle_springs.push_back(spring);
            }
            if (isInGrid(i, j-2)){
                int index = convert(i, j-2);
                Vector3f spring = Vector3f(index, kConstantFlex, restLengthFlex);
                particle_springs.push_back(spring);
            } 
            if (isInGrid(i+2, j)){
                int index = convert(i+2, j);
                Vector3f spring = Vector3f(index, kConstantFlex, restLengthFlex);
                particle_springs.push_back(spring);
            }   
            if (isInGrid(i, j+2)){
                int index = convert(i, j+2);
                Vector3f spring = Vector3f(index, kConstantFlex, restLengthFlex);
                particle_springs.push_back(spring);    
            }

            m_vectorSprings.push_back(particle_springs);
            //initiate all initial state
            Vector3f firstStartStatePos = Vector3f((float(i)*TOTAL_GRID_SIZE)/float(m_numX),0,(float(j)*TOTAL_GRID_SIZE)/float(m_numY));
	    Vector3f firstStartStateVel = Vector3f(0,0,0);
	    particleStates.push_back(firstStartStatePos);
	    particleStates.push_back(firstStartStateVel);
        } 
    }
    
    setState(particleStates);
}

//converts x,y coordinates to index
int ClothSystem::convert(int x, int y){
    return x*m_numX + y;
}

bool ClothSystem::isInGrid(int x, int y){
    if ((x>=0 && x<m_numX)&& (y>=0 && y<m_numY)){
        return true;
    }
    return false;
}


void ClothSystem::move(Vector3f displacement){
    if (m_moveClothForce == displacement){
        stopMoving = true;
        m_moveClothForce = Vector3f(0,0,0);
    }else{
        stopMoving = false;
        m_moveClothForce = displacement;
    }
}

void ClothSystem::moveOne(Vector3f displacement){
    if (m_moveOneClothForce == displacement){
        stopOneMoving = true;
        m_moveOneClothForce = Vector3f(0,0,0);
    }else{
        stopOneMoving = false;
        m_moveOneClothForce = displacement;
    }
}

Vector3f ClothSystem::calcForce(int i, float g, float dragk, vector<Vector3f> state){
    
    Vector3f x = state.at(i*2);
    Vector3f v = state.at(i*2 +1); 

    float mass = m_particleMasses.at(i);
	vector<Vector3f> mySprings = m_vectorSprings.at(i);
	Vector3f springForce = Vector3f(0,0,0);

    
	for(int j = 0; j< mySprings.size(); j++){
	    Vector3f spring = mySprings.at(j);
	    Vector3f otherX = state.at(spring[0]*2);
	    Vector3f distance = x - otherX;
	    springForce += -spring[1]*(distance*((distance.abs()-spring[2])/spring[2]));
        
	}

    Vector3f gravity = Vector3f(0,g,0);
    Vector3f drag = -1*dragk*v;
    Vector3f sol = (gravity+drag+springForce)*1.0/mass;
    return sol;
}

// for a given state, evaluate f(X,t)
vector<Vector3f> ClothSystem::evalF(vector<Vector3f> state)
{
	float g = -1.0;
    float dragk = 0.5;
	vector<Vector3f> f;
    vector<Vector3f> states = getState();
    for(int i=0; i< m_numParticles; i++){
        Vector3f newForce = calcForce(i, g, dragk, state);
        Vector3f v = state.at(i*2 +1);
        f.push_back(v);
        f.push_back(newForce);
    }
	return f;
}
Vector3f ClothSystem::calculateNormal(int i, int j, vector<Vector3f> states){
    int index = convert(i, j);
    Vector3f pos = states.at(index*2);
    Vector3f one = Vector3f(0,0,0);
    Vector3f two = Vector3f(0,0,0);
    Vector3f three = Vector3f(0,0,0);
    Vector3f four = Vector3f(0,0,0);
    Vector3f nab = Vector3f(0,0,0);
    Vector3f nbc = Vector3f(0,0,0);
    Vector3f ncd = Vector3f(0,0,0);
    Vector3f nda = Vector3f(0,0,0);
    bool a = false;
    bool b = false;
    bool c = false;
    bool d = false;

    //add struct springs
    if (isInGrid(i, j+1)){
        index = convert(i, j+1);
        one = states.at(index*2) - pos;
        a = true;
    }
    if (isInGrid(i+1, j)){
        index = convert(i+1, j);
        two = states.at(index*2)- pos;
        b = true;
    } 
    if (isInGrid(i, j-1)){
        index = convert(i, j-1);
        three = states.at(index*2)- pos;
        c = true;
    }   
    if (isInGrid(i-1, j)){
        index = convert(i-1, j);
        four = states.at(index*2)- pos;
        d = true;
    }

    if (a&&b){
        nab = -1*one.cross(-one,two);
    }
    
    if (b&&c){
        nbc = -1*two.cross(-two, three);
    }

    if (c&&d){
        ncd = -1*three.cross(-three,four);
    }

    if (d&&a){
        nda = -1*four.cross(-four,one);
    }

    Vector3f sol = nab + nbc;
    sol+=ncd;
    sol+=nda;
    return sol.normalized();
}

bool ClothSystem::intersect(Vector3f& pos, Vector3f& normal) {
    return true;
}

void ClothSystem::draw()
{
    //if (isUsingColor){
    //    glDisable (GL_LIGHTING);
    //}
    vector<Vector3f> states = getState();
    if (displayMesh){
        //cout<<"b"<<endl;
        for (int i = 0; i < m_numParticles; i++) {
		    Vector3f pos = states.at(i*2);//  position of particle i. YOUR CODE HERE
            vector<Vector3f> mySprings = m_vectorSprings.at(i);
	        
	        for(int j = 0; j< mySprings.size(); j++){
	            Vector3f spring = mySprings.at(j);
	            Vector3f otherX = states.at(spring[0]*2);
	            Vector3f distance = pos - otherX;
                glBegin(GL_LINES);
                     glVertex3f(pos[0], pos[1],pos[2]);
                     glVertex3f(otherX[0],otherX[1],otherX[2]);
                glEnd();
	        }
            glPushMatrix();
		    glTranslatef(pos[0], pos[1], pos[2] );
		    glutSolidSphere(0.075f,10.0f,10.0f);
		    glPopMatrix();
	    }
    }else{
        glEnable (GL_LIGHTING);
        glShadeModel( GL_SMOOTH );
        glEnable( GL_DEPTH_TEST );
        glEnable( GL_LIGHT0 );
        glEnable( GL_NORMALIZE );

        GLfloat Lt0diff[] = {1.0,1.0,1.0,1.0};
        GLfloat Lt0pos[] = {3.0,3.0,5.0,1.0};
        glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
        glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);

        m_normals.clear();
        for (int i = 0; i< m_numY; i++){
            for (int j = 0; j< m_numX; j++){
                Vector3f normal = calculateNormal(i, j, states);
                m_normals.push_back(normal);
            }
        }
        //cout<<"ss"<<endl;
        for (int i = 0; i< m_numY-1; i++){
            for (int j = 0; j< m_numX-1; j++){
                m_particleMasses.push_back(1);
                vector< Vector3f > particle_springs;
                int indexA = convert(i,j)*2;
                int indexB = convert(i, j+1)*2;
                int indexC = convert(i+1, j+1)*2;
                int indexD = convert(i+1, j)*2; 

                Vector3f a = states.at(indexA);
                Vector3f b = states.at(indexB);
                Vector3f c = states.at(indexC);
                Vector3f d = states.at(indexD);

                Vector3f na = m_normals.at(indexA/2);
                Vector3f nb = m_normals.at(indexB/2);
                Vector3f nc = m_normals.at(indexC/2);
                Vector3f nd = m_normals.at(indexD/2);

                glBegin(GL_TRIANGLES);
                glNormal3d(na[0], na[1], na[2]);
	            glVertex3f(a[0], a[1], a[2]);
                glNormal3d(nb[0], nb[1], nb[2]);
                glVertex3f(b[0], b[1], b[2]);
                glNormal3d(nc[0], nc[1], nc[2]);
	            glVertex3f(c[0], c[1], c[2]);
                glEnd();
    
                glBegin(GL_TRIANGLES);
                glNormal3d(-nc[0], -nc[1], -nc[2]);
	            glVertex3f(c[0], c[1], c[2]);
                glNormal3d(-nb[0], -nb[1], -nb[2]);
                glVertex3f(b[0], b[1], b[2]);
                glNormal3d(-na[0], -na[1], -na[2]);
	            glVertex3f(a[0], a[1], a[2]);
                glEnd();


                glBegin(GL_TRIANGLES);
                glNormal3d(nc[0], nc[1], nc[2]);
	            glVertex3f(c[0], c[1], c[2]);
                glNormal3d(nd[0], nd[1], nd[2]);
                glVertex3f(d[0], d[1], d[2]);
                glNormal3d(na[0], na[1], na[2]);
	            glVertex3f(a[0], a[1], a[2]);
                glEnd();
                
                glBegin(GL_TRIANGLES);
                glNormal3d(-na[0], -na[1], -na[2]);
	            glVertex3f(a[0], a[1], a[2]);
                glNormal3d(-nd[0], -nd[1], -nd[2]);
                glVertex3f(d[0], d[1], d[2]);
                glNormal3d(-nc[0], -nc[1], -nc[2]);
	            glVertex3f(c[0], c[1], c[2]);
                glEnd();
            } 
        }
    }
}


