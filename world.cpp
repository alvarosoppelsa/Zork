#include <iostream>
#include "globals.h"
#include "entity.h"
#include "creature.h"
#include "item.h"
#include "exit.h"
#include "room.h"
#include "player.h"
#include "world.h"
#include "puzzle.h"

// ----------------------------------------------------
World::World()
{
	tick_timer = clock();

	// Rooms ----
	Room* crew_rest   = new Room("Crew's Room","It's all dark here!", true);
	Room* hallway     = new Room("Hallway", "Can't see anyone. Something is not right...");
	Room* warehouse   = new Room("Warehouse", "Maybe I can find some useful stuff");
    Room* engine      = new Room("Engine Room", "I hate the smell of the engine");
    Room* control     = new Room("Control", "");
    Room* space       = new Room("Space", "");

	Exit* ex1 = new Exit("forward", "back", "A door with a faint light on it", crew_rest, hallway);
	Exit* ex2 = new Exit("left", "right", "Someone left this open", hallway, warehouse);
    Exit* ex3 = new Exit("forward", "back", "An electric door... without power will be impossible to open", hallway, control, true);
    ex3->lock();
    Exit* ex4 = new Exit("right", "left", "This door always stucks", hallway, engine, false, MUTABLE, "Restart power pin: 753148");
    ex4->lock();
    Exit* ex5 = new Exit("hatch", "control", "I don't want to open that", control, space);
    ex5->lock();

	entities.push_back(crew_rest);
	entities.push_back(hallway);
	entities.push_back(warehouse);
    entities.push_back(engine);
    entities.push_back(control);

	entities.push_back(ex1);
	entities.push_back(ex2);
    entities.push_back(ex3);
    entities.push_back(ex4);
    /*
	// Creatures ----
	Creature* butler = new Creature("Butler", "It's James, the house Butler.", house);
	butler->hit_points = 10;

	entities.push_back(butler);
    */
	// Items -----
	Item* backpack   = new Item("Backpack", "My old backpack", crew_rest, BAG);
    Item* wrench     = new Item("Wrench", "An always useful tool", backpack, TOOL);
    Item* flashlight = new Item("Flashlight", "This could help me to see in dark places", warehouse, LIGHT);
    
    entities.push_back(backpack);
    entities.push_back(wrench);
    entities.push_back(flashlight);

    ex4->setKey(wrench);

    // Puzzles -----
    Puzzle* keypad = new Puzzle("Keypad", "\"Enter pin to restart airship power\"", engine, "753148", ex3);

	// Player ----
	player = new Player("Lieutenant E.R.", "You better figure it out whats going on here", crew_rest);
	player->setHitPoints(25);
	entities.push_back(player);
}

// ----------------------------------------------------
World::~World()
{
	for(std::list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
		delete *it;

	entities.clear();
}

// ----------------------------------------------------
bool World::Tick(std::vector<std::string>& args)
{
	bool ret = true;

	if(args.size() > 0 && args[0].length() > 0)
		ret = ParseCommand(args);

	GameLoop();

	return ret;
}

// ----------------------------------------------------
void World::GameLoop()
{
	clock_t now = clock();

	if((now - tick_timer) / CLOCKS_PER_SEC > TICK_FREQUENCY)
	{
		for(std::list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
			(*it)->Tick();

		tick_timer = now;
	}
}

// ----------------------------------------------------
bool World::ParseCommand(std::vector<std::string>& args)
{
	bool ret = true;

	switch(args.size())
	{
		case 1: // commands with no arguments ------------------------------
		{
			if(Same(args[0], "look") || Same(args[0], "l"))
			{
				player->Look(args);
			}
			else if(Same(args[0], "north") || Same(args[0], "n"))
			{
				(args.size() == 1) ? args.push_back("north") : args[1] = "north";
				player->Go(args);
			}
			else if(Same(args[0], "south") || Same(args[0], "s"))
			{
				(args.size() == 1) ? args.push_back("south") : args[1] = "south";
				player->Go(args);
			}
			else if(Same(args[0], "east") || Same(args[0], "e"))
			{
				(args.size() == 1) ? args.push_back("east") : args[1] = "east";
				player->Go(args);
			}
			else if(Same(args[0], "west") || Same(args[0], "w"))
			{
				(args.size() == 1) ? args.push_back("west") : args[1] = "west";
				player->Go(args);
			}
			else if(Same(args[0], "up") || Same(args[0], "u"))
			{
				(args.size() == 1) ? args.push_back("up") : args[1] = "up";
				player->Go(args);
			}
			else if(Same(args[0], "down") || Same(args[0], "d"))
			{
				(args.size() == 1) ? args.push_back("down") : args[1] = "down";
				player->Go(args);
			}
			else if(Same(args[0], "stats") || Same(args[0], "st"))
			{
				player->Stats();
			}
			else if(Same(args[0], "inventory") || Same(args[0], "i"))
			{
				player->Inventory();
			}
			else
				ret = false;
			break;
		}
		case 2: // commands with one argument ------------------------------
		{
			if(Same(args[0], "look") || Same(args[0], "l"))
			{
				player->Look(args);
			}
			else if(Same(args[0], "go"))
			{
				player->Go(args);
			}
			else if(Same(args[0], "take") || Same(args[0], "pick"))
			{
				player->Take(args);
			}
			else if(Same(args[0], "drop") || Same(args[0], "put"))
			{
				player->Drop(args);
			}
			else if(Same(args[0], "equip") || Same(args[0], "eq"))
			{
				player->Equip(args);
			}
			else if(Same(args[0], "unequip") || Same(args[0], "uneq"))
			{
				player->UnEquip(args);
			}
			else if(Same(args[0], "examine") || Same(args[0], "ex"))
			{
				player->Examine(args);
			}
			else if(Same(args[0], "attack") || Same(args[0], "at"))
			{
				player->Attack(args);
			}
			else if(Same(args[0], "loot") || Same(args[0], "lt"))
			{
				player->Loot(args);
			}
			else
				ret = false;
			break;
		}
		case 3: // commands with two arguments ------------------------------
		{
			break;
		}
		case 4: // commands with three arguments ------------------------------
		{
			if(Same(args[0], "unlock") || Same(args[0], "unlk"))
			{
				player->UnLock(args);
			}
			else if(Same(args[0], "lock") || Same(args[0], "lk"))
			{
				player->Lock(args);
			}
			else if(Same(args[0], "take") || Same(args[0], "pick"))
			{
				player->Take(args);
			}
			else if(Same(args[0], "drop") || Same(args[0], "put"))
			{
				player->Drop(args);
			}
			else
				ret = false;
			break;
		}
		default:
		ret =  false;
	}

	return ret;
}