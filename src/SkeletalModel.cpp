#include "SkeletalModel.h"

#include <FL/Fl.H>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

void SkeletalModel::load(const char *skeletonFile, const char *meshFile, const char *attachmentsFile)
{
	loadSkeleton(skeletonFile);

	m_mesh.load(meshFile);
	m_mesh.loadAttachments(attachmentsFile, m_joints.size());

	computeBindWorldToJointTransforms();
	updateCurrentJointToWorldTransforms();
}

void SkeletalModel::draw(Matrix4f cameraMatrix, bool skeletonVisible)
{
	// draw() gets called whenever a redraw is required
	// (after an update() occurs, when the camera moves, the window is resized, etc)

	m_matrixStack.clear();
	m_matrixStack.push(cameraMatrix);

	if( skeletonVisible )
	{
		drawJoints();

		drawSkeleton();
	}
	else
	{
		// Clear out any weird matrix we may have been using for drawing the bones and revert to the camera matrix.
		glLoadMatrixf(m_matrixStack.top());

		glPushMatrix();
		glTranslatef(-5.0f, -5.0f, -7.0f);
		glScaled(10.0f, 10.0f, 10.0f);
		//Tell the mesh to draw itself.
		m_mesh.draw();
		glPopMatrix();
	}
	m_matrixStack.pop();
}

void SkeletalModel::loadSkeleton( const char* filename )
{
	// Load the skeleton from file here.
    ifstream in(filename);
    string line;
    float x;
    float y;
    float z;
    int parent;
    while (getline(in, line)) {
	stringstream ss(line);
	Joint * newJoint = new Joint;
	ss >> x >> y >> z >> parent;
	Matrix4f jointTransform = Matrix4f::identity();
	jointTransform.setCol(3, Vector4f(x, y, z, 1));
	newJoint->transform = jointTransform;
	if (parent == -1) {
	    m_rootJoint = newJoint;
	}
	else {
	    Joint * parentJoint = m_joints[parent];
	    parentJoint->children.push_back(newJoint);
	}
	m_joints.push_back(newJoint);
    }
    in.close();
}

void SkeletalModel::drawJoints( )
{
	// Draw a sphere at each joint. You will need to add a recursive helper function to traverse the joint hierarchy.
	//
	// We recommend using glutSolidSphere( 0.025f, 12, 12 )
	// to draw a sphere of reasonable size.
	//
	// You are *not* permitted to use the OpenGL matrix stack commands
	// (glPushMatrix, glPopMatrix, glMultMatrix).
	// You should use your MatrixStack class
	// and use glLoadMatrix() before your drawing call.
    drawJointsRecursion(m_rootJoint);
    setJointTransform(12, 0, 0, -50);
}

void SkeletalModel::drawJointsRecursion(Joint * parentJoint) {
    m_matrixStack.push(parentJoint->transform);
    glLoadMatrixf(m_matrixStack.top());
    glutSolidSphere(0.025f, 12, 12);
    for (int i=0; i<parentJoint->children.size(); i++) {
	drawJointsRecursion(parentJoint->children[i]);
    }
    m_matrixStack.pop();
}

void SkeletalModel::drawSkeleton( )
{
	// Draw boxes between the joints. You will need to add a recursive helper function to traverse the joint hierarchy.
    drawSkeletonRecursion(m_rootJoint);

}

