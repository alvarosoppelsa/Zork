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
	setType(CREATURE);
	hit_points_ = 1;
	min_damage = max_damage = min_protection = max_protection = 0;
	weapon = armour = NULL;
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
		std::cout << getName() << "\n";
		std::cout << getDescription() << "\n";
	}
	else
	{
		std::cout << getName() << "'s corpse\n";
		std::cout << "Here lies dead: " << getDescription() << "\n";
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
		std::cout << getName() << "goes " << args[1] << "...\n";

	ChangeParentTo(exit->GetDestinationFrom((Room*) getParentRef()));

	return true;
}

// ----------------------------------------------------
bool Creature::Take(const std::vector<std::string>& args)
{
	if(!IsAlive())
		return false;

	Item* item = (Item*)getParentRef()->Find(args[1], ITEM);

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
			std::cout << getName() << " looks into " << item->getName() << "...\n";

		item = subitem;
	}

	if(item == NULL)
		return false;

	if(PlayerInRoom())
		std::cout << getName() << " takes " << item->getName() << ".\n";

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
		std::cout << getName() << " does not own any items\n";
		return;
	}

	std::cout << "\n" << getName() << " owns:\n";
	for(std::list<Entity*>::const_iterator it = items.begin(); it != items.cend(); ++it)
	{
		if(*it == weapon)
			std::cout << (*it)->getName() << " (as weapon)\n";
		else if(*it == armour)
			std::cout << (*it)->getName() << " (as armour)\n";
		else
			std::cout << (*it)->getName() << "\n";
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

	switch(item->getType())
	{
		case WEAPON:
		weapon = item;
		break;

		default:
		return false;
	}

	if(PlayerInRoom())
		std::cout << getName() << " equips " << item->getName() << "...\n";

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
		armour = NULL;
	else
		return false;

	if(PlayerInRoom())
		std::cout << getName() << " un-equips " << item->getName() << "...\n";

	return true;
}


// ----------------------------------------------------
bool Creature::AutoEquip()
{
	if(!IsAlive())
		return false;

	std::list<Entity*> items;
	FindAll(ITEM, items);

	for(std::list<Entity*>::const_iterator it = items.begin(); it != items.cend(); ++it)
	{
		Item* i = (Item*)(*it);

		if(i->getItemType() == WEAPON)
			weapon = i;
		if(i->getItemType() == TOOL)
			armour = i;
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
		std::cout << "\n" << getName() << "locks " << exit->GetNameFrom((Room*)getParentRef()) << "...\n";

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
		std::cout << "\n" << getName() << "unlocks " << exit->GetNameFrom((Room*)getParentRef()) << "...\n";

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
		std::cout << getName() << " drops " << item->getName() << "...\n";
	
	item->ChangeParentTo(getParentRef());

	return true;
}

// ----------------------------------------------------
Room* Creature::GetRoom() const
{
	return (Room*)getParentRef();
}

// ----------------------------------------------------
bool Creature::PlayerInRoom() const
{
	return getParentRef()->Find(PLAYER) != NULL;
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
		if(getParentRef()->Find(combat_target) == true)
			MakeAttack();
		else
			combat_target = NULL;
	}
}

// ----------------------------------------------------
bool Creature::Attack(const std::vector<std::string>& args)
{
	Creature *target = (Creature*)getParentRef()->Find(args[1], CREATURE);

	if(target == NULL)
		return false;

	combat_target = target;
	std::cout << "\n" << getName() << " attacks " << target->getName() << "!\n";
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
		std::cout << getName() << " attacks " << combat_target->getName() << " for " << result << "\n";

	combat_target->ReceiveAttack(result);

	// make the attacker react and take me as a target
	if(combat_target->combat_target == NULL)
		combat_target->combat_target = this;

	return result;
}

// ----------------------------------------------------
int Creature::ReceiveAttack(int damage)
{
	int prot = (armour) ? armour->GetValue() : Roll(min_protection, max_protection);
	int received = damage - prot;

	hit_points_ -= received;

	if(PlayerInRoom())
		std::cout << getName() << " is hit for " << received << " damage (" << prot << " blocked) \n";

	if(IsAlive() == false)
		Die();

	return received;
}

// ----------------------------------------------------
void Creature::Die()
{
	if(PlayerInRoom())
		std::cout << getName() << " dies.\n";
}

// ----------------------------------------------------
bool Creature::Loot(const std::vector<std::string>& args)
{
	Creature *target = (Creature*)getParentRef()->Find(args[1], CREATURE);

	if(target == NULL && target->IsAlive() == false)
		return false;

	std::list<Entity*> items;
	target->FindAll(ITEM, items);

	for(std::list<Entity*>::const_iterator it = items.begin(); it != items.cend(); ++it)
	{
		Item* i = (Item*)(*it);
		i->ChangeParentTo(this);
	}

	std::cout << "\n" << getName() << " loots " << target->getName() << "'s corpse\n";

	return true;
}

// ----------------------------------------------------
void Creature::Stats() const
{
	std::cout << "\nHit Points: " << hit_points_;
	std::cout << "\nAttack: (" << ((weapon) ? weapon->getName() : "no weapon") << ") ";
	std::cout << ((weapon) ? weapon->getMinValue() : min_damage) << "-" << ((weapon) ? weapon->getMaxValue() : max_damage);
	std::cout << "\nProtection: (" << ((armour) ? armour->getName() : "no armour") << ") ";
	std::cout << ((armour) ? armour->getMinValue() : min_protection) << "-" << ((armour) ? armour->getMaxValue() : max_protection);
	std::cout << "\n";
}