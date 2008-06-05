#ifndef BATTLEGROUND_H
#define BATTLEGROUND_H

#include <QList>

#include "item.h"

/*! \brief Define the battleground (all items)
 * It defines the collision system too
 */

class Battleground
{
public:
    Battleground() {}

private:
    QList<Item*> _items;
    Robot *_player;
    QList<Robot*> _enemies;
    QList<Bullet*> _bullets;
};

#endif
