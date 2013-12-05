
#include "TimeStepper.h"
#include <iostream>

// implement ClothSimulation time integrator here
//Advance to candidate positions x^n+1 
//and velocities v^n+1 at time t^n+1 with the cloth internal dynamics
//Compute the average velocity v^n+1/2 = x^n+1-x^n/deltaT

//Check x^n for proximity, apply repulsion inpulses and friction to v^n+1/2 to get ~v^n+1/2

//Check linear trajectories from x^n with ~v^n+1/2 for collisions, 
//resolving them with a final midstep velocity v^n+1/2
//compute final positions x^n+1 = x^n + deltaT*v^n+1/2
//if no repulsions/collisions, v^n+1 = v^n+1
    //else  v^n+1 = v^n+1/2

ClothSimulation::ClothSimulation(){
	timeStepper = new RK4();
}

void ClothSimulation::takeStep(ParticleSystem* particleSystem, float stepSize)
{
    vector<Vector3f> oldvVecState = particleSystem->getState();
    vector<Vector3f> f = particleSystem -> evalF(oldvVecState);
    vector<Vector3f> newvVecState; 	
 	timeStepper->takeStep(particleSystem,stepSize);
	vector<Vector3f> newState = particleSystem->getState(); 	
	particleSystem->setState(oldvVecState);

    for (int i = 0; i< (oldvVecState.size()/2); i++){
        Vector3f oldX = oldvVecState.at(i*2);
     	Vector3f oldV = oldvVecState.at(i*2 + 1); 

	    Vector3f newX = newState.at(i*2);
        Vector3f newV = newState.at(i*2 + 1);

        Vector3f averageV1 = (newX-oldX)/stepSize;
        //apply the collision/repulsion changes to the average velocity
        if (particleSystem -> distanceToMesh(i, oldX, averageV1)>0){
            newV = averageV1;
            newX = oldX + stepSize*newV;
        }

        /*vector<int> connections = particleSystem->getConnections(i);
        for (int k = 0; k< connections.size(); k++){
            Vector3f b =  newState.at(k*2);
            Vector3f vb = newState.at(k*2+1);
            if (particleSystem -> distanceToEdgeMesh(i, k, newX, b, averageV1)>0){
                newV = averageV1;
                newX = oldX + stepSize*newV;
            }
            newState.at(k*2+1) = vb;
        }*/
        if (particleSystem -> distanceToMesh(i, newX, averageV1)>0){
            newV = (averageV1+newV)/2;
            newX = oldX + stepSize*newV;
        }

        newvVecState.push_back(newX);
        newvVecState.push_back(newV);
    }

    particleSystem->setState(newvVecState);
}