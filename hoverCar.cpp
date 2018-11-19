#include "hoverCar.h"
//Ivaylo Ivanov

hoverCar::hoverCar(I3DEngine* hoverCarEngine)//Construct that creates myEngine for the hoverCar
{
	myEngine = hoverCarEngine;
}

void hoverCar::createHoverCar()
{
	//Loads in all the meshes
	myEngine->AddMediaFolder(".\\media");
	hoverCarMesh = myEngine->LoadMesh("race2.x");
	hoverCarModel = hoverCarMesh->CreateModel(hoverCarStartY, 0.0f, hoverCarStartZ); 
	dummy = myEngine->LoadMesh("Dummy.x");
	//Creates the Camera and dummy Camera Model 
	myCamera = myEngine->CreateCamera(kManual,0.0f,0.0f,0.0f);
	dummyCamera = dummy->CreateModel();
	dummyCamera->SetZ(-dummyCameraStartingZ); 
	//Creates dummy models which are used as position to change camera angle
	dummyCameraThirdPerson = dummy->CreateModel();
	dummyCameraThirdPerson->SetZ(ThridPersonCameraStartZ); 
	dummyCameraThirdPerson->SetY(ThirdPersonCameraStartY);
	dummyCameraFirstPerson = dummy->CreateModel();
	dummyCameraFirstPerson->SetZ(firstPersonCameraStartZ); 
	dummyCameraFirstPerson->SetY(firstPersonCameraY);
	userInterfaceWriting = myEngine->LoadFont("Font1.bmp");
	//Resets the camera focus and starts mouse capture
	myCamera->ResetOrientation();
	myEngine->StartMouseCapture();
	
	/*---Variables---*/
	//Initialising variables that change throughout the game 
	cameraLimitLeftRight = 0.0f;
	cameraLimitUpDown = 0.0f;
	countDown = 3.0f;
	speedMeter = 0;
	hoverCarRadius = 3.0f;
	boostCounter = 0.0f;
	boostEnabled = true;
	boostMultiplier = 0.5f;
	speedMaximum = 20.0f;
	speedIncrease = 10.0f;
	speedDecrease = 20.0f;
	healthCounter = 100;
}
/*---Controls for the car---*/
/*---Key Controls---*/
EKeyCode IncreaseSpeed(Key_W);
EKeyCode DecreaseSpeed(Key_S);
EKeyCode turnLeft(Key_A);
EKeyCode turnRight(Key_D);
EKeyCode hitStart(Key_Space);
EKeyCode Up(Key_Up);
EKeyCode Down(Key_Down);
EKeyCode Left(Key_Left);
EKeyCode Right(Key_Right);
EKeyCode ThirdPerson(Key_1);
EKeyCode FirstPerson(Key_2);

/*-------------------Controling the Car----------------------*/
struct hoverCar::Vector2D
{
	float matrixCoordinateX;
	float matrixCoordinateZ;
};

hoverCar::Vector2D momentum{ 0.0f,0.0f };//Main force to calculate car movement
hoverCar::Vector2D thrust{ 0.0f,0.0f };//Aplies burst when the car turns
hoverCar::Vector2D drag{ 0.0f,0.0f };//Acts as a gravity which slows the car down

void hoverCar::getFrameTime(float &frameTimeIn)//Passes frameTime by reference
{
	frameTime = frameTimeIn;
}

