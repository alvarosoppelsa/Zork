#ifndef __Exit__
#define __Exit__

#include <string>
#include "entity.h"

class Room;

enum ExitType
{
    REGULAR,
    MUTABLE
};

class Exit : public Entity
{
public:
	Exit(const char* name, const char* opposite_name, const char* description, Room* origin,
         Room* destination, bool one_way = false, ExitType exit_type = REGULAR, std::string message = "");

    ~Exit();

	void Look() const;

	const std::string& GetNameFrom(const Room* room) const;
	Room* GetDestinationFrom(const Room* room) const;

    bool isLocked() { return locked_; }
    bool isClosed() { return closed_; }

    Entity*  getKey()         const { return key_;         }
    Room*    getDestination() const { return destination_; }
    ExitType getExitType()    const { return exit_type_;   }

    void unlock();
    void lock()   { locked_ = true; }
    void close()  { closed_ = true;  }
    void open()   { closed_ = false; }
    void setKey(Entity* key) { key_ = key; }

private :
	bool one_way_;
	bool closed_;
	bool locked_;
	std::string opposite_name_;
    std::string message_;
	Room* destination_;
	Entity* key_;
    ExitType exit_type_;

};

#endif //__Exit__