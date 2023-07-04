#pragma once

namespace Player
{
	char*	Name();
	void	ChangeName(const char* new_name);
	float	CurrentHeight();
	float	MaxHeight();
	void	ResetHeight();

	struct Player {
		char* name;
		unsigned int kill_streak;
		unsigned int nemesis_streak;
		unsigned int total_kills_against_enemy;
		unsigned int total_kills_against_player;
	};
}