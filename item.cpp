#include <iostream>
#include "room.h"
#include "exit.h"
#include "globals.h"
#include "item.h"

// ----------------------------------------------------
Item::Item(const char* title, const char* description, Entity* parent, ItemType item_type) :
Entity(title, description, parent), item_type(item_type)
{
	type = ITEM;
	min_value = max_value = 0;
}

// ----------------------------------------------------
Item::~Item()
{}

// ----------------------------------------------------
void Item::Look() const
{
	std::cout << "\n" << name << "\n";
	std::cout << description << "\n";

	std::list<Entity*> stuff;
	FindAll(ITEM, stuff);

	if(stuff.size() > 0)
	{
		std::cout << "It contains: " << "\n";
		for(std::list<Entity*>::const_iterator it = stuff.begin(); it != stuff.cend(); ++it)
			std::cout << (*it)->name << "\n";
	}
}

// ----------------------------------------------------
int Item::GetValue() const
{
	return Roll(min_value, max_value);
}