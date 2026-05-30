#include <iostream>
#include <map>
#include <list>
#include <string>
#include <random>
#include <ctime>
#include <fstream>
//list<map<int, list<string>>>
//kapott adatszerkezet

//a kulso lista jatekosokat valasztja el
//a belso map maga az inventory játékosonként
//a mapen alértékei az item egyedi azonosítója (unique_id) és tulajdonságai (list<string) -aka properties

// A kód célja egy rpg szerű inventory rendszer kezelő class

// célkitűzések: item hozzáadás, adatsorból random item hozzáadás (lootolás) akar több itemet,
// item eltávolítása játékos inventoryból, item tulajdonos csere,

using namespace std;
using Inventory = map<int, list<string>>;
using Players = list<Inventory>;


//DATABASE for random item lootolás (a rarity nem arányos a randomsággal)
list<tuple<string, string, int>> base_items = {
// --- TIER 0: COMMON ---
{"Rusty Dagger", "Weapon", 0},
{"Wooden Club", "Weapon", 0},
{"Worn Leather Boots", "Armor", 0},
{"Tattered Cloak", "Armor", 0},
{"Cracked Health Vial", "Potion", 0},
{"Dull Bronze Ring", "Accessory", 0},
{"Shirt of Toxicity", "Armor", 0}, // Low tier meme
{"Bonk Bat", "Weapon", 0},         // Low tier meme
{"Broken Glass Shard", "Armor", 0},
{"Rubber Chicken", "Weapon", 0},   // Joke item
{"Old Sock", "Accessory", 0},

// --- TIER 1: UNCOMMON (Standard Soldier/Adventurer) ---
{"Iron Shortsword", "Weapon", 1},
{"Steel Hatchet", "Weapon", 1},
{"Hardened Leather Vest", "Armor", 1},
{"Reinforced Buckler", "Armor", 1},
{"Standard Healing Potion", "Potion", 1},
{"Antidote", "Potion", 1},
{"Silver Amulet", "Accessory", 1},
{"GG Extract", "Potion", 1},              // Meme
{"Mountain Dew of Agility", "Potion", 1},  // Meme
{"The 'This is Fine' Mug", "Accessory", 1},// Meme
{"Infinity Gauntlet (Wish version)", "Accessory", 1}, // Meme

// --- TIER 2: RARE (Magical or High-Quality) ---
{"Mithril Rapier", "Weapon", 2},
{"Longbow of the Hunt", "Weapon", 2},
{"Chainmail Hauberk", "Armor", 2},
{"Knight's Greatshield", "Armor", 2},
{"Elixir of Strength", "Potion", 2},
{"Glowing Mana Crystal", "Accessory", 2},
{"Emotional Damage", "Weapon", 2}, // Meme
{"Blue Screen Shield", "Armor", 2}, // Meme

// --- TIER 3: EPIC (Heroic equipment) ---
{"Orichalcum Claymore", "Weapon", 3},
{"Phoenix Wing Staff", "Weapon", 3},
{"Crystal-Inlaid Plate", "Armor", 3},
{"Cloak of Displacement", "Armor", 3},
{"Essence of Vitality", "Potion", 3},
{"Ring of the Archmage", "Accessory", 3},
{"Rickroll's Microphone", "Weapon", 3}, // Meme

// --- TIER 4: LEGENDARY (Relics of the past) ---
{"Dragon-Tooth Blade", "Weapon", 4},
{"Void-Walker Bow", "Weapon", 4},
{"Sun-Blessed Aegis", "Armor", 4},
{"Nokia 3310 (Unbreakable)", "Armor", 4}, // High tier meme
{"Belt of Giant Strength", "Accessory", 4},
{"Amulet of Eternal Life", "Accessory", 4},
{"Diamond-Encrusted Keyboard", "Weapon", 4}, // Meme

// --- TIER 5: MYTHIC (Demigod level) ---
{"Star-Forged Warhammer", "Weapon", 5},
{"God-Slayer Dagger", "Weapon", 5},
{"Shaggy's 1% Power Shirt", "Armor", 5}, // High tier meme
{"ALL YOUR BASE", "Accessory", 5},       // High tier meme
{"Doge's Diamond Paws", "Accessory", 5}, // High tier meme
{"Cape of the Multiverse", "Armor", 5},
{"Ambrosia of the Gods", "Potion", 5},

// --- TIER 6: ARTIFACT (Breaking the fourth wall / Reality) ---
{"Plot Armor", "Armor", 6},              // Top tier meme
{"The Ban Hammer", "Weapon", 6},         // Top tier meme
{"Excalibur (Actually Real)", "Weapon", 6},
{"Eye of the Developer", "Accessory", 6},
{"Infinite Loop Hourglass", "Accessory", 6},
{"Absolute Unit Greaves", "Armor", 6},
{"Your smile", "Accessory", 6}

};
list<string> rarities = {
    "Common",
    "Uncommon",
    "Rare",
    "Epic",
    "Legendary",
    "Mythic",
    "Artifact"
};
list<string> enchantments = {
    "+10% Attack Speed",
    "+5% Critical Hit Chance",
    "Life Steal: 3%",
    "Movement Speed: +15",
    "Mana Regeneration: +2.5/s",
    "15% Chance to Stun",
    "Armor Penetration: +10",
    "Health Regeneration: +5/s",
    "Cool-down Reduction: 8%",
    "Bonus Gold from Kills: +12%"

};

