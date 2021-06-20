#include <iostream>
#include "globals.h"
#include "room.h"
#include "exit.h"
#include "item.h"
#include "creature.h"

// ----------------------------------------------------
Creature::Creature(const char* title, const char* description, Room* room) :
Entity(title, description, (Entity*)room)
{
	type = CREATURE;
	hit_points_ = 1;
	min_damage = max_damage = min_protection = max_protection = 0;
	weapon = bag = NULL;
	combat_target = NULL;
}

// ----------------------------------------------------
Creature::~Creature()
{}

// ----------------------------------------------------
void Creature::Look(const std::vector<std::string>& args) const
{
	if(IsAlive())
	{
		std::cout << "\n" << name << "\n";
		std::cout << description << "\n";
	}
	else
	{
		std::cout << "\n" << name << "'s corpse\n";
		std::cout << "\nHere lies dead: " << description << "\n";
	}
}

// ----------------------------------------------------
bool Creature::Go(const std::vector<std::string>& args)
{
	if(!IsAlive())
		return false;

	Exit* exit = GetRoom()->GetExit(args[1]);

	if(exit == NULL)
		return false;

	if(PlayerInRoom())
		std::cout << "\n" << name << "goes " << args[1] << "...\n";

	ChangeParentTo(exit->GetDestinationFrom((Room*) parent));

	return true;
}

// ----------------------------------------------------
bool Creature::Take(const std::vector<std::string>& args)
{
	if(!IsAlive())
		return false;

	Item* item = (Item*)parent->Find(args[1], ITEM);

	if(args.size() > 1)
	{
		// we could pick something from a container in our inventory ...
		if(item == NULL)
			item = (Item*)Find(args[1], ITEM);

		if(item == NULL)
			return false;

		Item* subitem = (Item*)item->Find(args[3], ITEM);

		if(subitem == NULL)
			return false;

		if(PlayerInRoom())
			std::cout << "\n" << name << " looks into " << item->name << "...\n";

		item = subitem;
	}

	if(item == NULL)
		return false;

	if(PlayerInRoom())
		std::cout << "\n" << name << " takes " << item->name << ".\n";

	item->ChangeParentTo(this);

	return true;
}

// ----------------------------------------------------
void Creature::Inventory() const
{
	std::list<Entity*> items;
	FindAll(ITEM, items);

	if(items.size() == 0)
	{
		std::cout << "\n" << name << " does not own any items\n";
		return;
	}

	std::cout << "\n" << name << " owns:\n";
	for(std::list<Entity*>::const_iterator it = items.begin(); it != items.cend(); ++it)
	{
		if(*it == weapon)
			std::cout << (*it)->name << " (as weapon)\n";
		else if(*it == bag)
			std::cout << (*it)->name << " (as bag)\n";
		else
			std::cout << (*it)->name << "\n";
	}
}

// ----------------------------------------------------
bool Creature::Equip(const std::vector<std::string>& args)
{
	if(!IsAlive())
		return false;

	Item* item = (Item*)Find(args[1], ITEM);

	if(item == NULL)
		return false;

	switch(item->type)
	{
		case WEAPON:
		weapon = item;
		break;

		default:
		return false;
	}

	if(PlayerInRoom())
		std::cout << name << " equips " << item->name << "...\n";

	return true;
}

// ----------------------------------------------------
bool Creature::UnEquip(const std::vector<std::string>& args)
{
	if(!IsAlive())
		return false;

	Item* item = (Item*)Find(args[1], ITEM);

	if(item == NULL)
		return false;

	if(item == weapon)
		weapon = NULL;
	else if(item == weapon)
		bag = NULL;
	else
		return false;

	if(PlayerInRoom())
		std::cout << name << " un-equips " << item->name << "...\n";

	return true;
}


// ----------------------------------------------------
bool Creature::AutoEquip()
{
	if(!IsAlive())
		return false;

	std::list<Entity*> items;
	FindAll(ITEM, items);

    for (std::list<Entity*>::const_iterator it = items.begin(); it != items.cend(); ++it)
    {
        Item* i = (Item*)(*it);

        if (i->getItemType() == WEAPON)
            weapon = i;
    }
	return true;
}

