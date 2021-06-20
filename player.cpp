#include <iostream>
#include "globals.h"
#include "room.h"
#include "exit.h"
#include "item.h"
#include "player.h"
#include "puzzle.h"

// ----------------------------------------------------
Player::Player(const char* title, const char* description, Room* room) :
Creature(title, description, room)
{
	type = PLAYER;
    item_taken_ = NULL;
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
		for(std::list<Entity*>::const_iterator it = parent->container.begin(); it != parent->container.cend(); ++it)
		{
			if(Same((*it)->name, args[1]) || ((*it)->type == EXIT && Same(args[1], ((Exit*)(*it))->GetNameFrom((Room*)parent))))
			{
				(*it)->Look();
				return;
			}
		}

        for (auto it : container)
        {
            if (Same(it->name, args[1]))
            {
                it->Look();
                return;
            }
        }

		if(Same(args[1], "me"))
		{
			std::cout << "\n" << name << "\n";
			std::cout << description << "\n";
		}
	}
    else if (item_taken_ != NULL && item_taken_->getItemType() == LIGHT)
    {
        GetRoom()->LookFull();
    }
	else
	{
		parent->Look();
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

	std::cout << "\nYou take direction " << exit->GetNameFrom((Room*) parent) << "...\n";
	ChangeParentTo(exit->GetDestinationFrom((Room*) parent));

    if (item_taken_ != NULL && item_taken_->getItemType() == LIGHT)
    {
        GetRoom()->LookFull();
    }
    else
    {
        parent->Look();
    }
    return true;
}


// ----------------------------------------------------
bool Player::Take(const std::vector<std::string>& args)
{
	if(args.size() == 4)
	{
		Item* item = (Item*)parent->Find(args[3], ITEM);

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
			std::cout << "\n" << item->name << " does not contain '" << args[1] << "'.\n";
			return false;
		}

		std::cout << "\nYou take " << subitem->name << " from " << item->name << ".\n";
		subitem->ChangeParentTo(this);
        item_taken_ = item;
	}
	else if(args.size() == 2)
	{
		Item* item = (Item*)parent->Find(args[1], ITEM);

		if(item == NULL)
		{
			std::cout << "\nThere is no item here with that name.\n";
			return false;
		}

		std::cout << "\nYou take " << item->name << ".\n";
		item->ChangeParentTo(this);
        item_taken_ = item;
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
			std::cout << "\n" << (*it)->name << " (as weapon)";
		else if(*it == bag)
			std::cout << "\n" << (*it)->name << " (as bag)";
		else
			std::cout << "\n" << (*it)->name;
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

		std::cout << "\nYou drop " << item->name << "...\n";
        item_taken_ = NULL;
		item->ChangeParentTo(parent);

		return true;
	}
	else if(args.size() == 4)
	{
		Item* item = (Item*)Find(args[1], ITEM);

		if(item == NULL)
		{
			std::cout << "\nCannot find '" << args[1] << "' in your inventory.\n";
			return false;
		}

		Item* container = (Item*)parent->Find(args[3], ITEM);

		if(container == NULL)
		{
			container = (Item*)Find(args[3], ITEM);
            if (container == NULL)
            {
                std::cout << "\nCannot find '" << args[3] << "' in your inventory or in the room.\n";
                return false;
            }
            else if (container->getItemType() != BAG)
            {
                std::cout << "\nAm I thinking to store a '" << args[1] << "' in a " << args[3] 
                          << "?\n I'm losing my mind...\n";
                return false;
            }
		}

		std::cout << "\nYou put " << item->name << " into " << container->name << ".\n";
        item_taken_ = NULL;
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
    case BAG:
         bag = item;
         break;
	default:
		std::cout << "\n" << item->name << " cannot be equipped.\n";
		return false;
	}
		
	std::cout << "\nYou equip " << item->name << "...\n";

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
		std::cout << "\n" << item->name << " is not in your inventory.\n";
		return false;
	}

	if(item == weapon)
		weapon = NULL;
	else if(item == bag)
		bag = NULL;
	else
	{
		std::cout << "\n" << item->name << " is not equipped.\n";
		return false;
	}

	std::cout << "\nYou un-equip " << item->name << "...\n";

	return true;
}

// ----------------------------------------------------
bool Player::Examine(const std::vector<std::string>& args) const
{
	Creature *target = (Creature*)parent->Find(args[1], CREATURE);

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
	Creature *target = (Creature*)parent->Find(args[1], CREATURE);

	if(target == NULL)
	{
		std::cout << "\n" << args[1] << " is not here.";
		return false;
	}

	combat_target = target;
	std::cout << "\nYou jump to attack " << target->name << "!\n";
	return true;
}

// ----------------------------------------------------
bool Player::Loot(const std::vector<std::string>& args)
{
	Creature *target = (Creature*)parent->Find(args[1], CREATURE);

	if(target == NULL)
	{
		std::cout << "\n" << args[1] << " is not here.\n";
		return false;
	}

	if(target->IsAlive() == true)
	{
		std::cout << "\n" << target->name << " cannot be looted until it is killed.\n";
		return false;
	}

	std::list<Entity*> items;
	target->FindAll(ITEM, items);

	if(items.size() > 0)
	{
		std::cout << "\nYou loot " << target->name << "'s corpse:\n";

		for(std::list<Entity*>::const_iterator it = items.begin(); it != items.cend(); ++it)
		{
			Item* i = (Item*)(*it);
			std::cout << "You find: " << i->name << "\n";
			i->ChangeParentTo(this);
		}
	}
	else
		std::cout << "\nYou loot " << target->name << "'s corpse, but find nothing there.\n";

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
		std::cout << "\nItem '" << item->name << "' is not the key for " << exit->GetNameFrom((Room*)parent) << ".\n";
		return false;
	}

	std::cout << "\nYou lock " << exit->GetNameFrom((Room*)parent) << "...\n";

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
		std::cout << "\nKey '" << item->name << "' is not the key for " << exit->GetNameFrom((Room*)parent) << ".\n";
		return false;
	}

	std::cout << "\nYou unlock " << exit->GetNameFrom((Room*)parent) << "...\n";

	exit->unlock();

	return true;
}

bool Player::Enter(const std::vector<std::string>& args)
{
    Puzzle* puzzle = (Puzzle*)parent->Find(args[3], PUZZLE);
    if (!parent)
    {
        std::cout << "\nThere isn't such a " << args[3] << ".\n";
        return false;
    }
    if (!puzzle->EnterPassword(args[1]))
    {
        std::cout << "\nI can't believe I forgot it again! \nMust be written somewere...\n";
        return false;
    }
    std::cout << "\nAlright!\n";
    return true;
}
