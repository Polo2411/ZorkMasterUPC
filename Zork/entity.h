#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <list>

enum EntityType {
    BASE,
    ROOM,
    EXIT,
    ITEM,
    CREATURE,
    PLAYER
};

class Entity {
public:
    // Constructor: receives name, description and an optional parent
    Entity(const std::string& name, const std::string& description, Entity* parent = nullptr);
    virtual ~Entity();

    // Displays basic info (name and description)
    virtual void Look() const;
    // Update logic (empty by default)
    virtual void Update();

    // Changes the parent of this entity
    void SetParent(Entity* newParent);

    // Checks if a child is in the children list
    bool HasChild(Entity* child) const;
    // Returns the first child with the specified type
    Entity* FindChild(EntityType type) const;
    // Returns the first child that matches the given name and type
    Entity* FindChild(const std::string& childName, EntityType type) const;
    // Fills foundChildren with all children of the specified type
    void FindAllChildren(EntityType type, std::list<Entity*>& foundChildren) const;

public:
    EntityType type;
    std::string name;
    std::string description;
    Entity* parent;
    std::list<Entity*> children;
};

#endif // ENTITY_H
