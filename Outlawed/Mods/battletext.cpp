#include "../pch.h"
#include "battletext.h"
#include "../Game/player.h"
#include "../Game/players.h"
#include "../Game/memory.h"
#include "../Lib/stringops.h"
#include "../globals.h"

namespace BattleText
{

	uintptr_t	player_name = (uintptr_t)Memory::base + 0x1cc8f4;

	const size_t len_killed = 8;
	const size_t len_blew_up = 9;

	enum Actions
	{
		killed = 8,
		blewup = 9
	};

	enum PlayerType
	{
		player,
		enemy
	};


	void UpdateTotalKillsOnEnemy(
		Actions action,
		size_t len_player_name,
		char* battle_text,
		size_t len_battle_text, 
		Players* players_list, 
		size_t players_list_pos)
	{
		char player_name[MAX_CHAT_BUF];

		stringops::slicestr(battle_text, player_name, len_player_name + action, len_battle_text);

	}

	//void UpdateTotalKillsOnPlayer(Actions action, char* battle_text, Players* enemy)
	//{
	//	char tmp[NAME_BUF_SIZE];
	//	stringops::slicestr(battle_text, tmp, strlen(Player::Name()) + action, strlen(battle_text));

	//	int tmp_len = strlen(tmp);

	//	for (int i = 0; i < tmp_len; i++)
	//	{
	//		if (strncmp(enemy[i].name, tmp, tmp_len) == 0)
	//		{
	//			enemy[i].total_kills_against_player += 1;
	//		}
	//	}
	//}

	void UpdateSuicideStats(char* battle_text, struct Stats* stats)
	{
		if (strncmp(battle_text, "You", 3) == 0)
		{
			switch (battle_text[4])
			{
			case 'b':
				stats->deaths += 1;
				stats->suicides_dynamite += 1;
				stats->death_from_dynamite += 1;
				break;

			case 'd':
				stats->deaths += 1;
				stats->suicides_drowned += 1;
				break;

			case 'g':
				stats->deaths += 1;
				stats->suicides_crushed += 1;
				break;

			case 'f':
				stats->deaths += 1;
				stats->suicides_fell += 1;

			default:
				break;
			}
		}
	}

	void UpdateDeaths(char* battle_text, Stats* stats)
	{
		char* ret = NULL;
		ret = strstr(battle_text, "killed you");
		if (ret != NULL) {
			stats->deaths += 1;
			return;
		}

		ret = strstr(battle_text, "blew you up");
		if (ret != NULL) {
			stats->deaths += 1;
			stats->death_from_dynamite += 1;
		}

	}

	void Update(char* battle_text, struct Stats* stats, Players* players)
	{

		//size_t len_player_name = strlen(Player::Name());
		size_t len_battle_text = strlen(battle_text);
		size_t len_player = 0;
		size_t players_list_pos = 0;
		int player_type = player;

		char player_name[NAME_BUF_SIZE];

		bool player = false;

		for (size_t i = 0; i < MAX_LOBBY_PLAYERS; i++)
		{
			len_player = strlen((char*)players[i].name);

			if (len_player > 0 &&
				strncmp(battle_text, (char*)players[i].name, len_player) == 0)
			{
				if (strncmp((char*)players[i].name, Player::Name(), len_player) == 0)
				{
					player_type = player;
					players_list_pos = i;
					player = true;
					break;
				}
				player_type = enemy;
				players_list_pos = i;
				player = false;
				break;
			}
		}

		if (len_battle_text > len_player + 2)
		{
			switch (battle_text[len_player + 1])
			{
			case ':':
				// Future - Check for a /slash command 
				// For now just exit because we don't care about the user chat
				break;

			case 'k':
				if (player) {
					// +1 total kills
					stats->kills += 1;
					
				}
				else {

				}
				break;

			case 'b':
				if (player) {
					// +1 total kills and +1 dynamite
					stats->kills += 1;
					stats->kills_from_dynamite += 1;
				}
				else {

				}
				break;

			default:
				break;
			}
		}





		//for (int i = 0; i < MAX_LOBBY_PLAYERS; i++)
		//{
		//	if (strncmp(enemy[i].name, battle_text, strlen(enemy[i].name)) == 0)
		//	{
		//		if (strncmp(enemy[i].name, Player::Name(), strlen(enemy[i].name)) == 0)
		//		{
		//			if (len_battle_text > len_player_name + 2)
		//			{
		//				switch (battle_text[len_player_name + 1])
		//				{
		//				case ':':
		//					// Future - Check for a /slash command 
		//					// For now just exit because we don't care about the user chat
		//				case 'k':
		//					// +1 total kills
		//					stats->kills += 1;

		//					break;
		//				case 'b':
		//					// +1 total kills and +1 dynamite
		//					stats->kills += 1;
		//					stats->kills_from_dynamite += 1;
		//					break;
		//				default:
		//					break;
		//				}
		//			}
		//		}
		//	}
		//}

		/* Check for players name */
		//if (strncmp(Player::Name(), battle_text, len_player_name) == 0)
		//{
		//	if (len_battle_text > len_player_name + 2) // i.e. "player_name :"
		//	{
		//		switch (battle_text[len_player_name + 1])
		//		{
		//		case ':':
		//			// Future - Check for a /slash command 
		//			// For now just exit because we don't care about the user chat
		//		case 'k':
		//			// +1 total kills
		//			stats->kills += 1;
		//			break;
		//		case 'b':
		//			// +1 total kills and +1 dynamite
		//			stats->kills += 1;
		//			stats->kills_from_dynamite += 1;
		//			break;
		//		default:
		//			break;
		//		}
		//	}
		//}

		/* Disregard any player chat. (All player chat uses a colon) */
		//if (PlayersChatting(battle_text)) return;

		UpdateSuicideStats(battle_text, stats);
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