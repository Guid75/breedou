#include <math.h>

#include <QDir>
#include <QCoreApplication>
#include <QCursor>
#include <QKeyEvent>

#include "keyboard.h"
#include "bullet.h"
#include "box.h"
#include "texture_manager.h"

#include "playground.h"

Playground *Playground::_instance = 0;

Playground *Playground::instance()
{
    if (!_instance)
        _instance = new Playground;

    return _instance;
}

Playground::Playground(QWidget *parent) : QGLWidget(parent)
{
    _mousePos = QPoint();
    _oldFrameTime = QTime::currentTime();
    _fireDelay = 0;

    this->installEventFilter(this);

    _timerAnimation.setInterval(1000 / 60);
    connect(&_timerAnimation, SIGNAL(timeout()), this, SLOT(timerAnimationTimeout()));
    _timerAnimation.start();
}

Playground::~Playground()
{
    qDeleteAll(_items);
}

void Playground::initializeGL()
{
    // Set up the rendering context, define display lists etc.:
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glClearColor(0.0, 0.0, 0.0, 0.0);
//    glEnable(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    TextureManager::instance().addAnimation(this, "me");
    TextureManager::instance().addAnimation(this, "bullet");
    TextureManager::instance().addAnimation(this, "box");

    _myRobot.init("me");

    // Some random boxes
    for (int i = 10; i > 0; --i)
    {
        Box *box = new Box;
        box->init("box");
        QPointF p;
        p = QPointF(-0.5 + ((double) qrand() / (double) RAND_MAX), -0.5 + ((double) qrand() / (double) RAND_MAX));
        box->setPosition(p);
        box->setZ(i * -0.01);
        _items << box;
    }
}

void Playground::resizeGL(int w, int h)
{
    // setup viewport, projection etc.:
    int side = qMin(w, h);
    _viewport.setRect((w - side) / 2, (h - side) / 2, side, side);
    glViewport(_viewport.x(), _viewport.y(), _viewport.width(), _viewport.height());

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-0.5, 0.5, 0.5, -0.5, 0.0, 15.0);
    glMatrixMode(GL_MODELVIEW);
}

void Playground::paintGL()
{
    qglClearColor(QColor(20, 20, 20));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslated(_myRobot.position().x(), _myRobot.position().y(), -0.1);
    glRotated(_myRobot.rotation(), 0.0, 0.0, 1.0);
    glCallList(_myRobot.animationFrames()[_myRobot.currentFrame()]);

    foreach (Item *item, _items)
    {
        glLoadIdentity();
        glTranslated(item->position().x(), item->position().y(), item->z());
        glRotated(item->rotation(), 0.0, 0.0, 1.0);

        glCallList(item->animationFrames()[item->currentFrame()]);
        if (item->collisionTimeout())
            glCallList(item->whiteFrames()[item->currentFrame()]);
    }
}