void hoverCar::gameStateChange()//Controls the states of the game
{
	//Checks against what the current state is 
	if (currentState == noState)
	{
		
		if (myEngine->KeyHit(hitStart))
		{
			currentState = StartState;
		}
	}
	//CheckPointPass is used to check if the hover car has passed through the checkpoint with the set coordinates
	//States control which checkPoint you have to pass through so you can move to the next 
	//Upon passing through as checkPoint the game Status changes 
	if (currentState == StartState)
	{
		Collision::boxSides checkPointPass= Collision::sphereToBoxCollision(hoverCarModel->GetX(), hoverCarModel->GetZ(), hoverCarRadius,hoverCarXCoordinate,hoverCarZCoordinate, 
																			checkPointXCoordinates[0], checkPointZCoordinates[0], checkPointWidth, checkPointDepth);
		if (checkPointPass==frontSide)currentState = CheckPointStateOne;
	}
	if (currentState == CheckPointStateOne)
	{
		Collision::boxSides checkPointPass = Collision::sphereToBoxCollision(hoverCarModel->GetX(), hoverCarModel->GetZ(), hoverCarRadius, hoverCarXCoordinate, hoverCarZCoordinate, 
																			 checkPointXCoordinates[1], checkPointZCoordinates[1], checkPointWidth, checkPointDepth);
		if (checkPointPass==frontSide)currentState = CheckPointStateTwo;
	}
	if (currentState == CheckPointStateTwo)
	{
		Collision::boxSides checkPointPass = Collision::sphereToBoxCollision(hoverCarModel->GetX(), hoverCarModel->GetZ(), hoverCarRadius, hoverCarXCoordinate, hoverCarZCoordinate, 
																			checkPointXCoordinates[2], checkPointZCoordinates[2], checkPointWidth, checkPointDepth);
		if (checkPointPass == frontSide || checkPointPass == leftSide)currentState = CheckPointStateThree;
	}
	if (currentState == CheckPointStateThree)
	{
		Collision::boxSides checkPointPass = Collision::sphereToBoxCollision(hoverCarModel->GetX(), hoverCarModel->GetZ(), hoverCarRadius, hoverCarXCoordinate, hoverCarZCoordinate, 
																			checkPointXCoordinates[3], checkPointZCoordinates[3], checkPointDepth, checkPointWidth);
		if (checkPointPass == leftSide)currentState = EndState;
	}
	if (currentState == EndState)
	{//If the player has reached the last checkpoint stop car movement
		if (speedMeter > speedMinimum) speedMeter -= speedIncrease * frameTime;
		thrust = { 0.0f,0.0f };
	}
}

hoverCar::Vector2D Scalar(float scalarMultiplier, hoverCar::Vector2D currentVector)
{//Used to calculate the drag for the car movement
	return{ scalarMultiplier*currentVector.matrixCoordinateX,
			scalarMultiplier*currentVector.matrixCoordinateZ };
}

hoverCar::Vector2D vectorCalculations(hoverCar::Vector2D vectorOne, hoverCar::Vector2D vectorTwo, hoverCar::Vector2D vectorThree)
{
	return{ vectorOne.matrixCoordinateX + vectorTwo.matrixCoordinateX + vectorThree.matrixCoordinateX,
			vectorOne.matrixCoordinateZ + vectorTwo.matrixCoordinateZ + vectorThree.matrixCoordinateZ };
}//Calculates the momentum based on the three factors (Old momemntum, thrust, drag) 

void hoverCar::StageDisplay()
{
	//Creates the text that shows the current GameState
	stageText << "Game State: " << stageDisplay;
	userInterfaceWriting->Draw(stageText.str(), stageDisplayTextX, stageDisplayTextY,kWhite);
	stageText.str("");
	if (currentState == noState)stageDisplay = "Hit 'Space' to Start";
	else if (currentState == StartState)stageDisplay = "Stage 1";
	else if (currentState == CheckPointStateOne)stageDisplay = "Stage 2";
	else if (currentState == CheckPointStateTwo)stageDisplay = "Stage 3";
	else if (currentState == CheckPointStateThree)stageDisplay = "Stage 4";
	else if (currentState == EndState)stageDisplay = "Race Complete";

	//Creates the Speed Meter Reader
	//Rounds up the number to a whole number
	if (speedMeter >= speedMinimum) speedMeterReader = (int)(speedMeter + rounder);
	else speedMeterReader = (int)(speedMeter - rounder);
	//Displays actual speed 
	speedText << "Speed: " << speedMeterReader;
	userInterfaceWriting->Draw(speedText.str(), speedMeterTextX, speedMeterTextY, kWhite);
	speedText.str("");
	
	//Displays the health counter
	healthDisplay << "Health: " << healthCounter;
	userInterfaceWriting->Draw(healthDisplay.str(), HealthTextX, HealthTextY, kWhite);
	healthDisplay.str("");
}

