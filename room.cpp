#include <iostream>
#include "globals.h"
#include "exit.h"
#include "item.h"
#include "creature.h"
#include "room.h"
#include "puzzle.h"

// ----------------------------------------------------
Room::Room(const char* title, const char* description, bool darkness) :
Entity(title, description, NULL)
{
	type = ROOM;
    darkness_ = darkness;
}

// ----------------------------------------------------
Room::~Room()
{
}

// ----------------------------------------------------
void Room::Look() const
{
	std::cout << "\n" << name << "\n";
	std::cout << description;

    for (auto it : container)
    {
        switch (it->type)
        {
        case EXIT:
        {
            Exit* ex = (Exit*)it;
            std::cout << "\nDirection (" << ex->GetNameFrom(this) << ") you see " << ex->GetDestinationFrom(this)->getName();
            break;
        }
        case ITEM:
        {
            if (darkness_) break;
            Item* item = (Item*)it;
            std::cout << "\nThere is an item here: " << item->getName();
            break;
        }
        case PUZZLE:
        {
            Puzzle* puzzle = (Puzzle *)it;
            std::cout << "\nThere is a " << puzzle->getName();
            break;
        }
        case CREATURE:
        {
            Creature* cr = (Creature*)it;
            std::cout << "\nThere is someone else here: " << cr->getName();
            if (cr->IsAlive() == false)
                std::cout << " (dead)";
            break;
        }
        default:
            break;
        }
    }
	// List exits --
	for(std::list<Entity*>::const_iterator it = container.begin(); it != container.cend(); ++it)
	{
		if((*it)->type == EXIT)
		{

		}
	}

	// List items --
    for(std::list<Entity*>::const_iterator it = container.begin(); it != container.cend(); ++it)
	{
        if (darkness_) break;

		if((*it)->type == ITEM)
		{

		}
	}

    // List puzzles --

	// List creatures --
	for(std::list<Entity*>::const_iterator it = container.begin(); it != container.cend(); ++it)
	{
		if((*it)->type == CREATURE)
		{
		}
	}

	std::cout << "\n";
}

void Room::LookFull()
{
    bool prev_state = darkness_;
    darkness_ = false;
    Look();
    darkness_ = prev_state;
}

// ----------------------------------------------------
Exit* Room::GetExit(const std::string& direction) const
{
	for(std::list<Entity*>::const_iterator it = container.begin(); it != container.cend(); ++it)
	{
		if((*it)->type == EXIT)
		{
			Exit* ex = (Exit*) *it;
			if(Same(ex->GetNameFrom(this), direction))
				return ex;
		}
	}

	return NULL;
}


