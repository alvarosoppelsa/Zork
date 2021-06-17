#ifndef __Item__
#define __Item__

#include <string>
#include "entity.h"

class Room;

enum ItemType
{
    COMMON,
    WEAPON,
	TOOL
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

    bool isHidden() const { return hidden_; }
    void hide(bool hidden) { hidden_ = hidden; }
private:
	int min_value_;
	int max_value_;
	ItemType item_type_;
    bool hidden_;
};

#endif //__Item__