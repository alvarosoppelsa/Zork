#ifndef __Item__
#define __Item__

#include <string>
#include "entity.h"

class Room;

enum ItemType
{
    COMMON,
    WEAPON,
	TOOL,
    BAG, 
    LIGHT
};

class Item : public Entity
{
public:
	Item(const char* name, const char* description, Entity* parent, ItemType item_type = COMMON, bool hidden_ = false);
	~Item();

	void Look() const;
	int GetValue() const;

    ItemType getItemType() const { return item_type_; }
    void setItemType(ItemType item_type) { item_type_ = item_type; }
    int getMinValue() const { return min_value_; }
    int getMaxValue() const { return max_value_; }
    void setMinValue(int new_value) { min_value_ = new_value; }
    void setMaxValue(int new_value) { max_value_ = new_value; }

private:
	int min_value_;
	int max_value_;
	ItemType item_type_;
};

#endif //__Item__