void hoverCar::carControls()
{
	
	//Attaches the camera to parent models
	myCamera->AttachToParent(dummyCameraThirdPerson);
	dummyCamera->AttachToParent(hoverCarModel);
	dummyCameraThirdPerson->AttachToParent(hoverCarModel);
	dummyCameraFirstPerson->AttachToParent(hoverCarModel);
	//Gets the facing vector 
	hoverCarModel->GetMatrix(&matrix[0][0]);
	hoverCar::Vector2D facingVector = { matrix[2][0],matrix[2][2] };

	//Constatnly updates get the hover car position used for collision
	hoverCarXCoordinate = hoverCarModel->GetX();
	hoverCarZCoordinate = hoverCarModel->GetZ();

	engineBoost();//Creaates the Engine boost

	if (currentState == StartState) hoverCar::countDownToStart();//Initiates the countDown

	//Allows the car momvemnt as long as its not it noState or endState
	if (currentState == StartState && readyStart || currentState == CheckPointStateOne 
		|| currentState == CheckPointStateTwo || currentState == CheckPointStateThree)
	{

		if (myEngine->KeyHeld(IncreaseSpeed))
		{
			if(speedMeter < speedMaximum)speedMeter += speedIncrease*frameTime;//Increases speed based on frameTime
			thrust = Scalar(carThrustFactor*speedMeter, facingVector);//Calcualtes the thrust 

		}
		else if (myEngine->KeyHeld(DecreaseSpeed))
		{	
			if (speedMeter > minDecreaseSpeed) speedMeter -= speedDecrease * frameTime;//Decreases speed to minimum backwards speed
			if (speedMeter < minDecreaseSpeed)speedMeter  += speedIncrease * frameTime;//If speed is smaller then minimum backwards speed increases it 
			thrust = Scalar(-(carThrustFactor*speedMeter), facingVector);//Dercreases the thrust
		}
		else//Slows down the car until it stops when no key held
		{
			thrust = { 0.0f,0.0f };
			if (speedMeter > speedMinimum)speedMeter -= speedDecrease*frameTime;
		}
		//Turn left and right controls roatates around Y axis with speed based on frameTime
		if (myEngine->KeyHeld(turnLeft))
		{
			hoverCarModel->RotateY(-carRotationSpeed *frameTime);

		}

		if (myEngine->KeyHeld(turnRight))
		{
			hoverCarModel->RotateY(carRotationSpeed*frameTime);

		}
	}

	drag = Scalar(carDragCoefficient, momentum);//Calculates drag using (dragCoefficient and old momentum)
	momentum = vectorCalculations(momentum, thrust, drag);//Calculates the momentum based on thrust drag momentum
	hoverCarModel->Move(momentum.matrixCoordinateX*frameTime, 0.0f, momentum.matrixCoordinateZ*frameTime);//Moves the car along X and Y axis based on the momentum
	
}

void hoverCar::countDownToStart()
{//When countdown initiates 
	countDown -= frameTime;//Counts down from 3 to 1 by taking away frameTime
	
	//Displays text according how many seconds have passed
	if (countDown < 3.0f && countDown > 2.0f)
	{
		countDownText << "3";
		userInterfaceWriting->Draw(countDownText.str(), countDownTextX, countDownTextY, kWhite); 
		countDownText.str("");

	}
	if (countDown < 2.0f && countDown > 1.0f)
	{
		countDownText<<"2";
		userInterfaceWriting->Draw(countDownText.str(), countDownTextX, countDownTextY, kWhite);
		countDownText.str("");
	}
	if (countDown < 1.0f && countDown > 0.0f)
	{
		countDownText << "1";
		userInterfaceWriting->Draw(countDownText.str(), countDownTextX, countDownTextY, kWhite);
		countDownText.str("");

	}
	if (countDown < 0.0f && countDown > -1.0f)
	{
		countDownText << "Go!";
		userInterfaceWriting->Draw(countDownText.str(), countDownTextX, countDownTextY, kWhite);
		countDownText.str("");
		readyStart = true;//After the counter is done starts the game
	}

}

