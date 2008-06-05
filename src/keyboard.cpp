#include <QApplication>
#include <QKeyEvent>

#include "keyboard.h"

Keyboard *Keyboard::_instance = 0;

Keyboard *Keyboard::instance()
{
    if (!_instance)
        _instance = new Keyboard;

    return _instance;
}

Keyboard::Keyboard(QObject *parent)
    : QObject(parent), _leftClick(false), _rightClick(false)

{
    qApp->installEventFilter(this);
}

bool Keyboard::eventFilter(QObject *watched, QEvent *event)
{
    switch (event->type())
    {
    case QEvent::KeyPress:
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        _keys << keyEvent->key();
    }
    break;
    case QEvent::KeyRelease:
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (_keys.contains(keyEvent->key()))
            _keys.removeAt(_keys.indexOf(keyEvent->key()));
    }
    break;
    case QEvent::MouseButtonDblClick:
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton)
            _leftClick = true;
        if (mouseEvent->button() == Qt::RightButton)
            _rightClick = true;
    }
    case QEvent::MouseButtonPress:
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton)
            _leftClick = true;
        if (mouseEvent->button() == Qt::RightButton)
            _rightClick = true;
    }
    break;
    case QEvent::MouseButtonRelease:
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton)
            _leftClick = false;
        if (mouseEvent->button() == Qt::RightButton)
            _rightClick = false;
    }
    break;
    default:;
    }

    return QObject::eventFilter(watched, event);
}

bool Keyboard::isKeyPushed(int key) const
{
    return _keys.indexOf(key) >= 0;
}
