//Ivaylo Ivanov
#include "Display.h"


void UserInterfaceDisplay::GetMyEngine(I3DEngine*&DisplayEngine)//Reference of myEngine passed in
{
	myEngine = DisplayEngine;
}
//Creates the wood UI sprite
void UserInterfaceDisplay::CreateBackgroundInterface()
{
	myEngine->AddMediaFolder(".\\media");
	interfaceBack = myEngine->CreateSprite("wood.jpg", SpirteX, SpriteY); 
	

}