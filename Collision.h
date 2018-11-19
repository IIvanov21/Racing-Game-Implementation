//Ivaylo Ivanov
#include "TL-Engine.h"
#include "GameStates.h"
#include <cmath>
using namespace tle;
#ifndef COLLISION_H
#define COLLISION_H
class Collision:public gameStatesDecision//Child class of master class Game States
{
public://The Collision class defines all the necessary functions needed to check for collision 
	bool sphereToSphereCollision(float sphereXPositionOne, float sphereZPositionOne, float sphereRadiusOne, float sphereXPositionTwo, float sphereZPositionTwo, float sphereRadiusTwo);

	enum boxSides { frontSide, backSide, leftSide, rightSide, noSide };//States used to return which object side was hit

	boxSides sphereToBoxCollision(float sphereXPosition, float sphereZPosition, float sphereRadius, float oldSphereXPosition, float oldSphereZPosition,
		float boxObjectXPosition, float boxObjectZPosition, float boxObjectWidth, float boxObjectDepth);

	bool Collision::pointToSphereCollision(float sphereXPositionOne, float sphereZPositionOne, float sphereRadiusOne, float sphereXPositionTwo, float sphereZPositionTwo);

	
};
#endif 

