#include "box.h"

Box::Box()
    : Item()
{
    setDestructible(true);
    setHealth(20);
    setCollisionDamages(0);
}
