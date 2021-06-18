#include <iostream>
#include "globals.h"
#include "room.h"
#include "exit.h"
#include "item.h"
#include "player.h"

// ----------------------------------------------------
Player::Player(const char* title, const char* description, Room* room) :
Creature(title, description, room)
{
	this->setType(PLAYER);
}

// ----------------------------------------------------
Player::~Player()
{
}

// ----------------------------------------------------
void Player::Look(const std::vector<std::string>& args) const
{
	if(args.size() > 1)
	{
		for(std::list<Entity*>::const_iterator it = 
            getParentRef()->getContainerRef().begin(); it != getParentRef()->getContainerRef().cend(); ++it)
		{
			if(Same((*it)->getName(), args[1]) || 
                ((*it)->getType() == EXIT && Same(args[1], ((Exit*)(*it))->GetNameFrom((Room*)getParentRef()))))
			{
				(*it)->Look();
				return;
			}
		}

		if(Same(args[1], "me"))
		{
			std::cout << "\n" << this->getName() << "\n";
			std::cout << this->getDescription() << "\n";
		}
	}
	else
	{
		getParentRef()->Look();
	}
}

// ----------------------------------------------------
bool Player::Go(const std::vector<std::string>& args)
{
	Exit* exit = GetRoom()->GetExit(args[1]);

	if(exit == NULL)
	{
		std::cout << "\nThere is no exit at '" << args[1] << "'.\n";
		return false;
	}

	if(exit->isLocked())
	{
		std::cout << "\nThat exit is locked.\n";
		return false;
	}

	std::cout << "\nYou take direction " << exit->GetNameFrom((Room*)getParentRef()) << "...\n";
	ChangeParentTo(exit->GetDestinationFrom((Room*)getParentRef()));
    getParentRef()->Look();

	return true;
}


// ----------------------------------------------------
bool Player::Take(const std::vector<std::string>& args)
{
	if(args.size() == 4)
	{
		Item* item = (Item*)getParentRef()->Find(args[3], ITEM);

		// we could pick something from a container in our inventory ...
		if(item == NULL)
			item = (Item*)Find(args[3], ITEM);

		if(item == NULL)
		{
			std::cout << "\nCannot find '" << args[3] << "' in this room or in your inventory.\n";
			return false;
		}

		Item* subitem = (Item*)item->Find(args[1], ITEM);

		if(subitem == NULL)
		{
			std::cout << "\n" << item->getName() << " does not contain '" << args[1] << "'.\n";
			return false;
		}

		std::cout << "\nYou take " << subitem->getName() << " from " << item->getName() << ".\n";
		subitem->ChangeParentTo(this);
	}
	else if(args.size() == 2)
	{
		Item* item = (Item*)getParentRef()->Find(args[1], ITEM);

		if(item == NULL)
		{
			std::cout << "\nThere is no item here with that name.\n";
			return false;
		}

		std::cout << "\nYou take " << item->getName() << ".\n";
		item->ChangeParentTo(this);
	}

	return false;
}

// ----------------------------------------------------
void Player::Inventory() const
{
	std::list<Entity*> items;
	FindAll(ITEM, items);

	if(items.size() == 0)
	{
		std::cout << "\nYou do not own any item.\n";
		return;
	}

	for(std::list<Entity*>::const_iterator it = items.begin(); it != items.cend(); ++it)
	{
		if(*it == weapon)
			std::cout << "\n" << (*it)->getName() << " (as weapon)";
		else if(*it == armour)
			std::cout << "\n" << (*it)->getName() << " (as armour)";
		else
			std::cout << "\n" << (*it)->getName();
	}

	std::cout << "\n";
}

// ----------------------------------------------------
bool Player::Drop(const std::vector<std::string>& args)
{
	if(args.size() == 2)
	{
		Item* item = (Item*)Find(args[1], ITEM);

		if(item == NULL)
		{
			std::cout << "\nThere is no item on you with that name.\n";
			return false;
		}

		std::cout << "\nYou drop " << item->getName() << "...\n";
		item->ChangeParentTo(getParentRef());

		return true;
	}
	else if(args.size() == 4)
	{
		Item* item = (Item*)Find(args[1], ITEM);

		if(item == NULL)
		{
			std::cout << "\nCan not find '" << args[1] << "' in your inventory.\n";
			return false;
		}

		Item* container = (Item*)getParentRef()->Find(args[3], ITEM);

		if(container == NULL)
		{
			container = (Item*)Find(args[3], ITEM);
			std::cout << "\nCan not find '" << args[3] << "' in your inventory or in the room.\n";
			return false;
		}

		std::cout << "\nYou put " << item->getName() << " into " << container->getName() << ".\n";
		item->ChangeParentTo(container);

		return true;
	}
	
	return false;
}