void SkeletalModel::drawSkeletonRecursion(Joint * parentJoint) {
    m_matrixStack.push(parentJoint->transform);
    for (int i=0; i<parentJoint->children.size(); i++) {
	Vector3f childCoords = parentJoint->children[i]->transform.getCol(3).xyz();

	// Rotate cube to point in direction of child joint
	Vector3f z = childCoords.normalized();
	Vector3f rnd = Vector3f(0, 0, 1);
	Vector3f y = Vector3f::cross(z, rnd).normalized();
	Vector3f x = Vector3f::cross(y, z).normalized();
	Matrix4f rotationMatrix = Matrix4f(Vector4f(x, 0), Vector4f(y, 0), Vector4f(z, 0), Vector4f(0, 0, 0, 1));
	m_matrixStack.push(rotationMatrix);

	// Scale cube to range from [-0.025, -0.025, 0] to [0.025, 0.025, l]
	m_matrixStack.push(Matrix4f::scaling(0.05, 0.05, childCoords.abs()));

	// Translate cube to range from [-0.5, -0.5, 0] to [0.5, 0.5, 1]
	Vector3f translationVector = Vector3f(0, 0, 0.5);
	m_matrixStack.push(Matrix4f::translation(translationVector));

	glLoadMatrixf(m_matrixStack.top());
	glutSolidCube(1.0f);
	m_matrixStack.pop();
	m_matrixStack.pop();
	m_matrixStack.pop();
	drawSkeletonRecursion(parentJoint->children[i]);
    }
    m_matrixStack.pop();
}

void SkeletalModel::setJointTransform(int jointIndex, float rX, float rY, float rZ)
{
	// Set the rotation part of the joint's transformation matrix based on the passed in Euler angles.
    Joint * joint = m_joints[jointIndex];
    Matrix3f rotateTransform = Matrix3f::rotateX(rX) * Matrix3f::rotateY(rY) * Matrix3f::rotateZ(rZ);
    joint->transform.setSubmatrix3x3(0, 0, rotateTransform);
}


void SkeletalModel::computeBindWorldToJointTransforms()
{
	// 2.3.1. Implement this method to compute a per-joint transform from
	// world-space to joint space in the BIND POSE.
	//
	// Note that this needs to be computed only once since there is only
	// a single bind pose.
	//
	// This method should update each joint's bindWorldToJointTransform.
	// You will need to add a recursive helper function to traverse the joint hierarchy.
    computeBindWorldToJointTransformsRecursion(m_rootJoint);
}

void SkeletalModel::computeBindWorldToJointTransformsRecursion(Joint * parentJoint) {
    m_matrixStack.push(parentJoint->transform);
    parentJoint->bindWorldToJointTransform = m_matrixStack.top().inverse();
    for (int i=0; i<parentJoint->children.size(); i++) {
	computeBindWorldToJointTransformsRecursion(parentJoint->children[i]);
    }
    m_matrixStack.pop();
}

void SkeletalModel::updateCurrentJointToWorldTransforms()
{
	// 2.3.2. Implement this method to compute a per-joint transform from
	// joint space to world space in the CURRENT POSE.
	//
	// The current pose is defined by the rotations you've applied to the
	// joints and hence needs to be *updated* every time the joint angles change.
	//
	// This method should update each joint's currentJointToWorldTransform.
	// You will need to add a recursive helper function to traverse the joint hierarchy.
    updateCurrentJointToWorldTransformsRecursion(m_rootJoint);
}

void SkeletalModel::updateCurrentJointToWorldTransformsRecursion(Joint * parentJoint) {
    m_matrixStack.push(parentJoint->transform);
    parentJoint->currentJointToWorldTransform = m_matrixStack.top();
    for (int i=0; i<parentJoint->children.size(); i++) {
	updateCurrentJointToWorldTransformsRecursion(parentJoint->children[i]);
    }
    m_matrixStack.pop();
}

void SkeletalModel::updateMesh()
{
	// 2.3.2. This is the core of SSD.
	// Implement this method to update the vertices of the mesh
	// given the current state of the skeleton.
	// You will need both the bind pose world --> joint transforms.
	// and the current joint --> world transforms.
    for (int i=0; i<m_mesh.currentVertices.size(); i++) {
	Vector4f newVertex;
	for (int j=0; j<m_joints.size(); j++) {
	   newVertex = newVertex + (m_mesh.attachments[i][j] * (m_joints[j]->currentJointToWorldTransform * m_joints[j]->bindWorldToJointTransform * m_mesh.bindVertices[i]));
	}
	m_mesh.currentVertices[i] = newVertex;
    } 
}