// ----------------------------------------------------
bool Creature::Lock(const std::vector<std::string>& args)
{
	if(!IsAlive())
		return false;

	Exit* exit = GetRoom()->GetExit(args[1]);

	if(exit == NULL || exit->isLocked())
		return false;

	Item* item = (Item*)Find(args[3], ITEM);

	if(item == NULL || exit->getKey() != item)
		return false;

	if(PlayerInRoom())
		std::cout << "\n" << name << "locks " << exit->GetNameFrom((Room*)parent) << "...\n";

	exit->lock();

	return true;
}

// ----------------------------------------------------
bool Creature::UnLock(const std::vector<std::string>& args)
{
	if(!IsAlive())
		return false;

	Exit* exit = GetRoom()->GetExit(args[1]);

	if(exit == NULL || !exit->isLocked())
		return false;

	Item* item = (Item*)Find(args[3], ITEM);

	if(item == NULL || exit->getKey() != item)
		return false;

	if(PlayerInRoom())
		std::cout << "\n" << name << "unlocks " << exit->GetNameFrom((Room*) parent) << "...\n";

	exit->unlock();

	return true;
}

// ----------------------------------------------------
bool Creature::Drop(const std::vector<std::string>& args)
{
	if(!IsAlive())
		return false;

	Item* item = (Item*)Find(args[1], ITEM);

	if(item == NULL)
		return false;

	if(PlayerInRoom())
		std::cout << name << " drops " << item->name << "...\n";
	
	item->ChangeParentTo(parent);

	return true;
}

// ----------------------------------------------------
Room* Creature::GetRoom() const
{
	return (Room*)parent;
}

// ----------------------------------------------------
bool Creature::PlayerInRoom() const
{
	return parent->Find(PLAYER) != NULL;
}

// ----------------------------------------------------
bool Creature::IsAlive() const
{
	return hit_points_ > 0;
}

// ----------------------------------------------------
void Creature::Tick()
{
	if(combat_target != NULL)
	{
		if(parent->Find(combat_target) == true)
			MakeAttack();
		else
			combat_target = NULL;
	}
}

// ----------------------------------------------------
bool Creature::Attack(const std::vector<std::string>& args)
{
	Creature *target = (Creature*)parent->Find(args[1], CREATURE);

	if(target == NULL)
		return false;

	combat_target = target;
	std::cout << "\n" << name << " attacks " << target->name << "!\n";
	return true;
}

// ----------------------------------------------------
int Creature::MakeAttack()
{
	if(!IsAlive() || !combat_target->IsAlive())
	{
		combat_target = combat_target->combat_target = NULL;
		return false;
	}

	int result = (weapon) ? weapon->GetValue() : Roll(min_damage, max_damage);

	if(PlayerInRoom())
		std::cout << name << " attacks " << combat_target->name << " for " << result << "\n";

	combat_target->ReceiveAttack(result);

	// make the attacker react and take me as a target
	if(combat_target->combat_target == NULL)
		combat_target->combat_target = this;

	return result;
}

// ----------------------------------------------------
int Creature::ReceiveAttack(int damage)
{
	
	int received = damage;

	hit_points_ -= received;

	if(PlayerInRoom())
		std::cout << name << " is hit for " << received << " damage\n";

	if(IsAlive() == false)
		Die();

	return received;
}

// ----------------------------------------------------
void Creature::Die()
{
	if(PlayerInRoom())
		std::cout << name << " dies.\n";
}

// ----------------------------------------------------
void Creature::Stats() const
{
	std::cout << "\nHit Points: " << hit_points_;
	std::cout << "\nAttack: (" << ((weapon) ? weapon->name : "no weapon") << ") ";
	std::cout << ((weapon) ? weapon->getMinValue() : min_damage) << "-" << ((weapon) ? weapon->getMaxValue() : max_damage);
	std::cout << "\n";
}