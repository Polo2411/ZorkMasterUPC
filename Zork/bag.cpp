#include "bag.h"
#include "string_utils.h"  // Para toLower()
#include <iostream>
#include <algorithm>

Bag::Bag(const std::string& name,
    const std::string& description,
    int capacity,
    const std::string& direction,
    Entity* parent)
    : Item(name, description, direction, parent), capacity(capacity), isOpen(false)
{
}

void Bag::Use() {
    if (!isOpen) {
        isOpen = true;
        std::cout << "You open " << name << ".\n";
    }
    else {
        std::cout << name << " is already open.\n";
    }
    ListContents();
}

void Bag::Close() {
    if (isOpen) {
        isOpen = false;
        std::cout << "You close " << name << ".\n";
    }
    else {
        std::cout << name << " is already closed.\n";
    }
}

bool Bag::IsOpen() const {
    return isOpen;
}

bool Bag::AddItem(Item* item) {
    if (!isOpen) {
        std::cout << "You must open " << name << " before adding items.\n";
        return false;
    }
    if (items.size() < static_cast<size_t>(capacity)) {
        items.push_back(item);
        std::cout << "Added " << item->name << " to " << name << ".\n";
        return true;
    }
    std::cout << name << " is full.\n";
    return false;
}

Item* Bag::RemoveItem(const std::string& itemName) {
    if (!isOpen) {
        std::cout << "You must open " << name << " before removing items.\n";
        return nullptr;
    }
    auto it = std::find_if(items.begin(), items.end(),
        [&](Item* i) {
            // Comparar sin distinguir mayúsculas.
            return toLower(i->name) == toLower(itemName);
        });
    if (it != items.end()) {
        Item* ptr = *it;
        items.erase(it);
        std::cout << "Removed " << itemName << " from " << name << ".\n";
        return ptr;
    }
    std::cout << itemName << " not found in " << name << ".\n";
    return nullptr;
}

void Bag::ListContents() const {
    if (items.empty()) {
        std::cout << name << " is empty.\n";
        return;
    }
    std::cout << name << " contains:\n";
    for (auto i : items) {
        std::cout << " - " << i->name << "\n";
    }
}

int Bag::GetRemainingCapacity() const {
    return capacity - static_cast<int>(items.size());
}
