#ifndef EXIT_H
#define EXIT_H
#include "key.h"
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

    void Open();   // Abre sin llave (para cofre, por ejemplo)
    void Close();
    void Lock();

    // Nuevos métodos que requieren llave
    void OpenWithKey(const Key* key);
    void CloseWithKey(const Key* key);

private:
    Room* source;
    Room* destination;
    ExitState state;
};

#endif // EXIT_H
