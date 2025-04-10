#ifndef BULLET_H
#define BULLET_H

#include "item.h"

// Representa una bala individual.
// Su lógica de "agregarse" a la Gun se maneja fuera,
// por ejemplo, en World o Player al "take bullet".
class Bullet : public Item {
public:
    Bullet(const std::string& name,
        const std::string& description,
        const std::string& direction,
        Entity* parent = nullptr);
};

#endif // BULLET_H