// ----------------------------------------------------
bool Player::Equip(const std::vector<std::string>& args)
{
	Item* item = (Item*)Find(args[1], ITEM);

	if(item == NULL)
	{
		std::cout << "\nCannot find '" << args[1] << "' is not in your inventory.\n";
		return false;
	}

	switch(item->getItemType())
	{
	case WEAPON:
		weapon = item;
		break;

	case TOOL:
		armour = item;
		break;

    case BAG:
        armour = item;
        break;

	default:
		std::cout << "\n" << item->getName() << " cannot be equipped.\n";
		return false;
	}
		
	std::cout << "\nYou equip " << item->getName() << "...\n";

	return true;
}

// ----------------------------------------------------
bool Player::UnEquip(const std::vector<std::string>& args)
{
	if(!IsAlive())
		return false;

	Item* item = (Item*)Find(args[1], ITEM);

	if(item == NULL)
	{
		std::cout << "\n" << item->getName() << " is not in your inventory.\n";
		return false;
	}

	if(item == weapon)
		weapon = NULL;
	else if(item == armour)
		armour = NULL;
	else
	{
		std::cout << "\n" << item->getName() << " is not equipped.\n";
		return false;
	}

	std::cout << "\nYou un-equip " << item->getName() << "...\n";

	return true;
}

// ----------------------------------------------------
bool Player::Examine(const std::vector<std::string>& args) const
{
	Creature *target = (Creature*)getParentRef()->Find(args[1], CREATURE);

	if(target == NULL)
	{
		std::cout << "\n" << args[1] << " is not here.\n";
		return false;
	}

	target->Inventory();
	target->Stats();

	return true;
}

// ----------------------------------------------------
bool Player::Attack(const std::vector<std::string>& args)
{
	Creature *target = (Creature*)getParentRef()->Find(args[1], CREATURE);

	if(target == NULL)
	{
		std::cout << "\n" << args[1] << " is not here.";
		return false;
	}

	combat_target = target;
	std::cout << "\nYou jump to attack " << target->getName() << "!\n";
	return true;
}

// ----------------------------------------------------
bool Player::Loot(const std::vector<std::string>& args)
{
	Creature *target = (Creature*)getParentRef()->Find(args[1], CREATURE);

	if(target == NULL)
	{
		std::cout << "\n" << args[1] << " is not here.\n";
		return false;
	}

	if(target->IsAlive() == true)
	{
		std::cout << "\n" << target->getName() << " cannot be looted until it is killed.\n";
		return false;
	}

	std::list<Entity*> items;
	target->FindAll(ITEM, items);

	if(items.size() > 0)
	{
		std::cout << "\nYou loot " << target->getName() << "'s corpse:\n";

		for(std::list<Entity*>::const_iterator it = items.begin(); it != items.cend(); ++it)
		{
			Item* i = (Item*)(*it);
			std::cout << "You find: " << i->getName() << "\n";
			i->ChangeParentTo(this);
		}
	}
	else
		std::cout << "\nYou loot " << target->getName() << "'s corpse, but find nothing there.\n";

	return true;
}

// ----------------------------------------------------
bool Player::Lock(const std::vector<std::string>& args)
{
	if(!IsAlive())
		return false;

	Exit* exit = GetRoom()->GetExit(args[1]);

	if(exit == NULL)
	{
		std::cout << "\nThere is no exit at '" << args[1] << "'.\n";
		return false;
	}

	if(exit->isLocked())
	{
		std::cout << "\nThat exit is already locked.\n";
		return false;
	}

	Item* item = (Item*)Find(args[3], ITEM);

	if(item == NULL)
	{
		std::cout << "\nItem '" << args[3] << "' not found in your inventory.\n";
		return false;
	}

	if(exit->getKey() != item)
	{
		std::cout << "\nItem '" << item->getName() << "' is not the key for " << exit->GetNameFrom((Room*)getParentRef()) << ".\n";
		return false;
	}

	std::cout << "\nYou lock " << exit->GetNameFrom((Room*)getParentRef()) << "...\n";

	exit->lock();

	return true;
}

// ----------------------------------------------------
bool Player::UnLock(const std::vector<std::string>& args)
{
	if(!IsAlive())
		return false;

	Exit* exit = GetRoom()->GetExit(args[1]);

	if(exit == NULL)
	{
		std::cout << "\nThere is no exit at '" << args[1] << "'.\n";
		return false;
	}

	if(!exit->isLocked())
	{
		std::cout << "\nThat exit is not locked.\n";
		return false;
	}

	Item* item = (Item*)Find(args[3], ITEM);

	if(item == NULL)
	{
		std::cout << "\nKey '" << args[3] << "' not found in your inventory.\n";
		return false;
	}

	if(exit->getKey() != item)
	{
		std::cout << "\nKey '" << item->getName() << "' is not the key for " << exit->GetNameFrom((Room*)getParentRef()) << ".\n";
		return false;
	}

	std::cout << "\nYou unlock " << exit->GetNameFrom((Room*)getParentRef()) << "...\n";

	exit->unlock();

	return true;
}