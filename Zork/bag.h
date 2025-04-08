#ifndef BAG_H
#define BAG_H

#include "item.h"
#include <vector>

class Bag : public Item {
public:
    Bag(const std::string& name, const std::string& description, int capacity, Entity* parent = nullptr);
    virtual void Use() override; // Opens the bag to show contents
    bool AddItem(Item* item);
    bool RemoveItem(const std::string& itemName);
    void ListContents() const;
    int GetRemainingCapacity() const;

private:
    int capacity;
    std::vector<Item*> items;
};

#endif // BAG_H

