//Ivaylo Ivanov
#pragma once
#include <iostream>
#include <sstream>
#include "TL-Engine.h"
#include "Models.h"
#include "GameStates.h"
#include "Collision.h"
#include "hoverCar.h"
using namespace tle;
const float AICarSpeed = 30.0f;
const int numberOfTargets = 13;
class AIhoverCar : public Collision
{
private:
public:

	void AIhoverCar::getFrameTime(float &frameTimeIn);
	float frameTime;

	AIhoverCar::AIhoverCar(I3DEngine*myEngineIn);
	void CreateNonPlayerCar();

	IMesh * nonPlayerCarMesh;
	IModel*nonPlayerCarModel;
	float nonPlayerCarRadius;
	float nonPlayerXCoordinates;
	float nonPlayerZCoordinates;
	void getNonPlayerCarCoordinates();

	float dummyXCoordinates[numberOfTargets] = { 0.0f,  -5.0f, -7.5f,  -10.0f, -12.5f, -15.0f,  0.0f,   0.0f,   25.0f,   50.0f,  70.0f,  90.0f,  170.0f };
	float dummyZCoordinates[numberOfTargets] = {100.0f, 110.0f, 115.0f, 120.0f, 125.0f, 130.0f, 175.0f, 175.0f, 190.0f, 210.0f, 205.0f, 200.0f, 200.0f };
	IMesh*dummy;
	IModel*AICarTarget[numberOfTargets];
	float dummyTargetRadius;

	void CreateAITargets();
	void moveAIToTarget();

	I3DEngine * myEngine;
	enum dummyStatuses {Checked, Unchecked};
	dummyStatuses dummyTargetStatus[numberOfTargets];
	int targetHit;

	bool readyStart;
	void getGameStatus(bool readyIn);
};

