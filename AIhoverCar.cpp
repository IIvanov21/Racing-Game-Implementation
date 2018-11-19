#include "AIhoverCar.h"
//Ivaylo Ivanov
void AIhoverCar::getFrameTime(float &frameTimeIn)//FrameTime passed in by a reference
{
	frameTime = frameTimeIn;
	
}
//myEngine passed in by a reference
AIhoverCar::AIhoverCar(I3DEngine*myEngineIn)
{
	myEngine = myEngineIn;
}
//Set up function for the NonPlayerCar
void AIhoverCar::CreateNonPlayerCar()
{
	nonPlayerCarMesh = myEngine->LoadMesh("race2.x");
	nonPlayerCarModel = nonPlayerCarMesh->CreateModel(-5.0f, 0.0f, -50.0f);
	nonPlayerCarRadius = 3.0f;
	nonPlayerCarModel->SetSkin("sp01.jpg");
	readyStart = false;//Used to check if the countDown has reached '0'
	
}
//Key Codes
EKeyCode StartGame(Key_Space);
//Creates the dummy targets which the AI Car follows
void AIhoverCar::CreateAITargets()
{
	dummy = myEngine->LoadMesh("Dummy.x");
	for (int i = 0; i < numberOfTargets; i++)
	{
		AICarTarget[i] = dummy->CreateModel(dummyXCoordinates[i], 0.0f, dummyZCoordinates[i]);
	}
	targetHit = 0;//(int) Used to track if the AI car has passed a checkpoint
}

void AIhoverCar::getNonPlayerCarCoordinates()//Constantly gets AIcar's X and Y coordinates for collision
{
	nonPlayerXCoordinates = nonPlayerCarModel->GetX();
	nonPlayerZCoordinates = nonPlayerCarModel->GetZ();
}

void AIhoverCar::getGameStatus(bool readyIn)//Checks if the countDown timer has ended on Start
{
	readyStart = readyIn;
}

void AIhoverCar::moveAIToTarget()//Movemement of the AIcar
{
	//Checks if the car has passed the dummy target
	bool pointCollision = AIhoverCar::pointToSphereCollision(nonPlayerCarModel->GetX(), nonPlayerCarModel->GetZ(), nonPlayerCarRadius, 
															dummyXCoordinates[targetHit], dummyZCoordinates[targetHit]);

	if (targetHit < numberOfTargets && readyStart)//As long as car is not at the last checkpoint 
	{
		nonPlayerCarModel->LookAt(AICarTarget[targetHit]);//Look at the current checkpoint
		nonPlayerCarModel->MoveLocalZ(AICarSpeed*frameTime);//Moves car 
	}

	if (pointCollision)
	{
		targetHit++;//Moves to the next target on collision with the current
	}

	
}

