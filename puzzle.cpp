#include <iostream>

#include "puzzle.h"
#include "exit.h"

Puzzle::Puzzle(const char* name, const char* description, Room* parent, std::string solution, Exit* exit, PuzzleType puzzle_type) :
Entity(name, description, (Entity *)parent), solution_(solution), exit_to_open_(exit), puzzle_type_(puzzle_type)
{
    type = PUZZLE;
}

Puzzle::~Puzzle()
{
}

bool Puzzle::SolvePuzzle(std::string solution)
{
    bool ret;
    switch (puzzle_type_)
    {
    case ACTION:
        ret = Action(solution);
        break;
    case PASSWORD:
        ret = EnterPassword(solution);
        break;
    default:
        ret = false;
        break;
    }
    return ret;
}

bool Puzzle::EnterPassword(std::string password)
{
    if (Same(password, solution_))
    {
        exit_to_open_->unlock();
        std::cout << "\nAlright!\n";
        return true;
    }
    std::cout << "\nWrong password!\n";
    return false;
}

bool Puzzle::Action(std::string action)
{
    if (Same(action, solution_))
    {
        exit_to_open_->unlock();
        std::cout << "\nSeems I had success...\n";
        return true;
    }
    std::cout << "\nI cannot do '"<< action << "'\n";
    return false;
}
