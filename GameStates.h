//Ivaylo Ivanov
#include "TL-Engine.h"
using namespace tle;
#ifndef GAMESTATES_H
#define GAMESTATES_H
class gameStatesDecision//Master class 
{
public:
	//(enum)Game State used to define the game states
	enum GameStates { StartState, CheckPointStateOne, CheckPointStateTwo,CheckPointStateThree, EndState, noState };
	GameStates currentState = noState;//variable that tracks the actual state
	
};
#endif // !GAMESTATES_H




