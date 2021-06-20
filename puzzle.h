#ifndef __Puzzle__
#define __Puzzle__

#include "entity.h"
#include "globals.h"

class Room;
class Exit;

class Puzzle : public Entity
{
public:
    Puzzle(const char* name, const char* description, Room* parent, std::string password, Exit* exit);
    virtual ~Puzzle();

    bool EnterPassword(std::string password);

private:
    std::string password_;
    Exit* exit_to_open_;
};

#endif // __Puzzle__
