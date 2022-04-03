
#include <lcf/reader_util.h>
#include "cards.h"
#include "../output.h"
#include "../game_interpreter.h"
#include "../game_map.h"
#include "../game_screen.h"
#include "../game_party.h"
#include "../main_data.h"

#define DO(n) for ( int ____n = n; ____n-->0; )

namespace Cards {

	Instance _;

	void init() {
		if (_.json.empty()) {
			_.json = {
				{
					"wisp", {
						{"name", "小精灵"},
						{"cost", 0},
						{"description", "零费小精灵，旅法第一卡。你说她不行，你行你上啊。"},
						{"hp", 1},{"HP", 1},
						{"mp", 0},{"MP", 3},
						{"AP", 1},
						{"DP", 0},
						{"charset", "monster-e02"},
						{"offset", 4}
					}
				},
				{
					"slime", {
						{"name", "史莱姆"},
						{"cost", 1},
						{"description", "滑溜溜的魔法生物，可以从地上弹起，黏住敌人，分泌酸液。虽然初始威胁不大，但任由她增殖的话也会带来意想不到的麻烦。"},
						{"hp", 2},{"HP", 2},
						{"mp", 0},{"MP", 5},
						{"AP", 1},
						{"DP", 0},
						{"charset", "Monster1"},
						{"offset", 0}
					}
				},
				{
					"gaint_slime", {
						{"name", "巨型史莱姆"},
						{"cost", 5},
						{"description", "在魔法事故中诞生的巨型史莱姆。"},
						{"hp", 10},{"HP", 10},
						{"mp", 2},{"MP", 20},
						{"AP", 3},
						{"DP", 0},
						{"charset", "char_m_sl_g"},
						{"offset", 2}
					}
				},
				{
					"rock_man", {
						{"name", "石头人"},
						{"cost", 3},
						{"description", "炼金术所产生的副产品。"},
						{"hp", 7},{"HP", 7},
						{"mp", 0},{"MP", 0},
						{"AP", 4},
						{"DP", 0},
						{"charset", "Monster2"},
						{"offset", 4}
					}
				},
				{
					"skull", {
						{"name", "骷髅"},
						{"cost", 1},
						{"description", "被法术操纵的人形骨骼。"},
						{"hp", 3},{"HP", 3},
						{"mp", 0},{"MP", 0},
						{"AP", 2},
						{"DP", 0},
						{"charset", "Monster1"},
						{"offset", 4}
					}
				},
				{
					"ghost", {
						{"name", "幽灵"},
						{"cost", 1},
						{"description", "Ghost Reporting."},
						{"hp", 5},{"HP", 5},
						{"mp", 0},{"MP", 0},
						{"AP", 4},
						{"DP", 0},
						{"charset", "Monster1"},
						{"offset", 3}
					}
				},
				{
					"diamond_men", {
						{"name", "钻石人"},
						{"cost", 4},
						{"description", "比金人更厉害的防魔法兵种，他们对魔法的伤害只接受5%，所以对付大量的钻石人基本就不要指望攻击魔法了。"},
						{"hp", 9},{"HP", 9},
						{"mp", 0},{"MP", 0},
						{"AP", 5},
						{"DP", 0},
						{"charset", "18557"},
						{"offset", 0}
					}
				},
				{
					"manticore", {
						{"name", "蝎狮"},
						{"cost", 6},
						{"description", "红色毛皮、蝙蝠翅膀和一条长满如豪猪尖刺的尾巴，另有一说则是拥有蝎子的尾针，并拥有带着三列像鲨鱼一样的尖锐牙齿的人脸狮子形态的怪物。有着无限的食欲，据说可以吃掉一个国家的军队。"},
						{"hp", 13},{"HP", 13},
						{"mp", 0},{"MP", 10},
						{"AP", 7},
						{"DP", 0},
						{"charset", "16549"},
						{"offset", 0}
					}
				},
				{
					"green_dragon", {
						{"name", "绿龙"},
						{"cost", 7},
						{"description", "绿龙是善良温和的龙类，但胆敢惹怒它们的人也会遭受灭顶之灾"},
						{"hp", 15},{"HP", 15},
						{"mp", 0},{"MP", 20},
						{"AP", 8},
						{"DP", 0},
						{"charset", "viptmp1139"},
						{"offset", 0}
					}
				},
				{
					"centaur", {
						{"name", "半人马"},
						{"cost", 1},
						{"description", "据说是最强 1 费生物。"},
						{"hp", 3},{"HP", 3},
						{"mp", 0},{"MP", 0},
						{"AP", 1},
						{"DP", 0},
						{"charset", "viptmp1139"},
						{"offset", 0}
					}
				},
				{
					"stone_gargoyle", {
						{"name", "石像鬼"},
						{"cost", 2},
						{"description", "可以变成石头。"},
						{"hp", 4},{"HP", 4},
						{"mp", 0},{"MP", 0},
						{"AP", 2},
						{"DP", 0},
						{"charset", "18550"},
						{"offset", 0}
					}
				},
				{
					"crusader", {
						{"name", "十字军"},
						{"cost", 4},
						{"description", "攻击两次。"},
						{"hp", 6},{"HP", 6},
						{"mp", 0},{"MP", 0},
						{"AP", 3},
						{"DP", 0},
						{"charset", "23178"},
						{"offset", 1}
					}
				}

			};
		};

		_.hand.clear();
		_.deck.clear();

		std::vector<int> party_items;
		Main_Data::game_party->GetItems(party_items);

		for (size_t i = 0; i < party_items.size(); ++i) {
			auto item = *lcf::ReaderUtil::GetElement(lcf::Data::items, party_items[i]);
			std::string s = std::string(item.name);
			if (s.substr(0, 5) != ".card") continue;
			s = s.substr(6);
			int cnt = Main_Data::game_party->GetItemCount(party_items[i]);
			DO(cnt) _.deck.push_back(monster(_.json[s], s));
		}

		DO(7) draw();
	}

