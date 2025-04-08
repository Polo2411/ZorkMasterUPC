#ifndef EXIT_H
#define EXIT_H

#include "entity.h"
#include "room.h"

enum ExitState { OPEN, CLOSED, LOCKED };

class Exit : public Entity {
public:
    Exit(const std::string& name, const std::string& description, Room* source, Room* destination);
    virtual ~Exit() = default;

    Room* GetSource() const;
    Room* GetDestination() const;
    ExitState GetState() const;
    void SetState(ExitState newState);

    // Maybe an open/close method if needed
    void Open();
    void Close();
    void Lock();

private:
    Room* source;
    Room* destination;
    ExitState state;
};

#endif // EXIT_H
