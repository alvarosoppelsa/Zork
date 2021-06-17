#ifndef __Room__
#define __Room__

#include <string>
#include <list>
#include "entity.h"

class Exit;
class Item;

class Room : public Entity
{
public:
	Room(const char* name, const char* description, bool darkness = false);
	~Room();

	void Look() const;

	Exit* GetExit(const std::string& direction) const;

protected:
    void illuminate() { darkness_ = false; };
    void darken()     { darkness_ = true; }

private:
    bool darkness_;
};

#endif //__Room__