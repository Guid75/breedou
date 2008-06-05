#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QObject>

class Keyboard : public QObject
{
    Q_OBJECT

public:
    static Keyboard *instance();

    void init() {}

    bool isKeyPushed(int key) const;

    bool leftClick() const { return _leftClick; }
    bool rightClick() const { return _rightClick; }

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    static Keyboard *_instance;
    QList<int> _keys;
    bool _leftClick;
    bool _rightClick;

    Keyboard(QObject *parent = 0);
};

#endif
