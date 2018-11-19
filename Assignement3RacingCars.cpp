// Assignement3RacingCars.cpp: A program using the TL-Engine
//Ivaylo Ivanov
#include <TL-Engine.h>	// TL-Engine include file and namespace
#include "Models.h"
#include "hoverCar.h"
#include "Display.h"
#include "AIhoverCar.h"
using namespace tle;

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder( ".\\media" );

	/**** Set up your scene here ****/
	EKeyCode Close(Key_Escape);
	/*--------------------------------------Object Creation------------------------------------*/
	Environment LoadInModels;
	hoverCar CreateRaceCar=hoverCar::hoverCar(myEngine);
	AIhoverCar CreateNonPlayer=AIhoverCar::AIhoverCar(myEngine);
	UserInterfaceDisplay ScreenItems;
	/*--------------------------------Set up scene modesl creation and variable initialisation----------------------------*/
	//Setting up Scene
	ScreenItems.GetMyEngine(myEngine);
	LoadInModels.GetMyEngine(myEngine);
	LoadInModels.LoadMeshes();
	LoadInModels.CreateModels();
	ScreenItems.CreateBackgroundInterface();//Creating background sprite
	//Setting up AIcar
	CreateNonPlayer.CreateNonPlayerCar();
	CreateNonPlayer.CreateAITargets();
	//Setting up hoverCar
	CreateRaceCar.createHoverCar();
	//Passing all object coordinates fron Environment class to hoverCar class
	CreateRaceCar.getCheckPointCoordinates(LoadInModels.checkPointLegsX, LoadInModels.checkPointLegsZ,LoadInModels.checkPointLegsRadius,
											LoadInModels.checkPointWidth,LoadInModels.checkPointDepth);
	CreateRaceCar.getWallCoordinates(LoadInModels.wallDepth, LoadInModels.wallWidth, LoadInModels.wallXCoordinates, LoadInModels.wallZCoordinate);

	CreateRaceCar.getWaterCoordinates(LoadInModels.waterTanksX, LoadInModels.waterTanksZ, LoadInModels.waterTankRadius);
	/*-------------------------------Vriables------------------------------------------------*/
	float frameTime = myEngine->Timer();
	
	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/
		frameTime = myEngine->Timer();

		/*----Movement varaibles displayed on screen----*/
		CreateRaceCar.StageDisplay();
		
		/*------------------------------------------Hover car Controls---------------------------*/
		CreateRaceCar.getFrameTime(frameTime);
		CreateNonPlayer.getFrameTime(frameTime);
		CreateRaceCar.gameStateChange();
		CreateRaceCar.carControls();
		CreateRaceCar.cameraControl();
		/*----------------AIcar Movement set up------*/
		CreateNonPlayer.getGameStatus(CreateRaceCar.readyStart);
		CreateNonPlayer.getNonPlayerCarCoordinates();
		CreateNonPlayer.moveAIToTarget();
		/*---------Passing AI car coordinates from AIhoverCar class to hoverCar class--------*/
		CreateRaceCar.getNPCCoordinates(CreateNonPlayer.nonPlayerXCoordinates,CreateNonPlayer.nonPlayerZCoordinates,CreateNonPlayer.nonPlayerCarRadius);
		/*------------------------------------------Collision-----------------------------------*/
		
		CreateRaceCar.CheckForCollision();

		/*----------------------------------------Close engine---------------------------------*/
		if (myEngine->KeyHit(Close)) return;
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
