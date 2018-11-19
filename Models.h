//Ivaylo Ivanov
#pragma once
#include "TL-Engine.h"
#include "hoverCar.h"
using namespace tle;
const int checkPointNum = 4;
const int wallNum = 4;
const int isleNum = 8;
const int waterTanksNum = 3;

class Environment//Independent class 
{				//Creates all the stationary models and sets up the map
private:
	//Stationary Meshes and Models 
	IMesh*skyboxMesh;
	IModel*skybox;
	IMesh*groundMesh;
	IModel*ground;
	IMesh * checkPointMesh;
	IModel*checkPoint[checkPointNum];
	IMesh*isleMesh;
	IModel*isle[isleNum];
	IMesh*wallMesh;
	IModel*wall[wallNum];
	IMesh*waterTankMesh;
	IModel*waterTank[waterTanksNum];
	
	float skybox_Y;
	

	//Arrays with list of coordinates used to create the models at set location
	float checkPointsX[checkPointNum]{ 0.0f,0.0f,30.0f,130.0f };
	float checkPointsZ[checkPointNum]{ 0.0f,100.0f,200.0f,200.0f };

	float walls_X[wallNum]{ -10.5f,9.5f,89.5f,89.5f };
	float walls_Z[wallNum]{ 46.0f,46.0f,206.0f,195.0f };

	float isles_X[isleNum]{ 10.0f,-10.0f,10.0f,-10.0f,97.0f,83.0f,97.0f,83.0f };
	float isles_Z[isleNum]{ 40.0f,40.0f,53.0f,53.0f,195.0f,195.0f,206.0f,206.0f };

	float waterTanksXCoordinates[waterTanksNum]{ -5.0f,-10.0f,10.0f };
	float waterTanksZCoordinates[waterTanksNum]{ 130.0f,190.0f,160.0f };
	//(floats)Used to rotate the models after creating them
	float roatatebyRightAngle = 90.0f;
	float checkPointRotation = 60.0f;

	float waterTankXRotation = 20.0f; 
	float waterTankYRotation = -8.5f;

	I3DEngine*myEnginePassed;
public:
	//Checkpoints variables
	float checkPointLegsX[checkPointNum];
	float checkPointLegsZ[checkPointNum];
	float checkPointWidth;
	float checkPointDepth;
	float checkPointLegsRadius;
	//Wall variables
	float wallWidth;
	float wallDepth;
	float wallZCoordinate[wallNum];
	float wallXCoordinates[wallNum];
	//Water Tank variables
	float waterTanksX[waterTanksNum];
	float waterTanksZ[waterTanksNum];
	float waterTankRadius;
	//Functions used to create the model
	void GetMyEngine(I3DEngine*& myEngineIn);//Passes a reference of myEngine to the class
	void LoadMeshes();
	void CreateModels();
};