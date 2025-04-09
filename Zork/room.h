#ifndef ROOM_H
#define ROOM_H

#include "entity.h"
#include <map>
#include <vector>
#include <string>

class Room : public Entity {
public:
    Room(const std::string& name, const std::string& description, Entity* parent = nullptr);

    // Agrega una entidad a la direcci�n indicada ("north", "south", etc.)
    void AddEntity(const std::string& direction, Entity* entity);
    // Elimina una entidad de la direcci�n indicada
    void RemoveEntity(const std::string& direction, Entity* entity);

    // Devuelve una copia del vector de entidades en la direcci�n indicada
    std::vector<Entity*> GetEntities(const std::string& direction) const;

    // Verifica si la direcci�n existe en la habitaci�n.
    bool HasDirection(const std::string& direction) const;

    // Muestra el nombre y la descripci�n del room y, para cada direcci�n, 
    // genera el listado de entidades usando "and" entre ellas.
    virtual void Look() const override;

private:
    // Cada direcci�n (center, north, south, east, west) tendr� un vector de entidades.
    std::map<std::string, std::vector<Entity*>> directions;
};

#endif // ROOM_H