void hoverCar::cameraControl()
{
	if (readyStart)
	{
		//Moves the camera Up, Down, Left and Right with speed based on frameTime
		if (myEngine->KeyHeld(Up))
		{
			myCamera->MoveZ(cameraSpeed*frameTime);
		}
		if (myEngine->KeyHeld(Down))
		{
			myCamera->MoveZ(-(cameraSpeed*frameTime));
		}
		if (myEngine->KeyHeld(Left))
		{
			myCamera->MoveX(-(cameraSpeed*frameTime));
		}
		if (myEngine->KeyHeld(Right))
		{
			myCamera->MoveX(cameraSpeed*frameTime);
		}
		if (myEngine->KeyHit(ThirdPerson))//Resets the camera focus and sets it to Third Person Angle
		{
			myCamera->SetZ(dummyCameraThirdPerson->GetZ());
			myCamera->SetX(dummyCameraThirdPerson->GetX());
			myCamera->SetY(dummyCameraThirdPerson->GetY());
			myCamera->ResetOrientation();
			cameraLimitUpDown = 0.0f;
			cameraLimitLeftRight = 0.0f;
		}
		if (myEngine->KeyHit(FirstPerson))//Resets the camera focus and sets it to First Person Angle
		{
			myCamera->SetZ(dummyCameraFirstPerson->GetZ());
			myCamera->SetX(dummyCameraFirstPerson->GetX());
			myCamera->SetY(dummyCameraFirstPerson->GetY());
			myCamera->ResetOrientation();
			cameraLimitUpDown = 0.0f;
			cameraLimitLeftRight = 0.0f;
		}
		//Constantly gets the mouse movement to be able to determine which way to rotate the camera
		int getMouseMovementX = myEngine->GetMouseMovementX(); 
		int getMouseMovementY = myEngine->GetMouseMovementY();

		//Roatates the camera based on Mouse Movement and with set Limiters
		//Limit is calculated by mouseMovement*frameTime so it knows the camera moves in the certain direction
		//The two variables cameraUpDown and LeftRight track the camera position
		if (myEngine->GetMouseMovementX() > getMouseMovementX && cameraLimitLeftRight < cameraRotationLimit)
		{

			myCamera->RotateY(getMouseMovementX*cameraMouseRotationSpeed*frameTime);
			cameraLimitLeftRight -= frameTime*getMouseMovementX;
		}

		if (myEngine->GetMouseMovementX() < getMouseMovementX && cameraLimitLeftRight > -cameraRotationLimit)
		{
			myCamera->RotateY(getMouseMovementX*cameraMouseRotationSpeed*frameTime);
			cameraLimitLeftRight -= frameTime*getMouseMovementX;

		}
		//Rotating around the local axis to prevent the camera from turning upside down
		if (myEngine->GetMouseMovementY() > getMouseMovementY && cameraLimitUpDown < cameraRotationLimit)
		{
			myCamera->RotateLocalX(getMouseMovementY*cameraMouseRotationSpeed*frameTime);
			cameraLimitUpDown -= frameTime*getMouseMovementY;

		}

		if (myEngine->GetMouseMovementY() < getMouseMovementY && cameraLimitUpDown > -cameraRotationLimit)
		{
			myCamera->RotateLocalX(getMouseMovementY*cameraMouseRotationSpeed*frameTime);
			cameraLimitUpDown -= frameTime*getMouseMovementY;
		}
	}
	
}

void hoverCar::getCheckPointCoordinates(float objectXCoordinateIn[checkPointNumber], float objectZCoordinateIn[checkPointNumber], float checkPointRadiusIn, float checkPointWidthIn, float checkPointDepthIn)
{
	//Gets all the checkPoint coordinates
	for (int i=0; i < checkPointNumber; i++)
	{
		checkPointXCoordinates[i] = objectXCoordinateIn[i];
		checkPointZCoordinates[i] = objectZCoordinateIn[i];
	}
	//Changes the coordinate focus to the legs of the checkpoint
	checkPointLeftLegX[0] = checkPointXCoordinates[0] - moveCheckPointToLeg;
	checkPointLeftLegZ[0] = checkPointZCoordinates[0];
	checkPointRightLegX[0] = checkPointXCoordinates[0] + moveCheckPointToLeg;
	checkPointRightLegZ[0] = checkPointZCoordinates[0];
	
	checkPointLeftLegX[1] = checkPointXCoordinates[1] - moveCheckPointToLeg;
	checkPointLeftLegZ[1] = checkPointZCoordinates[1];
	checkPointRightLegX[1] = checkPointXCoordinates[1] + moveCheckPointToLeg;
	checkPointRightLegZ[1] = checkPointZCoordinates[1];
	//If checkpoint is rotated by single angle it will need to move to the leg based on rotation
	checkPointLeftLegX[2] = checkPointXCoordinates[2] - moveToLegXAngle;
	checkPointLeftLegZ[2] = checkPointZCoordinates[2] + moveToLegZAngle;
	checkPointRightLegX[2] = checkPointXCoordinates[2] + moveToLegXAngle; 
	checkPointRightLegZ[2] = checkPointZCoordinates[2] - moveToLegZAngle; 

	checkPointLeftLegX[3] = checkPointXCoordinates[3];
	checkPointLeftLegZ[3] = checkPointZCoordinates[3] - moveCheckPointToLeg;
	checkPointRightLegX[3] = checkPointXCoordinates[3];
	checkPointRightLegZ[3] = checkPointZCoordinates[3] + moveCheckPointToLeg; 
	//Passes the rest of variables needed for collision
	objectRadius = checkPointRadiusIn;
	checkPointDepth = checkPointDepthIn;
	checkPointWidth = checkPointWidthIn;
}

