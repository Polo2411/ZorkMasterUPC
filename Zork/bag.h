#ifndef BAG_H
#define BAG_H

#include "item.h"
#include <vector>
#include <string>

class Bag : public Item {
public:
    Bag(const std::string& name,
        const std::string& description,
        int capacity,
        const std::string& direction,
        Entity* parent = nullptr);

    virtual void Use() override;  // Abre la bolsa (isOpen=true) y lista el contenido
    void Close();                 // Cierra la bolsa (isOpen=false)
    bool IsOpen() const;

    bool AddItem(Item* item);
    Item* RemoveItem(const std::string& itemName);
    void ListContents() const;
    int GetRemainingCapacity() const;

private:
    int capacity;
    std::vector<Item*> items;
    bool isOpen;
};

#endif // BAG_H
