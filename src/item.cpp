#include <QFileInfo>
#include <QDir>
#include <QCoreApplication>
#include <QMap>

#include "texture_manager.h"

#include "item.h"

Item::Item()
    : _size(QSizeF(0.08, 0.08)), _position(QPointF(0.0, 0.0)), _rotation(0.0),
      _z(0.0), _destructible(true), _health(1), _collisionTimeout(0),
      _currentFrame(0), _frameDuration(100)
{
}

void Item::update(int delta)
{
    _frameDuration -= delta;

    if (_frameDuration <= 0)
    {
        _currentFrame = (_currentFrame + (_frameDuration / 100 + 1)) % _animationFrames.count();
        _frameDuration = 100;
    }

    if (_collisionTimeout)
    {
        _collisionTimeout -= delta;
        if (_collisionTimeout <= 0)
            _collisionTimeout = 0;
    }
}

void Item::init(const QString &animFileName)
{
    QList<GLuint> animation = TextureManager::instance().getAnimation(animFileName);
    QList<GLuint> whiteAnimation = TextureManager::instance().getWhiteAnimation(animFileName);

    for (int i = 0; i < animation.count(); ++i)
    {
        _animationFrames << makeObject(animation[i]);
        _whiteFrames << makeObject(whiteAnimation[i]);
    }
}

GLuint Item::makeObject(GLuint texture)
{
    GLuint list = glGenLists(1);
    glNewList(list, GL_COMPILE);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBegin(GL_QUADS);

    glTexCoord2d(0, 0);
    glVertex3d(-_size.width() / 2.0, _size.height() / 2.0, 0.0);

    glTexCoord2d(1, 0);
    glVertex3d(_size.width() / 2.0, _size.height() / 2.0, 0.0);

    glTexCoord2d(1, 1);
    glVertex3d(_size.width() / 2.0, - _size.height() / 2.0, 0.0);

    glTexCoord2d(0, 1);
    glVertex3d(- _size.width() / 2.0, - _size.height() / 2.0, 0.0);

    glEnd();

    glEndList();

    return list;
}

void Item::hit(int damages)
{
    if (_health <= 0)
        return;

    _health -= damages;
    _collisionTimeout = 100;
}

QRectF Item::boundingBox() const
{
    return QRectF(_position.x() - _size.width() / 2.0,
                  _position.y() - _size.height() / 2.0,
                  _size.width(),
                  _size.height());
}
