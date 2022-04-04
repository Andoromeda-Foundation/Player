
#include <lcf/reader_util.h>
#include "cards.h"
#include "../output.h"
#include "../game_interpreter.h"
#include "../game_map.h"
#include "../game_screen.h"
#include "../game_party.h"
#include "../game_actors.h"
#include "../game_variables.h"
#include "../main_data.h"
#include "../graphics.h"

#define DO(n) for ( int ____n = n; ____n-->0; )

namespace Cards {

	Instance _;


	void draw_specific::process() {
		Output::Debug("draw skull");
		for (int i=0;i<_.deck.size();++i) {
			if (_.deck[i].key == key) {
				_.hand.push_back(_.deck[i]);
				_.deck.erase(_.deck.begin() + i);
				return;
			}
		}
	}

	monster::monster(configor::json json, std::string key): key(key) {
		cost = json["cost"]; name = std::string(json["name"]);
		hp = json["hp"]; HP = json["HP"]; mp = json["mp"]; MP = json["MP"];
		AP = json["AP"]; DP = json["DP"];
		if (key == "skull") {
			deathrattle.push_back(new draw_specific("skull"));
		}
	}

	void monster::dead(int i) {
		auto map_event = Game_Map::GetEvent(id); map_event->SetActive(false);
		for (int i=0;i<deathrattle.size();++i) {
			deathrattle[i]->process();
		}
		deathrattle.clear();
		_.battlefield.erase(_.battlefield.begin() + i);
	}

	void monster::damaged(int d, int aid, int i) {
		hp -= d;
		if (hp <= 0) {
			if (aid == 142) aid += 1;
			Main_Data::game_screen->ShowBattleAnimation(aid, id, 0);
			dead(i);
		} else {
			Main_Data::game_screen->ShowBattleAnimation(aid, id, 0);
		}
	}

	std::string monster::info() {
		std::string z;
		z += name + " " +
			std::to_string(cost) + "费 " +
			"HP " + std::to_string(hp) + "/" + std::to_string(HP) + " "
			"MP " + std::to_string(mp) + "/" + std::to_string(MP) + " "
			"AP " + std::to_string(AP);
		return z;
	}

