#include "../pch.h"
#include "battletext.h"
#include "../Game/player.h"

namespace BattleText
{
	struct {
		const char killed[8] = " killed";
		const char blew_up[9] = " blew up";
	} Actions;



	void UpdateSuicideStats(char* token, struct Stats* stats)
	{
		if (strcmp(token, "blew") == 0) {
			stats->deaths += 1;
			stats->suicides_dynamite += 1;
			stats->death_from_dynamite += 1;
			return;
		}
		else if (strcmp(token, "drowned!") == 0) {
			stats->deaths += 1;
			stats->suicides_drowned += 1;
			return;
		}
		else if (strcmp(token, "got") == 0) {
			stats->deaths += 1;
			stats->suicides_crushed += 1;
			return;
		}
		else if (strcmp(token, "fell") == 0) {
			stats->deaths += 1;
			stats->suicides_fell += 1;
			return;
		}
	}

	void UpdateKills(char* token, Stats* stats)
	{
		if (strcmp(token, "killed") == 0) {
			stats->kills += 1;
		}
		else if (strcmp(token, "blew") == 0) {
			stats->kills += 1;
			stats->kills_from_dynamite += 1;
		}
	}

	void UpdateDeaths(char* token, Stats* stats)
	{
		char* ret = NULL;
		ret = strstr(token, "killed you");
		if (ret != NULL) {
			stats->deaths += 1;
			return;
		}

		ret = strstr(token, "blew you up");
		if (ret != NULL) {
			stats->deaths += 1;
			stats->death_from_dynamite += 1;
		}

	}

	void Update(char* battle_text, struct Stats* stats)
	{

		size_t len_player_name = strlen(Player::Name());
		size_t len_battle_text = strlen(battle_text);

		std::string player_actions = Player::Name();
		
		// Check battle_text for player written text - Don't want players to have the ability to 
		// write "killed" and cheat on their scores
		if (len_battle_text > len_player_name + 3)
		{
			if (battle_text[len_player_name + 2] == ':') {
				// This is a player written message, check for a / command
				if (battle_text[len_player_name + 4] == '/') {
					// do some stuff with the slash command
				}
				// Exit
				return;
			}
		}

		int n = strncmp(battle_text, , len_player_name);

		if (n == 0)
		{
			// Check for actions like "killed" or "blew up"

			
		}

		//===========================

		char temp[100];

		strcpy_s(temp, _countof(temp), battle_text);

		char* token = NULL;
		char* next_token = NULL;

		token = strtok_s(temp, " ", &next_token);

		while (token != NULL) {

			// Check to see if Players name is in the text
			if (strcmp(token, "You") == 0) {
				// Check 1 of 4 possible actions and increment stats
				token = strtok_s(NULL, " ", &next_token);
				UpdateSuicideStats(token, stats);
				break;
			}

			if (strcmp(token, Player::Name()) == 0) {
				// If not a ":" or / command then check for 1 of 2 possible actions
				// "killed or blew up"
				std::cout << "Found players name " << Player::Name() << std::endl;
				token = strtok_s(NULL, " ", &next_token);
				if (strcmp(token, ":") == 0) {
					// Check to see if there is a /slash command
					token = strtok_s(NULL, "/", &next_token);
					std::cout << "Found a command " << token << std::endl;
				}
				else {
					//
					UpdateKills(token, stats);
				}
				break;
			}
			token = NULL;
		}
		// Finally if none of those conditions are met then check these 2 actions
		// strstr the original array and look for "killed you or blew you up"
		UpdateDeaths(battle_text, stats);

		// Update our KDR
		if (stats->kills == 0 || stats->deaths == 0)
		{
			stats->kdr = (float)stats->kills;
		}
		else
		{
			stats->kdr = (float)stats->kills / (float)stats->deaths;
		}


	}
}
