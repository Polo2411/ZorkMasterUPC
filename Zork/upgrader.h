// upgrader.h
#ifndef UPGRADER_H
#define UPGRADER_H

#include "item.h"

class Upgrader : public Item {
public:
    Upgrader(const std::string& name,
        const std::string& description,
        const std::string& direction,
        Entity* parent = nullptr);
};

#endif
