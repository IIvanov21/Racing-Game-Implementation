//Ivaylo Ivanov
//This cpp should always include Collision.h header file
#include "Collision.h"

/*========================================================Sphere Collision=================================================================*/
bool Collision::sphereToSphereCollision(float sphereXPositionOne, float sphereZPositionOne, float sphereRadiusOne, float sphereXPositionTwo, float sphereZPositionTwo, float sphereRadiusTwo)
{
	//Its used to calculate the collision between two object like spheres using the sum of the two radii and checking them against the coordinates
	float distanceX = sphereXPositionTwo - sphereXPositionOne;
	float distanceZ = sphereZPositionTwo - sphereZPositionOne;
	float distance = sqrt(distanceX * distanceX + distanceZ * distanceZ);

	return(distance < (sphereRadiusOne + sphereRadiusTwo));
}

/*========================================================Point to Sphere Collision=================================================================*/
bool Collision::pointToSphereCollision(float sphereXPositionOne, float sphereZPositionOne, float sphereRadiusOne, float sphereXPositionTwo, float sphereZPositionTwo)
{
	//Its used to determine if an object has passed through a set of coordinates checking them against the object radius
	float distanceX = sphereXPositionTwo - sphereXPositionOne;
	float distanceZ = sphereZPositionTwo - sphereZPositionOne;
	float distance = sqrt(distanceX * distanceX + distanceZ * distanceZ);

	return(distance < (sphereRadiusOne));
}
/*========================================================Box Collision=================================================================*/
Collision::boxSides Collision::sphereToBoxCollision(float sphereXPosition, float sphereZPosition, float sphereRadius, float oldSphereXPosition, float oldSphereZPosition,
													float boxObjectXPosition, float boxObjectZPosition, float boxObjectWidth, float boxObjectDepth)
{/*Calculation of the object boundaries to check for collision between the two objects*/
	float minimumXBoundary = boxObjectXPosition - (boxObjectWidth / 2) - sphereRadius;
	float maximumXBoundary = boxObjectXPosition + (boxObjectWidth / 2) + sphereRadius;
	float minimumZBoundary = boxObjectZPosition - (boxObjectDepth / 2) - sphereRadius;
	float maximumZBoundary = boxObjectZPosition + (boxObjectDepth / 2) + sphereRadius;

	Collision::boxSides result = Collision::noSide; 

	if (sphereXPosition > minimumXBoundary && sphereXPosition<maximumXBoundary && sphereZPosition>minimumZBoundary && sphereZPosition < maximumZBoundary)//checks if sphere has collided with the box
	{
		//checks with a statement to decide which side of the object is hit
		if (oldSphereXPosition < minimumXBoundary)		result = Collision::leftSide;
		else if (oldSphereXPosition > maximumXBoundary) result = Collision::rightSide;
		else if (oldSphereZPosition < minimumZBoundary) result = Collision::frontSide;
		else     										result = Collision::backSide;
	}

	return(result);//returns the result to loop where the function is called
}



