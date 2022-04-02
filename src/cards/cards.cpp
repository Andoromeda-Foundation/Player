
#include "cards.h"
#include "../output.h"
#include "../game_interpreter.h"
#include "../game_map.h"
#include "../game_screen.h"
#include "../main_data.h"


namespace Cards {

	Instance _;

	void show() {
		lcf::rpg::EventCommand com;
		//com.string = (std::string("我是 ") + std::to_string(_.current_map_event_id)).c_str();
		// com.string = lcf::DBString(std::to_string(_.current_map_event_id));
		Output::Debug("size: {}", _.p1.size());
		for (int i=0;i<_.p1.size();++i) {
			Output::Debug("id,id: {} {}", _.p1[i].id, _.current_map_event_id);
			if (_.p1[i].id == _.current_map_event_id) {
				com.string = lcf::DBString(_.p1[i].info());
				break;
			}
		}

		auto t = Game_Map::GetInterpreter();
		t.CommandShowMessage(com);
		/*
		Game_Map::GetInterpreter()
		Game_Interpreter::CommandShowMessage(com);*/
	}

	int owner(int id) {
		for (int i=0;i<_.p1.size();++i) {
			if (_.p1[i].id == id) {
				return 1;
			}
		}
		for (int i=0;i<_.p2.size();++i) {
			if (_.p2[i].id == id) {
				return 2;
			}
		}
		return 0;
	}

	bool inBattleField(int id) {
		for (int i=0;i<_.p1.size();++i) {
			if (_.p1[i].id == id) {
				return true;
			}
		}
		for (int i=0;i<_.p2.size();++i) {
			if (_.p2[i].id == id) {
				return true;
			}
		}
		return false;
	}

	void atk() {
		int this_id = _.current_map_event_id, that_id = -1;
		Game_Event *this_event = Game_Map::GetEvent(this_id), *that_event;
		monster* that_monster;
		int x = this_event->GetX(), y = this_event->GetY();

		int i = 0;
		if (owner(this_id) == 1) {
			for (i=0;i<_.p2.size();++i) {
				that_event = Game_Map::GetEvent(_.p2[i].id);
				int xx = that_event->GetX(), yy = that_event->GetY();
				if (xx == x && yy == y - 1) {
					break;
				}
			}
		} else {

		}

		if (i == _.p2.size()) {
			Main_Data::game_screen->ShowBattleAnimation(142, 6, 0);
		} else {

			_.p2[i].hp -= 2;
			if (_.p2[i].hp < 0) {
				Main_Data::game_screen->ShowBattleAnimation(143, _.p2[i].id, 0);
				that_event->SetActive(false);
				_.p2.erase(_.p2.begin() + i);
			} else {
				Main_Data::game_screen->ShowBattleAnimation(142, _.p2[i].id, 0);
			}
		}

	}

	Instance& instance() {
		return _;
	}

} // namespace Cards