ostream & operator<<(ostream& os, const list<string>& props) {
    for (auto it = props.begin(); it!=props.end();++it) {
        os<<*it<<(next(it) != props.end() ? ", " : "");
    }
    return os;
}

ostream & operator<<(ostream& os, const Inventory & inv) {

    if (inv.empty()) {
        os<<"This player's inventory is empty."<<endl;
        return os;
    }
    for (const auto& [id, props] : inv) {


        os<<"{Item ID: "<<id<<"},"<<props<<endl;
    }
    return os;
}

class InventoryManager {
    Players player_inventories;
    int next_unique_id;
    int total_items =0;

public:
    //Kontruktor:
    InventoryManager(int player_count) : next_unique_id(1000){
        for (int i=0; i<player_count; i++) {
            player_inventories.push_back(Inventory());
        }
    }

    void addItem(int player_idx, list<string> properties) {
        auto it = player_inventories.begin();
        advance(it, player_idx); //++it/--it player_idx -szer

        int unq_id = next_unique_id++;
        (*it)[unq_id] = properties;
        total_items++;

        auto prop_it = properties.begin();
        string base_name = *prop_it;
        string type = *++prop_it;
        string rarity = *++prop_it;

        string ench = "";

        if (properties.size()>3){
        ench =*++prop_it;}
        if (ench!="") {
            cout<< "Player ID: " << player_idx << " gained "<<"{Item ID: "<<unq_id<<"}, "<<type<<": "<< base_name << " [" << rarity << "] with " <<ench << endl;
        }
        else{cout<< "Player ID: " << player_idx << " gained "<<"{Item ID: "<<unq_id<<"}, "<<type<<": "<< base_name << " [" << rarity << "]" << endl;}

    }

    void removeItem(int unique_item_idx) {
        for (Inventory& inv: player_inventories) {
            auto it = inv.find(unique_item_idx);
            if (it != inv.end()) {
                inv.erase(it);
                cout << "Item with id: " << unique_item_idx << " was removed from its player's inventory." << endl;
                total_items--;
                return;
            }
        }
        cout<<"No such item exist with id: "<<unique_item_idx<<endl;
    }

    void swapItem(int p_idx_from, int p_idx_to, int unique_item_idx) {
        if (p_idx_from < 0 || p_idx_from >= player_inventories.size() ||
        p_idx_to < 0 || p_idx_to >= player_inventories.size()) {
            cout << "Error: Player indices out of bounds." << endl;
            return;
        }

        auto from_it = player_inventories.begin();
        advance(from_it, p_idx_from);
        Inventory& from_inv = *from_it;


        auto item_it = from_inv.find(unique_item_idx);
        if (item_it == from_inv.end()) {
            cout << "Error: Player " << p_idx_from << " does not have item ID: " << unique_item_idx << endl;
            return;
        }

        auto toit = player_inventories.begin();
        advance(toit, p_idx_to);
        Inventory& to_inv = *toit;

        //The actual swap
        to_inv[unique_item_idx] = item_it->second;
        from_inv.erase(item_it);

        cout << "Item ID " << unique_item_idx << " was traded from Player "
         << p_idx_from << " to Player " << p_idx_to << "." << endl;
    }


