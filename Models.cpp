//Ivaylo Ivanov
#include "Models.h"
void Environment::GetMyEngine(I3DEngine*& myEngineIn)
{
	myEnginePassed = myEngineIn;
}

void Environment::LoadMeshes()
{
	//Loads all the meshes used to create the models
	myEnginePassed->AddMediaFolder(".\\media");
	skyboxMesh = myEnginePassed->LoadMesh("Skybox 07.x");
	groundMesh = myEnginePassed->LoadMesh("ground.x");
	checkPointMesh = myEnginePassed->LoadMesh("Checkpoint.x");
	isleMesh = myEnginePassed->LoadMesh("IsleStraight.x");
	wallMesh = myEnginePassed->LoadMesh("Wall.x");
	waterTankMesh = myEnginePassed->LoadMesh("TankSmall1.x");
}



void Environment::CreateModels()
{
	//Skybox Model
	skybox_Y = -960.0f;
	skybox = skyboxMesh->CreateModel(0.0f,skybox_Y,0.0f);
	//Ground Model
	ground = groundMesh->CreateModel();

	//Checkpoint Models Creation
	//All(floats)coordinates that are need for collision
	checkPointLegsRadius = 0.5f;
	checkPointDepth = 1.0f;
	checkPointWidth = 8.0f;
	for (int i = 0; i < checkPointNum; i++)//Iteration for CheckPoint
	{
		checkPoint[i] = checkPointMesh->CreateModel(checkPointsX[i], 0.0f, checkPointsZ[i]);//Uses the coordinates store in the array
		if (i == 2)
		{
			checkPoint[i]->RotateY(checkPointRotation); 
		}
		if (i == 3)
		{
			
			checkPoint[i]->RotateY(roatatebyRightAngle); 
		}
		//(float)Arrays that stores each models coordinates and used to pass them between classes
		checkPointLegsX[i] = checkPoint[i]->GetX();
		checkPointLegsZ[i] = checkPoint[i]->GetZ();
	}
	
	//Isle Models Creation
	
	for (int i = 0; i < isleNum; i++)//Iteration for isle Models
	{
		isle[i] = isleMesh->CreateModel(isles_X[i], 0.0f, isles_Z[i]);
		if (i >= 4)isle[i]->RotateY(roatatebyRightAngle);
	}
	
	//Wall Models Creation
	//(floats)Used to check for collision
	wallWidth = 3.0f;
	wallDepth = 14.0f;
	for (int i = 0; i < wallNum; i++)//Iteration for Walls
	{
		wall[i] = wallMesh->CreateModel(walls_X[i], 0.0f, walls_Z[i]);
		if(i>=2)wall[i]->RotateY(roatatebyRightAngle);
		wallXCoordinates[i] = wall[i]->GetX();
		wallZCoordinate[i] = wall[i]->GetZ();
	}
	//Water Tanks
	//(float)Used to check for collision
	waterTankRadius = 3.5f;
	for (int i = 0; i < waterTanksNum; i++)//Iteration for the water Tanks
	{
		waterTank[i] = waterTankMesh->CreateModel(waterTanksXCoordinates[i], 0.0f, waterTanksZCoordinates[i]);
		waterTanksX[i] = waterTank[i]->GetX();
		waterTanksZ[i] = waterTank[i]->GetZ();
		if (i == 0)//Rotates the water Tank specified
		{
			waterTank[i]->RotateX(waterTankXRotation); 
			waterTank[i]->SetY(waterTankYRotation);
		}
	}
	
}




