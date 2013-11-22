#include "TimeStepper.h"
#include <iostream>

// implement Explicit Euler time integrator here
void ForwardEuler::takeStep(ParticleSystem* particleSystem, float stepSize)
{
    vector<Vector3f> oldvVecState = particleSystem->getState();
    vector<Vector3f> f = particleSystem -> evalF(oldvVecState);
    vector<Vector3f> newvVecState;

    for (int i = 0; i< (oldvVecState.size()/2); i++){
        
        Vector3f oldX = oldvVecState.at(i*2);
		Vector3f oldV = oldvVecState.at(i*2+1);
        Vector3f x1 = f.at(i*2);
        Vector3f v1 = f.at(i*2+1);
    
        Vector3f newX = oldX + stepSize*x1;
	    Vector3f newV = oldV + stepSize*v1;
        newvVecState.push_back(newX);
        newvVecState.push_back(newV);
    }

    particleSystem->setState(newvVecState);
}

// implement Trapzoidal rule here
void Trapzoidal::takeStep(ParticleSystem* particleSystem, float stepSize)
{
    vector<Vector3f> oldvVecState = particleSystem->getState();
    vector<Vector3f> f0 = particleSystem -> evalF(oldvVecState);
    vector<Vector3f> endpoint;

    for (int i = 0; i< (oldvVecState.size()/2); i++){
        Vector3f x1 = f0.at(i*2);
        Vector3f v1 = f0.at(i*2+1);
        Vector3f oldX = oldvVecState.at(i*2);
		Vector3f oldV = oldvVecState.at(i*2+1);
        Vector3f newX = oldX + stepSize*x1;
	    Vector3f newV = oldV + stepSize*v1;
        endpoint.push_back(newX);
        endpoint.push_back(newV);
    }

    vector<Vector3f> f1 = particleSystem -> evalF(endpoint);
    vector<Vector3f> newvVecState;

    for (int i = 0; i< (oldvVecState.size()/2); i++){
        Vector3f oldX = oldvVecState.at(i*2);
		Vector3f oldV = oldvVecState.at(i*2+1);
        Vector3f newX = oldX + 0.5*stepSize*(f0.at(i*2)+ f1.at(i*2));
        Vector3f newV = oldV + 0.5*stepSize*(f0.at(i*2+1)+ f1.at(i*2+1));
        newvVecState.push_back(newX);
        newvVecState.push_back(newV);
    }

    particleSystem->setState(newvVecState);
}
