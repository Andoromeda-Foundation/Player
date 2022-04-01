
#include "cards.h"
#include "../output.h"
#include "../game_interpreter.h"
#include "../game_map.h"

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

	Instance& instance() {
		return _;
	}

} // namespace Cards
