#include <iostream>
#include "globals.h"
#include "entity.h"
#include "creature.h"
#include "boss.h"
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
    Room* control     = new Room("Control", "The rest of the crew is laying on the floor");
    Room* space       = new Room("Space", "Somewere in the Reticulum constellation");

    entities.push_back(crew_rest);
    entities.push_back(hallway);
    entities.push_back(warehouse);
    entities.push_back(engine);
    entities.push_back(control);
    entities.push_back(space);

	Exit* ex1 = new Exit("forward", "back", "A door with a faint light on it", crew_rest, hallway);
	Exit* ex2 = new Exit("left", "right", "Someone left this open", hallway, warehouse);
    Exit* ex3 = new Exit("up", "down", "An electric door... in an emergency, only opens after restarting power",
                         hallway, control, true);
    ex3->lock();
    Exit* ex4 = new Exit("right", "left", "This door always stucks", hallway, engine, false, MUTABLE, "Restart power pin: 753148");
    ex4->lock();
    Exit* ex5 = new Exit("hatch", "control", "I don't want to open that before tied myself somewhere", 
                         control, space, false, MUTABLE, "Open the hatch is my only chance to come back home...");
    ex5->lock();
    ex5->setPush(true);
    Exit* ex6 = new Exit("down", "space", "If I open that, surely I will be pushed to the outer space", warehouse, space);
    ex6->lock();

    entities.push_back(ex1);
    entities.push_back(ex2);
    entities.push_back(ex3);
    entities.push_back(ex4);
    entities.push_back(ex5);
    entities.push_back(ex6);

    // Creatures ----
	Boss* alien = new Boss("Alien", "What the hell is that!!", control);
	alien->setHitPoints(999);

	entities.push_back(alien);
    
	// Items -----
	Item* backpack   = new Item("Backpack", "My old backpack", crew_rest, BAG);
    Item* wrench     = new Item("Wrench", "An always useful tool", backpack, TOOL);
    Item* flashlight = new Item("Flashlight", "This could help me to see in dark places", warehouse, LIGHT);
    Item* claw       = new Item ("Claw", "", alien, WEAPON);
    claw->setMinValue(1);
    claw->setMaxValue(10);

    entities.push_back(backpack);
    entities.push_back(wrench);
    entities.push_back(flashlight);
    entities.push_back(claw);

    ex4->setKey(wrench);
    alien->AutoEquip();

    // Puzzles -----
    Puzzle* keypad = new Puzzle("Keypad", "\"Enter pin to restart airship power\"", engine, "753148", ex3, PASSWORD);
    Puzzle* hook   = new Puzzle("Hook", "This should be strong enough to tie myself", control, "tie", ex5, ACTION);

    entities.push_back(keypad);
    entities.push_back(hook);

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
			else if(Same(args[0], "forward") || Same(args[0], "f"))
			{
				(args.size() == 1) ? args.push_back("forward") : args[1] = "forward";
				player->Go(args);
			}
			else if(Same(args[0], "back") || Same(args[0], "b"))
			{
				(args.size() == 1) ? args.push_back("back") : args[1] = "back";
				player->Go(args);
			}
			else if(Same(args[0], "right") || Same(args[0], "r"))
			{
				(args.size() == 1) ? args.push_back("right") : args[1] = "right";
				player->Go(args);
			}
			else if(Same(args[0], "left") || Same(args[0], "l"))
			{
				(args.size() == 1) ? args.push_back("left") : args[1] = "left";
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
            else if (Same(args[0], "open") || Same(args[0], "op"))
            {
                player->Open(args);
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
            else if (Same(args[0], "enter") || Same(args[0], "in"))
            {
                player->Solve(args);
            }
            else if (Same(args[0], "action") || Same(args[0], "do"))
            {
                player->Solve(args);
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