#ifndef ROOM_H
#define ROOM_H

#include "entity.h"
#include <map>
#include <vector>
#include <string>

class Room : public Entity {
public:
    Room(const std::string& name, const std::string& description, Entity* parent = nullptr);

    // Agrega una entidad a la dirección indicada ("north", "south", etc.)
    void AddEntity(const std::string& direction, Entity* entity);
    // Elimina una entidad de la dirección indicada
    void RemoveEntity(const std::string& direction, Entity* entity);

    // Devuelve una copia del vector de entidades en la dirección indicada
    std::vector<Entity*> GetEntities(const std::string& direction) const;

    // Verifica si la dirección existe en la habitación.
    bool HasDirection(const std::string& direction) const;

    // Muestra el nombre y la descripción del room y, para cada dirección, 
    // genera el listado de entidades usando "and" entre ellas.
    virtual void Look() const override;

private:
    // Cada dirección (center, north, south, east, west) tendrá un vector de entidades.
    std::map<std::string, std::vector<Entity*>> directions;
};

#endif // ROOM_H
