#include <iostream>
#include "globals.h"
#include "exit.h"
#include "item.h"
#include "creature.h"
#include "room.h"

// ----------------------------------------------------
Room::Room(const char* title, const char* description) :
Entity(title, description, NULL)
{
	type = ROOM;
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

	// List exits --
	for(std::list<Entity*>::const_iterator it = container.begin(); it != container.cend(); ++it)
	{
		if((*it)->type == EXIT)
		{
			Exit* ex = (Exit*)*it;
			std::cout << "\nDirection (" << ex->GetNameFrom(this) << ") you see " << ex->GetDestinationFrom(this)->name;
		}
	}

	// List items --
	for(std::list<Entity*>::const_iterator it = container.begin(); it != container.cend(); ++it)
	{
		if((*it)->type == ITEM)
		{
			Item* item = (Item*)*it;
			std::cout << "\nThere is an item here: " << item->name;
		}
	}

	// List creatures --
	for(std::list<Entity*>::const_iterator it = container.begin(); it != container.cend(); ++it)
	{
		if((*it)->type == CREATURE)
		{
			Creature* cr = (Creature*)*it;
			std::cout << "\nThere is someone else here: " << cr->name;
			if(cr->IsAlive() == false)
				std::cout << " (dead)";
		}
	}

	std::cout << "\n";
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


