#include "puzzle.h"
#include "exit.h"

Puzzle::Puzzle(const char* name, const char* description, Room* parent, std::string password, Exit* exit) :
Entity(name, description, (Entity *)parent), password_(password), exit_to_open_(exit)
{
    type = PUZZLE;
}

Puzzle::~Puzzle()
{
}

bool Puzzle::CheckPassword(std::string password)
{
    if (Same(password, password_))
    {
        exit_to_open_->unlock();
        return true;
    }
    return false;
}