void hoverCar::getWaterCoordinates(float waterTankXCoordinatesIn[waterTanksNumber], float waterTankZCoordinatesIn[waterTanksNumber], float waterTanksRadiusIn)
{//Passes the water tanks coordinates for collision
	for (int i = 0; i < waterTanksNum; i++)
	{
		waterTanksXCollisionCoordinates[i] = waterTankXCoordinatesIn[i];
		waterTanksZCollisionCoordinates[i] = waterTankZCoordinatesIn[i];
	}
	waterTankCollisionRadius=waterTanksRadiusIn;
}

void hoverCar::getWallCoordinates(float wallDepthIn, float wallWidthIn, float wallXCoordinatesIn[wallNumber], float wallZCoordinateIn[wallNumber])
{//Passes the wall coordinates for collision
	wallDepth = wallDepthIn;
	wallWidth = wallWidthIn;
	for (int i = 0; i < wallNumber; i++)
	{
		wallXCoordinates[i] = wallXCoordinatesIn[i];
		wallZCoordinate[i] = wallZCoordinateIn[i];
	}
}

void hoverCar::CheckForCollision()
{
	//CheckPoints Collision
	//Boolean array for CheckPoint to check for collision

	//The base of sphere to sphere 2D collision is passing both object X and Z coordinates including the radius 

	/*The base of sphere to box 2D collision is passing the sphere object X, Z, oldX, oldZ and raidus with the 
	box object X and Z coordinates including the width and depth*/
	 collisionCheckPoint[0] = hoverCar::sphereToSphereCollision(hoverCarModel->GetX(), hoverCarModel->GetZ(), hoverCarRadius, 
		 checkPointLeftLegX[0], checkPointLeftLegZ[0], objectRadius);

	 collisionCheckPoint[1] = hoverCar::sphereToSphereCollision(hoverCarModel->GetX(), hoverCarModel->GetZ(), hoverCarRadius, 
		 checkPointRightLegX[0], checkPointRightLegZ[0], objectRadius);

	 collisionCheckPoint[2] = hoverCar::sphereToSphereCollision(hoverCarModel->GetX(), hoverCarModel->GetZ(), hoverCarRadius, 
		 checkPointLeftLegX[1], checkPointLeftLegZ[1], objectRadius);

	 collisionCheckPoint[3] = hoverCar::sphereToSphereCollision(hoverCarModel->GetX(), hoverCarModel->GetZ(), hoverCarRadius, 
		 checkPointRightLegX[1], checkPointRightLegZ[1], objectRadius);

	 collisionCheckPoint[4] = hoverCar::sphereToSphereCollision(hoverCarModel->GetX(), hoverCarModel->GetZ(), hoverCarRadius, 
		 checkPointLeftLegX[2], checkPointLeftLegZ[2], objectRadius);

	 collisionCheckPoint[5] = hoverCar::sphereToSphereCollision(hoverCarModel->GetX(), hoverCarModel->GetZ(), hoverCarRadius, 
		 checkPointRightLegX[2], checkPointRightLegZ[2], objectRadius);

	 collisionCheckPoint[6] = hoverCar::sphereToSphereCollision(hoverCarModel->GetX(), hoverCarModel->GetZ(), hoverCarRadius, 
		 checkPointLeftLegX[3] , checkPointLeftLegZ[3], objectRadius);

	 collisionCheckPoint[7] = hoverCar::sphereToSphereCollision(hoverCarModel->GetX(), hoverCarModel->GetZ(), hoverCarRadius, 
		 checkPointRightLegX[3] , checkPointRightLegZ[3], objectRadius);

	 //Resolves the collision for each model is the same
	 for (int i = 0; i < checkPointCollision; i++)
	 {
		 if (collisionCheckPoint[i]==true)
		 {
			 hoverCarModel->SetPosition(hoverCarXCoordinate, 0.0f, hoverCarZCoordinate);
			 if (speedMeter > damageSpeedPoint) healthCounter -= 1;//If there is collision wit speed above (int)damageSpeed take away health
			 if (speedMeter > speedMinimum) speedMeter = 0;//Reduce speed 
			 //Sets the momentum to the opposite so it makes the car bounce back
			 momentum.matrixCoordinateX /= -momentumReverse; 
			 momentum.matrixCoordinateZ /= -momentumReverse;
		 }
	 }

	//Walls Collision
	//Enum state for each wall
	
	wallCollision[0] = Collision::sphereToBoxCollision(hoverCarModel->GetX(), hoverCarModel->GetZ(), hoverCarRadius, hoverCarXCoordinate, hoverCarZCoordinate,
														wallXCoordinates[0], wallZCoordinate[0], wallWidth, wallDepth);
	wallCollision[1] = Collision::sphereToBoxCollision(hoverCarModel->GetX(), hoverCarModel->GetZ(), hoverCarRadius, hoverCarXCoordinate, hoverCarZCoordinate,
														wallXCoordinates[1], wallZCoordinate[1], wallWidth, wallDepth);
	wallCollision[2] = Collision::sphereToBoxCollision(hoverCarModel->GetX(), hoverCarModel->GetZ(), hoverCarRadius, hoverCarXCoordinate, hoverCarZCoordinate,
														wallXCoordinates[2], wallZCoordinate[2], wallDepth, wallWidth);
	wallCollision[3] = Collision::sphereToBoxCollision(hoverCarModel->GetX(), hoverCarModel->GetZ(), hoverCarRadius, hoverCarXCoordinate, hoverCarZCoordinate,
														wallXCoordinates[3], wallZCoordinate[3], wallDepth, wallWidth);

	for (int i = 0; i < wallNumber; i++)
	{
		if (wallCollision[i] == frontSide || wallCollision[i] == backSide || wallCollision[i] == rightSide || wallCollision[i] == leftSide)
		{
			hoverCarModel->SetPosition(hoverCarXCoordinate, 0.0f, hoverCarZCoordinate);
			if (speedMeter>damageSpeedPoint) healthCounter -= 1;
			if (speedMeter > speedMinimum) speedMeter = 0;
			momentum.matrixCoordinateX /= -momentumReverse;
			momentum.matrixCoordinateZ /= -momentumReverse;
			thrust = { 0.0f,0.0f };
		}
	}
	

	//Water Tanks Collision
	//Boolean array for waterTanks
	collisionWaterTanks[0] = hoverCar::sphereToSphereCollision(hoverCarModel->GetX(), hoverCarModel->GetZ(), hoverCarRadius, 
		waterTanksXCollisionCoordinates[0], waterTanksZCollisionCoordinates[0], waterTankCollisionRadius);
	collisionWaterTanks[1] = hoverCar::sphereToSphereCollision(hoverCarModel->GetX(), hoverCarModel->GetZ(), hoverCarRadius, 
		waterTanksXCollisionCoordinates[1], waterTanksZCollisionCoordinates[1], waterTankCollisionRadius);
	collisionWaterTanks[2] = hoverCar::sphereToSphereCollision(hoverCarModel->GetX(), hoverCarModel->GetZ(), hoverCarRadius, 
		waterTanksXCollisionCoordinates[2], waterTanksZCollisionCoordinates[2], waterTankCollisionRadius);

	for (int i = 0; i<waterTanksNumber; i++)
	{
		if (collisionWaterTanks[i] == true)
		{
			hoverCarModel->SetPosition(hoverCarXCoordinate, 0.0f, hoverCarZCoordinate);
			if (speedMeter>damageSpeedPoint) healthCounter -= 1;
			if (speedMeter > speedMinimum) speedMeter = 0;
			momentum.matrixCoordinateX /= -momentumReverse;
			momentum.matrixCoordinateZ /= -momentumReverse;
		}
	}
	//Car To Car Collision
	////Boolean variable for AIcar Collision
	carsCollision= hoverCar::sphereToSphereCollision(hoverCarModel->GetX(), hoverCarModel->GetZ(), hoverCarRadius, npcXCoordinates, npcZCoordinates, npcRadius);

	if (carsCollision)
	{
		hoverCarModel->SetPosition(hoverCarXCoordinate, 0.0f, hoverCarZCoordinate);
		if (speedMeter>damageSpeedPoint) healthCounter -= 1;
		if (speedMeter > speedMinimum) speedMeter = 0;
		momentum.matrixCoordinateX /= -momentumReverse;
		momentum.matrixCoordinateZ /= -momentumReverse;
		
	}

}

