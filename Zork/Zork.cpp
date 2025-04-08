#include "entity.h"

int main() {
    // Crear una entidad base sin padre.
    Entity* e = new Entity("BaseEntity", "Una entidad de prueba");
    e->Look();

    delete e;
    return 0;
}
