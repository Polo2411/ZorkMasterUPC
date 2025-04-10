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
    std::map<std::string, std::vector<Entity*>>::iterator it = directions.find(direction);
    if (it != directions.end()) {
        it->second.push_back(entity);
    }
}

void Room::RemoveEntity(const std::string& direction, Entity* entity) {
    std::map<std::string, std::vector<Entity*>>::iterator it = directions.find(direction);
    if (it != directions.end()) {
        std::vector<Entity*>& vec = it->second;
        vec.erase(std::remove(vec.begin(), vec.end(), entity), vec.end());
    }
}

std::vector<Entity*> Room::GetEntities(const std::string& direction) const {
    std::map<std::string, std::vector<Entity*>>::const_iterator it = directions.find(direction);
    if (it != directions.end()) {
        return it->second; // Devolvemos copia
    }
    return {};
}

bool Room::HasDirection(const std::string& direction) const {
    return directions.find(direction) != directions.end();
}

void Room::Look() const {
    std::cout << name << "\n" << description << "\n";

    // Recorremos cada direcci�n
    for (std::map<std::string, std::vector<Entity*>>::const_iterator it = directions.begin();
        it != directions.end(); ++it)
    {
        const std::string& dir = it->first;
        const std::vector<Entity*>& ents = it->second;
        if (!ents.empty()) {
            std::cout << "In " << dir << " you see ";
            std::vector<std::string> phrases;
            phrases.reserve(ents.size());

            for (size_t i = 0; i < ents.size(); i++) {
                std::string article = getIndefiniteArticle(ents[i]->name);
                phrases.push_back(article + " " + ents[i]->name);
            }

            if (phrases.size() == 1) {
                std::cout << phrases[0];
            }
            else if (phrases.size() == 2) {
                std::cout << phrases[0] << " and " << phrases[1];
            }
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

const std::map<std::string, std::vector<Entity*>>& Room::GetDirections() const {
    return directions;
}
