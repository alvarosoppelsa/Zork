#ifndef __Puzzle__
#define __Puzzle__

#include "entity.h"
#include "globals.h"

class Room;
class Exit;

enum PuzzleType
{
    ACTION,
    PASSWORD

};
class Puzzle : public Entity
{
public:
    Puzzle(const char* name, const char* description, Room* parent, std::string solution, Exit* exit, PuzzleType puzzle_type);
    virtual ~Puzzle();

    bool SolvePuzzle(std::string solution);
    bool EnterPassword(std::string password);
    bool Action(std::string action);

    PuzzleType getPuzzleType() const { return puzzle_type_; }
private:
    std::string solution_;
    Exit* exit_to_open_;
    PuzzleType puzzle_type_;
};

#endif // __Puzzle__
