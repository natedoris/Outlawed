#pragma once
#include "IDrawFont.h"

class ScoreBoard : virtual IDrawFont
{
	public:
		ScoreBoard();

		
	private:
		ID3DXFont* player_name;
		ID3DXFont* stats;
};