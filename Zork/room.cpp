#include "room.h"
#include "string_utils.h"
#include <iostream>
#include <algorithm>

Room::Room(const std::string& name, const std::string& description, Entity* parent)
    : Entity(name, description, parent)
{
    type = ROOM;
    directions["center"] = {};
    directions["north"] = {};
    directions["south"] = {};
    directions["east"] = {};
    directions["west"] = {};
}

void Room::AddEntity(const std::string& direction, Entity* entity) {
    auto it = directions.find(direction);
    if (it != directions.end()) {
        it->second.push_back(entity);
    }
}

void Room::RemoveEntity(const std::string& direction, Entity* entity) {
    auto it = directions.find(direction);
    if (it != directions.end()) {
        auto& vec = it->second;
        vec.erase(std::remove(vec.begin(), vec.end(), entity), vec.end());
    }
}

std::vector<Entity*> Room::GetEntities(const std::string& direction) const {
    auto it = directions.find(direction);
    if (it != directions.end()) {
        return it->second;
    }
    return {};
}

bool Room::HasDirection(const std::string& direction) const {
    return directions.find(direction) != directions.end();
}

void Room::Look() const {
    // Muestra el nombre y la descripción estática
    std::cout << name << "\n" << description << "\n";

    // Recorremos cada dirección e imprimimos las entidades de forma natural
    for (const auto& kv : directions) {
        const std::string& dir = kv.first;
        const auto& ents = kv.second;
        if (!ents.empty()) {
            std::cout << "In " << dir << " you see ";
            std::vector<std::string> phrases;

            // Para cada entidad, generamos la parte "a/an <nombre>"
            for (const auto& e : ents) {
                std::string article = getIndefiniteArticle(e->name);
                phrases.push_back(article + " " + e->name);
            }

            // Si hay uno solo, se imprime tal cual
            if (phrases.size() == 1) {
                std::cout << phrases[0];
            }
            // Si hay dos, se separan con " and "
            else if (phrases.size() == 2) {
                std::cout << phrases[0] << " and " << phrases[1];
            }
            // Si hay más, unimos con comas y "and" antes del último
            else {
                for (size_t i = 0; i < phrases.size(); i++) {
                    if (i == 0) {
                        std::cout << phrases[i];
                    }
                    else if (i < phrases.size() - 1) {
                        std::cout << ", " << phrases[i];
                    }
                    else {
                        std::cout << " and " << phrases[i];
                    }
                }
            }

            std::cout << ".\n";
        }
    }
}
