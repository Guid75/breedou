#ifndef ITEM_H
#define ITEM_H

#include <QGLWidget>

class Item
{
public:
    Item();

    void init(const QString &animFileName);

    virtual void update(int delta);

    virtual QRectF boundingBox() const;

    const QSizeF &size() const { return _size; }

    const QPointF &position() const { return _position; } //! \brief Position of the item center
    void setPosition(const QPointF &value) { _position = value; }
    void setPosition(qreal x, qreal y) { _position.rx() = x; _position.ry() = y; }

    double rotation() const { return _rotation; }
    void setRotation(double value) { _rotation = value; }

    double z() const { return _z; }
    void setZ(double value) { _z = value; }

    bool destructible() const { return _destructible; }

    int health() const { return _health; }
    void hit(int damages);

    int collisionDamages() const { return _collisionDamages; }

    int collisionTimeout() const { return _collisionTimeout; }

    int currentFrame() const { return _currentFrame; }
    const QList<GLuint> &animationFrames() const { return _animationFrames; }
    const QList<GLuint> &whiteFrames() const { return _whiteFrames; }

protected:
    void setSize(const QSizeF &value) { _size = value; }
    void setDestructible(bool value) { _destructible = value; }
    void setHealth(int value) { _health = value; }
    void setCollisionDamages(int value) { _collisionDamages = value; }

private:
    QSizeF _size;
    QPointF _position;
    double _rotation;
    double _z;
    bool _destructible;
    int _health;
    int _collisionDamages;

    int _collisionTimeout; // In milliseconds

    QList<GLuint> _animationFrames;
    QList<GLuint> _whiteFrames;
    int _currentFrame;
    int _frameDuration; // In ms

    GLuint makeObject(GLuint texture);
};

#endif
