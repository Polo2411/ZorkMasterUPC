// exit.h
#ifndef EXIT_H
#define EXIT_H

#include "entity.h"
#include "room.h"
#include "key.h"

enum ExitState { OPEN, CLOSED, LOCKED };

class Exit : public Entity {
public:
    Exit(const std::string& name, const std::string& description, Room* source, Room* destination);
    virtual ~Exit() = default;

    Room* GetSource() const;
    Room* GetDestination() const;
    ExitState GetState() const;
    void SetState(ExitState newState);

    // Devuelve la room a la que ir si estás en 'current'
    Room* GetDestinationFor(Room* current) const;

    void Open();
    void Close();
    void Lock();

    void OpenWithKey(const Key* key);
    void CloseWithKey(const Key* key);

private:
    Room* source;
    Room* destination;
    ExitState state;
};

#endif // EXIT_H
