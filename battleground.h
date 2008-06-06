#include <QGLWidget>
#include <QTimer>
#include <QRectF>
#include <QTime>

#include "robot.h"

class Playground : public QGLWidget
{
    Q_OBJECT

public:
    static Playground *instance();

    const QList<Item*> &items() { return _items; }

protected:
    void initializeGL();

    void resizeGL(int w, int h);

    void paintGL();

    bool eventFilter(QObject *obj, QEvent *event);

private:
    static Playground *_instance;
    QPoint _mousePos;
    QTimer _timerAnimation;
    QRect _viewport;
    Robot _myRobot;
    QList<Item*> _items;
    QTime _oldFrameTime;
    int _fireDelay; // Time since the last shoot in ms

    Playground(QWidget *parent = 0);
    ~Playground();

    QPoint scenePointToPixelPoint(const QPointF &scenePoint) const;
    QPointF pixelPointToScenePoint(const QPoint &pixelPoint) const;

    bool intersectOtherItems(const QRectF &r);

private slots:
    void timerAnimationTimeout();
};