	void show() {
		lcf::rpg::EventCommand com;
		//com.string = (std::string("我是 ") + std::to_string(_.current_map_event_id)).c_str();
		// com.string = lcf::DBString(std::to_string(_.current_map_event_id));

		for (int i=0;i<_.battlefield.size();++i) {
			// Output::Debug("id,id: {} {}", _.p1[i].id, _.current_map_event_id);
			if (_.battlefield[i].id == _.current_map_event_id) {
				com.string = lcf::DBString(_.battlefield[i].info());
				break;
			}
		}

		auto t = Game_Map::GetInterpreter();
		t.CommandShowMessage(com);
	}

	int getBattleFieldId(int id) {
		for (int i=0;i<_.battlefield.size();++i) {
			if (_.battlefield[i].id == id) {
				return i;
			}
		}
		return -1;
	}

	void atk() {
		/*
		int this_id = _.current_map_event_id, that_id = -1;
		Game_Event *this_event = Game_Map::GetEvent(this_id), *that_event;
		int x = this_event->GetX(), y = this_event->GetY();

		int i = 0;

		this_id = getBattleFieldId(this_id);

		for (i=0;i<_.battlefield.size();++i) {
			if (_.battlefield[i].master != _.battlefield[this_id].master) {
				that_event = Game_Map::GetEvent(_.battlefield[i].id);
				int xx = that_event->GetX(), yy = that_event->GetY();
				if (xx == x && yy == y - 1) {
					break;
				}
			}
		}
		if (i == _.battlefield.size()) {
			Main_Data::game_screen->ShowBattleAnimation(142, 6, 0);
		} else {
			_.battlefield[i].hp -= _.battlefield[this_id].AP;
			if (_.battlefield[i].hp <= 0) {
				Main_Data::game_screen->ShowBattleAnimation(143, _.battlefield[i].id, 0);
				that_event->SetActive(false);
				_.battlefield.erase(_.battlefield.begin() + i);
			} else {
				Main_Data::game_screen->ShowBattleAnimation(142, _.battlefield[i].id, 0);
			}
		}*/
	}

	void draw() {
		if (_.deck.empty()) return;
		int t = rand() % _.deck.size();
		Output::Debug("Draw id: {}", _.deck[t].name);
		_.hand.push_back(_.deck[t]);
		_.deck.erase(_.deck.begin() + t);
	}

	Instance& instance() {
		return _;
	}

} // namespace Cards

#undef DO
