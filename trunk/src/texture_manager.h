#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <QGLWidget>
#include <QMap>
#include <QString>
#include <QList>

class TextureManager
{
public:
    static TextureManager &instance();

    void addAnimation(QGLWidget *contextWidget, const QString &filename);
    QList<GLuint> getAnimation(const QString &filename) const;
    QList<GLuint> getWhiteAnimation(const QString &filename) const;

private:
    static TextureManager *_instance;

    QMap<QString, QList<GLuint> > _textures;
    QMap<QString, QList<GLuint> > _whiteTextures;

    GLuint makeTexture(QGLWidget *widget, const QString &filename, bool white = false);
};

#endif
