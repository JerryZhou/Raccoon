#ifndef _GAMESTATE_H_
#define _GAMESTATE_H_
//by eagle

//-----------------------------------------------------------------

namespace GameState
{
	enum ActorType 
	{
		Ground,
		Wall,
		Coin,
		Pig,
		RedBird,
		BlackBird,
	};

	enum MenuStartState
	{
		LeftState,
		RightState,
		HasClickedState,
		IdleState,
	};

	enum PigState
	{
		Visible,
		Invisible,
	};

	enum CoinState
	{
		VisibleCoin,
		InvisibleCoin,
	};
}

#endif