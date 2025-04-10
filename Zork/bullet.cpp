#include "bullet.h"

Bullet::Bullet(const std::string& name,
    const std::string& description,
    const std::string& direction,
    Entity* parent)
    : Item(name, description, direction, parent)
{
    // Sin l�gica adicional; su recolecci�n se maneja fuera (World/Player)
}
