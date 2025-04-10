#include "bullet.h"

Bullet::Bullet(const std::string& name,
    const std::string& description,
    const std::string& direction,
    Entity* parent)
    : Item(name, description, direction, parent)
{
    // Sin lógica adicional; su recolección se maneja fuera (World/Player)
}
