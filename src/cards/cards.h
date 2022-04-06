/*
 * This file is part of EasyRPG Player.
 *
 * EasyRPG Player is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EasyRPG Player is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EasyRPG Player. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EP_CARDS_MAIN_H
#define EP_CARDS_MAIN_H

#include <string>
#include <vector>

#include "../configor/json.hpp"

namespace Cards {

	struct Instance;
	Instance& instance();

	struct magic {
		virtual void process()=0;
		virtual ~magic() {}
	};

	struct draw_specific : magic {
		std::string key;
		draw_specific(std::string key):key(key){};
		void process();
	};

	struct monster {
		std::string key; std::string name; int cost; int master, id;
		int hp, HP, mp, MP;
		int AP, DP;

		std::vector<magic*> battlecry, deathrattle;

		monster(){};
		monster(configor::json json, std::string key);
		void dead(int i);
		void damaged(int d, int aid, int i);
		void physicalDamaged(int d, int aid, int i);
		void atk(int t);
		std::string info();
	};

	struct Instance {
		int current_map_event_id, selected_id;
		std::vector<monster> deck, hand, battlefield, grave;
		int hp, mp, MP, turn;

		std::vector<monster> ai_deck, ai_hand;
		int ai_hp, ai_mp, ai_MP;
		bool pause;

		configor::json json;
	};

	int getBattleFieldId(int id);
	void show();
	void atk(int id);
	void atk();
	void ai_draw();
	void draw();
	void mainLoop();
	void initJson();
	void init();
	void changeAvatar();
	void prevCard();
	void succCard();

} // namespace Cards

#endif
