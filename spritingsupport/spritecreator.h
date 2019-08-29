#ifndef SPRITECREATOR_H
#define SPRITECREATOR_H

#include "qobject.h"

#include "qimage.h"

#include "oxygine-flow.h"

class SpriteCreator : public QObject
{
    Q_OBJECT
public:
    static void createSprites(QString input, QString colorTable, QString maskTable);
    static void createSprites(QString file, QImage& colorTable, QImage maskTable);
    static oxygine::ResAnim* createAnim(QString input, QString colorTable, QString newTable,
                                                       qint32 columns, qint32  rows, float scaleFactor);
    static oxygine::ResAnim* createAnim(QString input, QImage& colorTableImg, QImage& maskTableImg,
                                                 qint32 columns, qint32  rows, float scaleFactor);
    static void createSprite(QString input, QImage& colorTableImg, QImage maskTableImg);

    static QImage createColorTable(QImage& image);
};

#endif // SPRITECREATOR_H
