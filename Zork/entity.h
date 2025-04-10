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

    Entity(const std::string& name, const std::string& description, Entity* parent = nullptr);
    virtual ~Entity();


    virtual void Look() const;

    virtual void Update();


    void SetParent(Entity* newParent);


    bool HasChild(Entity* child) const;

    Entity* FindChild(EntityType type) const;

    Entity* FindChild(const std::string& childName, EntityType type) const;

    void FindAllChildren(EntityType type, std::list<Entity*>& foundChildren) const;

public:
    EntityType type;
    std::string name;
    std::string description;
    Entity* parent;
    std::list<Entity*> children;
};

#endif // ENTITY_H