void hoverCar::engineBoost()
{
	if (boostEnabled && (currentState!=noState|| currentState != EndState) && readyStart)
	{
		if (myEngine->KeyHeld(hitStart))
		{
			boostCounter += frameTime;//Used to follow for how long boost is held 
			if (boostCounter > boostMaxBound)//Check for boost overheating
			{//Disabled boost and sets a cool down for 5 seconds (int)boostCooler
				boostEnabled = false;
				boostCounter = boostCoooler; 
				boostStatus.str("");
			}

			if (speedMaximum < boostMaximum)//If the max speed is below the boost max
			{
				speedMaximum += speedMaximum * boostMultiplier*frameTime;//Increases the max speed by (float)boostMultiplier
				speedIncrease += speedIncrease * boostMultiplier*frameTime;//Increases the speed increase rate by (float)boostMultiplier
			}
		}
		else//Resets the speed to normal when the boost isn't activated
		{
			speedMaximum = 20.0f;
			speedIncrease = 10.0f;
			if(boostCounter>0)boostCounter -= frameTime;
			if (speedMeter>speedMaximum)speedMeter -= speedIncrease * frameTime;
		}

		if (boostCounter >= 0) boostCounterReader = (int)(boostCounter + rounder);
		else boostCounterReader = (int)(boostCounter - rounder);//Round up the counter so it's displayed in whole numbers
		//Displays boost Status
		boostStatus << "BoostEngineStatus: " << boostCounterReader<<"s";
		userInterfaceWriting->Draw(boostStatus.str(), boostStatusTextX, boostStatusTextY, kWhite);
		boostStatus.str("");
		if (boostCounter > boostWarning && boostCounter < boostMaxBound)
		{//Shows warning before the boost overheats
			boostStatus << "Warning! 1 second before overheat!";
			userInterfaceWriting->Draw(boostStatus.str(), boostStatusTextX, warningTextY, kWhite); 
			boostStatus.str("");
		}
	}
	if (boostEnabled==false)//If boosts overheats
	{
		//Sets speed to normal
		speedMaximum = 20.0f;
		speedIncrease = 10.0f;
		if(speedMeter>speedMaximum)speedMeter -= speedIncrease * frameTime;
		boostCounter -= frameTime;//Starts boost cool down count
		if (boostCounter >= 0) boostCounterReader = (int)(boostCounter + rounder);
		else boostCounterReader = (int)(boostCounter - rounder);//Round up the counter so it's displayed in whole numbers
		//Displays Cool down count
		boostStatus << "Cooling boost: " << boostCounterReader << "s";
		userInterfaceWriting->Draw(boostStatus.str(), boostStatusTextX, boostStatusTextY, kWhite);
		boostStatus.str("");
		if (boostCounter <= 0)boostEnabled = true;//Enables boost ones cool down coutner is finished
	}
	

	
}

void hoverCar::getNPCCoordinates(float npcXCoordinatesIn, float npcZCoordinatesIn, float npcRadiusIn)
{//Constantly gets NonPlayerCar coordinates for collision 
	npcXCoordinates = npcXCoordinatesIn;
	npcZCoordinates = npcZCoordinatesIn;
	npcRadius = npcRadiusIn;
}