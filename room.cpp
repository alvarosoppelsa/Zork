#include <iostream>
#include "globals.h"
#include "exit.h"
#include "item.h"
#include "creature.h"
#include "room.h"

// ----------------------------------------------------
Room::Room(const char* title, const char* description, bool darkness) :
Entity(title, description, NULL)
{
	this->setType(ROOM);
    darkness_ = darkness;
}

// ----------------------------------------------------
Room::~Room()
{
}

// ----------------------------------------------------
void Room::Look() const
{
	std::cout << "\n" << getName() << "\n";
	std::cout << getDescription();

	// List exits --
	for(std::list<Entity*>::const_iterator it = getContainerRef().begin(); it != getContainerRef().cend(); ++it)
	{
		if((*it)->getType() == EXIT)
		{
			Exit* ex = (Exit*)*it;
			std::cout << "\nDirection (" << ex->GetNameFrom(this) << ") you see " << ex->GetDestinationFrom(this)->getName();
		}
	}

	// List items --
    for(std::list<Entity*>::const_iterator it = getContainerRef().begin(); it != getContainerRef().cend(); ++it)
	{
        if (darkness_) break;

		if((*it)->getType() == ITEM)
		{
			Item* item = (Item*)*it;
			std::cout << "\nThere is an item here: " << item->getName();
		}
	}

	// List creatures --
	for(std::list<Entity*>::const_iterator it = getContainerRef().begin(); it != getContainerRef().cend(); ++it)
	{
		if((*it)->getType() == CREATURE)
		{
			Creature* cr = (Creature*)*it;
			std::cout << "\nThere is someone else here: " << cr->getName();
			if(cr->IsAlive() == false)
				std::cout << " (dead)";
		}
	}

	std::cout << "\n";
}

// ----------------------------------------------------
Exit* Room::GetExit(const std::string& direction) const
{
	for(std::list<Entity*>::const_iterator it = getContainerRef().begin(); it != getContainerRef().cend(); ++it)
	{
		if((*it)->getType() == EXIT)
		{
			Exit* ex = (Exit*) *it;
			if(Same(ex->GetNameFrom(this), direction))
				return ex;
		}
	}

	return NULL;
}


