#include <QCoreApplication>
#include <QDir>
#include <QRegExp>

#include "texture_manager.h"

TextureManager *TextureManager::_instance = 0;

TextureManager &TextureManager::instance()
{
    if (!_instance)
        _instance = new TextureManager;

    return *_instance;
}

void TextureManager::addAnimation(QGLWidget *contextWidget, const QString &filename)
{
    if (_textures.find(filename) != _textures.end())
        return;

    QList<GLuint> animation;
    QList<GLuint> whiteAnimation;

    QDir imagesDir = QDir(QCoreApplication::applicationDirPath()).filePath("materials");

    QRegExp regExp("^" + filename + "-(\\d)+\\.png$");
    QMap<int,QString> files;
    foreach (const QFileInfo &fileInfo, imagesDir.entryInfoList(QDir::Files))
    {
        if (regExp.exactMatch(fileInfo.fileName()))
            files.insert(regExp.cap(1).toInt(), fileInfo.absoluteFilePath());
    }

    foreach (const QString &fileName, files)
    {
        animation << makeTexture(contextWidget, fileName);
        whiteAnimation << makeTexture(contextWidget, fileName, true);
    }

    _textures.insert(filename, animation);
    _whiteTextures.insert(filename, whiteAnimation);
}

GLuint TextureManager::makeTexture(QGLWidget *contextWidget, const QString &filename, bool white)
{
    QPixmap pixmap(filename);

    if (white)
    {
        QImage image = pixmap.toImage();
        for (int row = 0; row < image.height(); ++row)
            for (int col = 0; col < image.width(); ++col)
            {
                QRgb v = image.pixel(col, row) | 0x00FFFFFF;
                unsigned int a = v >> 25;
                v &= 0x00FFFFFF;
                v |= a << 24;
                image.setPixel(col, row, v);
            }
        pixmap = QPixmap::fromImage(image);
    }

    return contextWidget->bindTexture(pixmap, GL_TEXTURE_2D);
}

QList<GLuint> TextureManager::getAnimation(const QString &filename) const
{
    QMap<QString, QList<GLuint> >::const_iterator it = _textures.find(filename);
    if (it != _textures.end())
        return *it;
    return QList<GLuint>();
}

QList<GLuint> TextureManager::getWhiteAnimation(const QString &filename) const
{
    QMap<QString, QList<GLuint> >::const_iterator it = _whiteTextures.find(filename);
    if (it != _whiteTextures.end())
        return *it;
    return QList<GLuint>();
}
