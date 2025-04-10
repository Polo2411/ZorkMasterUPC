#ifndef ROOM_H
#define ROOM_H

#include "entity.h"
#include <map>
#include <vector>
#include <string>

class Room : public Entity {
public:
    Room(const std::string& name, const std::string& description, Entity* parent = nullptr);

    void AddEntity(const std::string& direction, Entity* entity);
    void RemoveEntity(const std::string& direction, Entity* entity);
    std::vector<Entity*> GetEntities(const std::string& direction) const;
    bool HasDirection(const std::string& direction) const;
    virtual void Look() const override;

    // NUEVO: devolvemos una referencia const al mapa de direcciones
    const std::map<std::string, std::vector<Entity*>>& GetDirections() const;

private:
    std::map<std::string, std::vector<Entity*>> directions;
};

#endif // ROOM_H
