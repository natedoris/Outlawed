#pragma once

struct Players {
	uintptr_t name;
	unsigned int kill_streak;
	unsigned int nemesis_streak;
	unsigned int total_kills_against_enemy;
	unsigned int total_kills_against_player;
};