	void initJson() {
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
						{"description", "被法术操纵的人形骨骼，亡语：从卡组中检索另一张同名卡牌加入手牌。"},
						{"hp", 2},{"HP", 2},
						{"mp", 0},{"MP", 0},
						{"AP", 1},
						{"DP", 0},
						{"charset", "Monster1"},
						{"offset", 4}
					}
				},
				{
					"ghost", {
						{"name", "幽灵"},
						{"cost", 3},
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
					"bone_dragon", {
						{"name", "骨龙"},
						{"cost", 7},
						{"description", "可从墓地除外至多 3 张卡片进行召唤，每除外 1 张费用减 1。"},
						{"hp", 12},{"HP", 12},
						{"mp", 0},{"MP", 0},
						{"AP", 4},
						{"DP", 0},
						{"charset", "18277"},
						{"offset", 3}
					}
				},
				{
					"mummy", {
						{"name", "木乃伊"},
						{"cost", 3},
						{"description", "."},
						{"hp", 5},{"HP", 5},
						{"mp", 0},{"MP", 0},
						{"AP", 2},
						{"DP", 0},
						{"charset", "monster-g08"},
						{"offset", 1}
					}
				},
				{
					"grim_reaper", {
						{"name", "死神"},
						{"cost", 7},
						{"description", "随机消灭对手战场上的一个使魔，如果战场上没有地方卡牌，则随机丢弃对手一张手牌。"},
						{"hp", 15},{"HP", 15},
						{"mp", 0},{"MP", 10},
						{"AP", 4},
						{"DP", 0},
						{"charset", "18277"},
						{"offset", 2}
					}
				},
				{
					"succubus", {
						{"name", "魅魔"},
						{"cost", 5},
						{"description", "魅惑一个非亡灵单位。"},
						{"hp", 7},{"HP", 7},
						{"mp", 0},{"MP", 10},
						{"AP", 2},
						{"DP", 0},
						{"charset", "9890"},
						{"offset", 0}
					}
				},
				{
					"naga", {
						{"name", "娜迦"},
						{"cost", 6},
						{"description", "."},
						{"hp", 7},{"HP", 7},
						{"mp", 0},{"MP", 0},
						{"AP", 6},
						{"DP", 0},
						{"charset", "monster-g08"},
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
						{"charset", "monster-g04"},
						{"offset", 0}
					}
				},
				{
					"witch", {
						{"name", "魔女"},
						{"cost", 1},
						{"description", "对最近距离的敌方单位使用火球术，造成 1d6 点伤害。"},
						{"hp", 6},{"HP", 6},
						{"mp", 10},{"MP", 10},
						{"AP", 1},
						{"DP", 0},
						{"charset", "monster-g04"},
						{"offset", 5}
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
					"pikeman", {
						{"name", "枪兵"},
						{"cost", 1},
						{"description", "骑兵的冲锋无效。"},
						{"hp", 3},{"HP", 3},
						{"mp", 0},{"MP", 0},
						{"AP", 2},
						{"DP", 0},
						{"charset", "heisic2"},
						{"offset", 1}
					}
				},
				{
					"snail", {
						{"name", "大蜗牛"},
						{"cost", 1},
						{"description", "护甲 1。"},
						{"hp", 2},{"HP", 2},
						{"mp", 0},{"MP", 0},
						{"AP", 1},
						{"DP", 1},
						{"charset", "かたつむり魔物"},
						{"offset", 0}
					}
				},
				{
					"magma_snail", {
						{"name", "熔岩蜗牛"},
						{"cost", 2},
						{"description", "护甲 1。"},
						{"hp", 4},{"HP", 4},
						{"mp", 0},{"MP", 0},
						{"AP", 2},
						{"DP", 1},
						{"charset", "かたつむり魔物"},
						{"offset", 2}
					}
				},
				{
					"crusader", {
						{"name", "十字军"},
						{"cost", 4},
						{"description", "护甲 1。"},
						{"hp", 6},{"HP", 6},
						{"mp", 0},{"MP", 0},
						{"AP", 3},
						{"DP", 1},
						{"charset", "23178"},
						{"offset", 1}
					}
				},
				{
					"cavalier", {
						{"name", "骑兵"},
						{"cost", 6},
						{"description", "杀伤力随移动距离增大。"},
						{"hp", 13},{"HP", 13},
						{"mp", 0},{"MP", 0},
						{"AP", 8},
						{"DP", 0},
						{"charset", "heisic5"},
						{"offset", 0}
					}
				},
				{
					"bat", {
						{"name", "蝙蝠"},
						{"cost", 2},
						{"description", ""},
						{"hp", 3},{"HP", 3},
						{"mp", 0},{"MP", 0},
						{"AP", 1},
						{"DP", 0},
						{"charset", "17708"},
						{"offset", 1}
					}
				}
			};
		};
	}

	void init() {

		_.deck.clear();
		_.hand.clear();
		_.battlefield.clear();

		// Init Player Deck
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

		// Init Enemy Deck
		DO(20) {
			int i = 100 + rand() % 21;
			auto item = *lcf::ReaderUtil::GetElement(lcf::Data::items, i);
			std::string s = std::string(item.name);
			if (s.substr(0, 5) != ".card") continue;
			s = s.substr(6);
			_.ai_deck.push_back(monster(_.json[s], s));
		}

		DO(7) draw(); DO(7) ai_draw();
		_.hp = _.ai_hp = 20;
		Graphics::setCardsInfo(true);
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


		int this_id = _.current_map_event_id, that_id = -1;

		// Output::Debug("this_id: {}/{}", this_id,_.battlefield.size());

		Game_Event *this_event = Game_Map::GetEvent(this_id), *that_event;
		int x = this_event->GetX(), y = this_event->GetY();

		this_id = getBattleFieldId(this_id);

		// Output::Debug("this_id: {}/{}", this_id,_.battlefield.size());
		auto& this_card = _.battlefield[this_id];


		int i = 0;
		for (i=0;i<_.battlefield.size();++i) {
			if (_.battlefield[i].master != this_card.master) {
				that_event = Game_Map::GetEvent(_.battlefield[i].id);
				int xx = that_event->GetX(), yy = that_event->GetY();
				if (xx == x && yy == y || xx == x && yy == y-1 && this_card.master == 1 || xx == x && yy == y+1 && this_card.master == 2) {
					break;
				}
			}
		}

		if (i == _.battlefield.size()) {
			if (this_card.master == 1) {
				_.ai_hp -= _.battlefield[this_id].AP;
				Main_Data::game_screen->ShowBattleAnimation(142, 6, 0);
			} else {
				_.hp -= _.battlefield[this_id].AP;
				Main_Data::game_screen->ShowBattleAnimation(142, 10001, 0);
			}
		} else {
			// todo(minakokojima): add damage function.
			_.battlefield[i].damaged(this_card.AP, 142, i);
		}
	}

	void ai_draw() {
		if (_.ai_deck.empty()) return;
		int t = rand() % _.ai_deck.size();
		_.ai_hand.push_back(_.ai_deck[t]);
		_.ai_deck.erase(_.ai_deck.begin() + t);
	}

	void draw() {
		if (_.deck.empty()) return;
		int t = rand() % _.deck.size();
		_.hand.push_back(_.deck[t]);
		_.deck.erase(_.deck.begin() + t);
	}

	void ai_turn() {
		std::random_shuffle(_.ai_hand.begin(), _.ai_hand.end());
		for (int i=0;i<_.ai_hand.size();++i) {
			if (_.ai_hand[i].cost <= _.ai_mp) {
				_.ai_mp -= _.ai_hand[i].cost;
				Game_Map::summon(_.ai_hand[i], 2, 8 + rand() % 5, 8);
				_.ai_hand.erase(_.ai_hand.begin() + i);
			}
		}
	}

	void mainLoop() {
		draw(); ai_draw();
		_.turn += 1;
		_.mp = _.MP = _.turn;
		_.ai_mp = _.ai_MP = _.turn;

		for (auto &m: _.battlefield) {
			if (m.mp < m.MP) m.mp += 1;
		}

		ai_turn();
		// Output::Debug("main loop: {} {}", _.turn, _.mp);
	}

	void changeAvatar() {
		Game_Actor* actor = Main_Data::game_actors->GetActor(1);
		auto file = ToString(Main_Data::game_actors->GetActor(14)->GetName());
		int idx = int(Main_Data::game_variables->Get(1));
		actor->SetSprite(file, idx, 0);
		Main_Data::game_player->ResetGraphic();
	}

	Instance& instance() {
		return _;
	}

} // namespace Cards

#undef DO
