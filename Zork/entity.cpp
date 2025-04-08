#include "entity.h"
#include <iostream>

// Constructor: inicializa nombre, descripción y padre. Si se pasa un padre, se agrega a su lista de hijos.
Entity::Entity(const std::string& name, const std::string& description, Entity* parent)
    : name(name), description(description), parent(parent)
{
    type = BASE;  // Por defecto se asigna BASE. Las subclases deberán asignar su tipo propio.
    if (parent != nullptr) {
        parent->children.push_back(this);
    }
}

// Destructor virtual vacío (puedes agregar aquí la limpieza de memoria si fuera necesario).
Entity::~Entity() {
}

// Imprime el nombre y la descripción.
void Entity::Look() const {
    std::cout << name << "\n" << description << "\n";
}

// Método Update básico (por defecto no hace nada).
void Entity::Update() {
    // Este método puede ser redefinido en las subclases.
}

// Cambia el padre de la entidad. Se remueve del padre anterior y se agrega al nuevo.
void Entity::SetParent(Entity* newParent) {
    if (parent != nullptr) {
        parent->children.remove(this);
    }
    parent = newParent;
    if (parent != nullptr) {
        parent->children.push_back(this);
    }
}

// Devuelve true si 'child' se encuentra en la lista de children.
bool Entity::HasChild(Entity* child) const {
    for (auto ch : children) {
        if (ch == child)
            return true;
    }
    return false;
}

// Busca el primer hijo de un tipo específico.
Entity* Entity::FindChild(EntityType type) const {
    for (auto child : children) {
        if (child->type == type)
            return child;
    }
    return nullptr;
}

// Busca el primer hijo que coincida en nombre y tipo.
Entity* Entity::FindChild(const std::string& childName, EntityType type) const {
    for (auto child : children) {
        if (child->type == type && child->name == childName)
            return child;
    }
    return nullptr;
}

// Rellena la lista foundChildren con todos los hijos que sean del tipo indicado.
void Entity::FindAllChildren(EntityType type, std::list<Entity*>& foundChildren) const {
    for (auto child : children) {
        if (child->type == type) {
            foundChildren.push_back(child);
        }
    }
}