    void LOOTING(int player_idx, int loot_amount=1) {

        for (int i=0; i<loot_amount; i++) {
            list<string> new_item_properties;

            auto lit = base_items.begin();
            advance(lit, rand()%base_items.size());

            string name = get<0>(*lit);
            string type = get<1>(*lit);
            int base_tier_idx = get<2>(*lit);

            new_item_properties.push_back(name);
            new_item_properties.push_back(type);

            int luck = rand()%2;
            int final_tier_idx =base_tier_idx;
            if (base_tier_idx!=6) {final_tier_idx = base_tier_idx + luck;};

            auto rit = rarities.begin();
            string ench = "";
            advance(rit, final_tier_idx);
            new_item_properties.push_back(*rit);

            luck = rand()%2; //new roll
            if (luck ==1) {
                auto enit = enchantments.begin();
                advance(enit, rand()%enchantments.size());
                ench = (*enit);
            }
            new_item_properties.push_back(ench);

            //int unq_id = next_unique_id++;
            //(*player_it)[unq_id] = new_item_properties;

            addItem(player_idx, new_item_properties);

            //cout<< "Player ID: " << player_idx << " Looted: "<< name << " [" << *rit << "]" << endl;
        }

    }

    void showPlayerInventory(int player_idx) {
        if (player_idx < 0 || player_idx >= player_inventories.size()) {
            cout<< "Error: Player index out of bounds." << endl;
            return;
        }
        auto it = player_inventories.begin();
        advance(it, player_idx);

        cout << "Inventory of the Player with index: " << player_idx << ":" << endl;
        cout<<(*it)<<endl;
    }
    int Get_totalItems(){return total_items;};

    void showALL_INVENTORY() {
        for (int i =0; i<player_inventories.size();i++) {
            showPlayerInventory(i);
        }
    }

    //todo: saveToFile() és loadFromFile()

};


int main() {
    srand(time(NULL));

    int player_count_in_main=(rand()%5)+5;
    InventoryManager IM(player_count_in_main);

    cout<<"ADD ITEM FUNCTION: "<<endl;
    IM.addItem(0, {"Vaskard", "Weapon", "Common",});
    IM.addItem(1, {"Varazspalca","Weapon","Epic"});

    cout<<endl;

    cout<<"RANDOM LOOTING FUNCTION: "<<endl;
    int loot_amount_outter =(rand()%3) +1;
    IM.LOOTING(rand()%player_count_in_main, loot_amount_outter);
    cout<<endl;

    loot_amount_outter =(rand()%3) +1;
    IM.LOOTING(rand()%player_count_in_main, loot_amount_outter);

    cout<<endl;

    cout<<"REMOVE ITEM FUNCTION: "<<endl;
    IM.removeItem(1000+rand()%(IM.Get_totalItems()+1));
    cout<<" ---Intended error testing--- "<<endl;
    IM.removeItem(10010);
    cout<<" ---Intended error testing--- "<<endl;
    cout<<endl;

    cout<<"Inventory of PLayer FUNCTION: "<<endl;

    IM.showPlayerInventory(rand()%player_count_in_main);
    IM.showPlayerInventory(rand()%player_count_in_main);

    cout<<"Inventory of EVERYONE FUNCTION:\n"<<endl;
    IM.showALL_INVENTORY();

    cout<<"SWAP INVENTORY FUNCTION\n"<<endl;
    IM.swapItem(0,rand()%player_count_in_main,1000);
    //one person CAN swap with themselves, it is an intended *feauture* (realistically unnecessary to fix)


    return 0;
}