#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <list>

// Definición de tipos de entidad usando nombres comunes
enum EntityType {
    BASE,    // Entidad base
    ROOM,
    EXIT,
    ITEM,
    CREATURE,
    PLAYER
};

class Entity {
public:
    // Constructor. Se recibe nombre, descripción y un puntero opcional al padre.
    Entity(const std::string& name, const std::string& description, Entity* parent = nullptr);
    // Destructor virtual para permitir la herencia.
    virtual ~Entity();

    // Función que imprime la información de la entidad.
    virtual void Look() const;
    // Método de actualización, para que las subclases puedan definir su comportamiento.
    virtual void Update();

    // Cambia el padre de la entidad, ajustando la lista de hijos adecuadamente.
    void SetParent(Entity* newParent);

    // Devuelve true si el objeto pasado se encuentra en la lista de hijos.
    bool HasChild(Entity* child) const;

    // Busca y devuelve el primer hijo que tenga el tipo indicado.
    Entity* FindChild(EntityType type) const;

    // Busca y devuelve el primer hijo que coincida con el nombre y el tipo.
    Entity* FindChild(const std::string& childName, EntityType type) const;

    // Rellena la lista pasada con todos los hijos que sean del tipo indicado.
    void FindAllChildren(EntityType type, std::list<Entity*>& foundChildren) const;

public:
    EntityType type;           // El tipo de entidad (BASE, ROOM, etc.)
    std::string name;          // Nombre de la entidad.
    std::string description;   // Descripción.
    Entity* parent;            // Puntero a su entidad contenedora.
    std::list<Entity*> children; // Lista de subentidades.
};

#endif // ENTITY_H
