//Ivaylo Ivanov
#pragma once
#include "TL-Engine.h"
using namespace tle;

class UserInterfaceDisplay//This is an independet class that creates all the sprites 
{
private:

	I3DEngine * myEngine;
	//Sprite Definition
	ISprite*interfaceBack;
	//Spite Creation Coordinates
	float SpirteX = -110.0f; 
	float SpriteY = 600.0f;
public:
	//Setter used to define 'myEngine' in the class
	void GetMyEngine(I3DEngine*&DisplayEngine);
	void CreateBackgroundInterface();
};