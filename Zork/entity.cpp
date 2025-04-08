#include "entity.h"
#include <iostream>

Entity::Entity(const std::string& name, const std::string& description, Entity* parent)
    : name(name), description(description), parent(parent)
{
    type = BASE;
    if (parent)
        parent->children.push_back(this);
}

Entity::~Entity() {
}

void Entity::Look() const {
    std::cout << name << "\n" << description << "\n";
}

void Entity::Update() {
}

void Entity::SetParent(Entity* newParent) {
    if (parent)
        parent->children.remove(this);
    parent = newParent;
    if (parent)
        parent->children.push_back(this);
}

bool Entity::HasChild(Entity* child) const {
    for (auto c : children)
        if (c == child)
            return true;
    return false;
}

Entity* Entity::FindChild(EntityType typeToFind) const {
    for (auto child : children)
        if (child->type == typeToFind)
            return child;
    return nullptr;
}

Entity* Entity::FindChild(const std::string& childName, EntityType typeToFind) const {
    for (auto child : children)
        if (child->type == typeToFind && child->name == childName)
            return child;
    return nullptr;
}

void Entity::FindAllChildren(EntityType typeToFind, std::list<Entity*>& foundChildren) const {
    for (auto child : children)
        if (child->type == typeToFind)
            foundChildren.push_back(child);
}
