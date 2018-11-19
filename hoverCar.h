//Ivaylo Ivanov
#pragma once
#include <iostream>//Used for debugging
#include <sstream>//Used for displaying text
#include "TL-Engine.h"
#include "Models.h"//Models header to access variables from it
#include "GameStates.h"//GameState header
#include "Collision.h"//Collision header
using namespace tle;
//Constants to initialises arrays and set up limit variables
const int wallNumber = 4;
const int checkPointNumber = 4;
const int waterTanksNumber = 3;
const float hoverCarStartY = 5.0f; 
const float hoverCarStartZ = -50.0f;
const int speedMinimum = 0;
const int minDecreaseSpeed = 3;
const double rounder = 0.5;
const float carRotationSpeed = 100.0f;
const float cameraSpeed = 20.0f;
const float cameraRotationLimit = 3.0f;
const float cameraMouseRotationSpeed = 30.0f;
const float moveCheckPointToLeg = 9.0f;
const int damageSpeedPoint = 10;
const float boostMaxBound = 3.0f;
const float boostMaximum = 40.0f;
const float boostWarning = 2.0f;
const float boostCoooler = 5.0f;
const int checkPointCollision = 8;
class hoverCar:public Collision//child of class Collision inheritng all functions from Collision and Game States
{
private:
public:
	friend class Environment;//Access private variables of class Environment
	//Meshes and Models used while game is active
	ICamera*myCamera;
	IMesh*hoverCarMesh;
	IModel*hoverCarModel;
	IFont* userInterfaceWriting;//Font used for text on the screen
	//Camera models
	IMesh*dummy;
	IModel*dummyCamera;
	IModel*dummyCameraThirdPerson;
	IModel*dummyCameraFirstPerson;

	//Constructor for my Engine
	hoverCar(I3DEngine* hoverCarEngine);
	I3DEngine*myEngine;//Initalises my Engine

	//FrameTime variable
	float frameTime;

	//Speed Variables
	float speedMeter;
	int speedMeterReader;
	float speedMaximum;
	float speedIncrease;
	float speedDecrease;
	
	//Hover car coordinates
	float hoverCarXCoordinate;
	float hoverCarOldXCoordinate;
	float hoverCarZCoordinate;
	float hoverCarOldZCoordinate;
	float hoverCarRadius;
	float objectRadius;
	
	

	//Count Down variables and Display strings
	float countDown;
	int stageDisplayTextX = 20;
	int stageDisplayTextY = 610;
	string stageDisplay = "";
	void hoverCar::StageDisplay();
	float speedMeterTextY = 630.0f;
	float speedMeterTextX = 20.0f;
	stringstream stageText;
	stringstream speedText;
	string countDownString;
	stringstream countDownText;
	void countDownToStart();
	float countDownTextX = 20.0f; 
	float countDownTextY = 670.0f;
	bool readyStart = false;//Used to track if countDown is completed

	//enum States for the walls
	Collision::boxSides wallCollision[wallNumber];

	//Structure used to calculate the movement of the hover car
	struct Vector2D;
	//Variables used to calculate car movement
	float carThrustFactor = 0.01f;
	float carDragCoefficient = -0.0009f;
	float matrix[4][4];
	float momentumReverse = 1.2f;

	//Function setting up the Car models, car Controls and cars Collison
	void createHoverCar();
	void carControls();
	bool carsCollision;

	//frameTime passed by reference
	void getFrameTime(float &frameTimeIn);

	//Tracks and changes game State
	void gameStateChange();

	//Camera set up variables and functions
	float ThridPersonCameraStartZ = -25.0f; 
	float ThirdPersonCameraStartY = 10.0f;
	float firstPersonCameraStartZ = 2.1f; 
	float firstPersonCameraY = 4.8f;
	float dummyCameraStartingZ = -30.0f;
	float cameraLimitLeftRight;
	float cameraLimitUpDown;
	void cameraControl();//Sets up camera controls
	float getMouseYCoordinate;
	float getMouseXCoordinate;

	//CheckPoint variables
	bool collisionCheckPoint[checkPointCollision];
	float checkPointXCoordinates[checkPointNumber];
	float checkPointZCoordinates[checkPointNumber];
	float checkPointWidth;
	float checkPointDepth;

	//Function for getting the checkPoint coordinates
	void getCheckPointCoordinates(float objectXCoordinateIn[checkPointNumber], float objectZCoordinateIn[checkPointNumber], 
								  float checkPointRadiusIn,float checkPointWidthIn,float checkPointDepthIn);

	float checkPointLeftLegX[checkPointNumber];
	float checkPointLeftLegZ[checkPointNumber];
	float checkPointRightLegX[checkPointNumber];
	float checkPointRightLegZ[checkPointNumber];
	float moveToLegXAngle = 4.5f;
	float moveToLegZAngle = 7.5f;

	//Wall coordinates and functions
	float wallDepth;
	float wallWidth;
	float wallXCoordinates[wallNumber];
	float wallZCoordinate[wallNumber];
	void getWallCoordinates(float wallDepthIn, float wallWidthIn, 
							float wallXCoordinatesIn[wallNumber], float wallZCoordinateIn[wallNumber]);
	//Water tanks coordinates and functions
	float waterTanksXCollisionCoordinates[waterTanksNumber];
	float waterTanksZCollisionCoordinates[waterTanksNumber];
	float waterTankCollisionRadius;
	void getWaterCoordinates(float waterTankXCoordinatesIn[waterTanksNumber], 
							 float waterTankZCoordinatesIn[waterTanksNumber], float waterTanksRadiusIn);
	bool collisionWaterTanks[waterTanksNumber];
	//Main function that checks for collision
	void CheckForCollision();

	//Non Player Car coordinates and functions
	float npcXCoordinates;
	float npcZCoordinates;
	float npcRadius;
	void getNPCCoordinates(float npcXCoordinatesIn, float npcZCoordinatesIn, float npcRadiusIn);

	//Booster variables and functions
	bool boostEnabled;
	stringstream boostStatus;
	float boostCounter;
	int boostCounterReader;
	float boostMultiplier;
	void engineBoost();
	float boostStatusTextY = 650.0f; 
	float boostStatusTextX = 20.0f;
	float warningTextY = 670.0f;

	//Health variables
	//Could be used to initiate health warnings
	enum healthStatus {Critical,Low,Fair,Healthy};
	healthStatus healthLevel;
	//Actual variables used to track and display health Status 
	int healthCounter;
	stringstream healthDisplay;
	float HealthTextX = 140.0f;
	float HealthTextY = 630.0f;

	
};