void Playground::timerAnimationTimeout()
{
    // Angle between the robot and the mouse cursor
    QPoint newP = QCursor::pos();
    QPointF cursorScenePoint = pixelPointToScenePoint(mapFromGlobal(newP));
    QLineF vector(_myRobot.position(), cursorScenePoint);
    vector.setLength(1.0f);

    // Remove all outside bullets
    foreach (Item *item, _items)
    {
        int index = _items.indexOf(item);
        Bullet *bullet = dynamic_cast<Bullet*>(item);
        if (!bullet)
            continue;

        QRectF screen(-0.5, -0.5, 1.0, 1.0);
        QRectF bulletRect(QPointF(bullet->position().x() - bullet->size().width() / 2,
                                  bullet->position().y() - bullet->size().height() / 2),
                          bullet->size());
        if (!screen.intersects(bulletRect))
        {
            _items.removeAt(index);
            delete bullet;
        }
    }

    // Compute my speed
    QTime currentTime = QTime::currentTime();
    int delta = _oldFrameTime.msecsTo(currentTime);

    QPointF speed(0.0, 0.0);
    if (Keyboard::instance()->isKeyPushed(Qt::Key_Up))
        speed.setY(-0.005);
    else if (Keyboard::instance()->isKeyPushed(Qt::Key_Down))
        speed.setY(0.005);
    if (Keyboard::instance()->isKeyPushed(Qt::Key_Left))
        speed.setX(-0.005);
    else if (Keyboard::instance()->isKeyPushed(Qt::Key_Right))
        speed.setX(0.005);

    _myRobot.setSpeed(speed);

    QList<Item*> itemsToDestroy;

    // Collisions
    for (int i = 0; i < _items.count(); ++i)
    {
        Bullet *bullet = dynamic_cast<Bullet*>(_items[i]);

        if (!bullet)
            continue;

        QRectF bulletBB = bullet->boundingBox();

        for (int j = 0; j < _items.count(); j++)
        {
            Box *box = dynamic_cast<Box*>(_items[j]);
            if (!box)
                continue;
            if (box->boundingBox().intersects(bulletBB))
            {
                itemsToDestroy << bullet;
                box->hit(bullet->collisionDamages());
                if (box->health() <= 0)
                    itemsToDestroy << box;
                break;
            }
        }
    }

    for (int i = 0; i < itemsToDestroy.count(); ++i)
    {
        Item *item = itemsToDestroy[i];
        _items.removeAt(_items.indexOf(item));
        delete item;
    }
    itemsToDestroy.clear();

/*    // Collision of myself with boxes
    QRectF futureBB = _myRobot.futureBoundingBox();
    for (int j = 0; j < _items.count(); j++)
    {
        Box *box = dynamic_cast<Box*>(_items[j]);
        if (!box)
            continue;
        if (box->boundingBox().intersects(futureBB))
        {
            itemsToDestroy << bullet;
            box->hit(bullet->collisionDamages());
            if (box->health() <= 0)
                itemsToDestroy << box;
            break;
        }
        }*/

    // Shoot?
    if (Keyboard::instance()->leftClick())
    {
        if (_fireDelay <= 0)
        {
            // Fire!
            Bullet *bullet = new Bullet;
            bullet->init("bullet");
            _items << bullet;
            bullet->setPosition(_myRobot.position());
            QPointF speed(vector.dx() * 0.02, vector.dy() * 0.02);
            bullet->setSpeed(speed);

            _fireDelay = _myRobot.firePeriod();
        } else
            _fireDelay -= delta;
    } else if (_fireDelay > 0)
        _fireDelay -= delta;

    if (newP != _mousePos || speed.x() || speed.y())
    {
        _mousePos = newP;
        _myRobot.setRotation(- vector.angle());
    }

    foreach (Item *item, _items)
        item->update(delta);

    _myRobot.update(delta);

    _oldFrameTime = currentTime;

    updateGL();
}

bool Playground::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

    }
    return QGLWidget::eventFilter(obj, event);
}

QPoint Playground::scenePointToPixelPoint(const QPointF &scenePoint) const
{
    QPoint result;

    result.setX(_viewport.x() + (int) ((scenePoint.x() + 0.5) * _viewport.width()));
    result.setY(_viewport.y() + (int) ((scenePoint.y() + 0.5) * _viewport.height()));
    return result;
}

QPointF Playground::pixelPointToScenePoint(const QPoint &pixelPoint) const
{
    QPointF result;

    // Center pixel
    QPoint centerPoint(_viewport.left() + _viewport.width() / 2,
                       _viewport.top() + _viewport.height() / 2);

    result.setX((double) ((pixelPoint.x() - centerPoint.x()) * 0.5) / (double) (_viewport.width() / 2.0));
    result.setY((double) ((pixelPoint.y() - centerPoint.y()) * 0.5) / (double) (_viewport.height() / 2.0));

    return result;
}
