#ifndef SPRITECREATOR_H
#define SPRITECREATOR_H

#include "qobject.h"

#include "qimage.h"

#include "oxygine-flow.h"

class SpriteCreator : public QObject
{
    Q_OBJECT
    static const qint32 colorBoxSize;
public:
    /**
     * @brief createSprites
     * @param input
     * @param colorTable
     * @param maskTable
     */
    static void createSprites(QString input, QString colorTable, QString maskTable);
    /**
     * @brief createSprites
     * @param file
     * @param colorTable
     * @param maskTable
     */
    static void createSprites(QString file, QImage& colorTable, QImage maskTable);
    /**
     * @brief createAnim
     * @param input
     * @param colorTable
     * @param newTable
     * @param columns
     * @param rows
     * @param scaleFactor
     * @return
     */
    static oxygine::ResAnim* createAnim(QString input, QString colorTable, QString newTable, bool useColorBox,
                                        qint32 columns, qint32  rows, float scaleFactor);
    /**
     * @brief createAnim
     * @param input
     * @param colorTableImg
     * @param maskTableImg
     * @param columns
     * @param rows
     * @param scaleFactor
     * @return
     */
    static oxygine::ResAnim* createAnim(QString input, QImage& colorTableImg, QImage& maskTableImg,
                                        bool useColorBox, qint32 columns, qint32  rows, float scaleFactor);
    /**
     * @brief createSprite
     * @param input
     * @param colorTableImg
     * @param maskTableImg
     * @param useColorBox
     */
    static void createSprite(QString input, QImage& colorTableImg, QImage maskTableImg, bool useColorBox);

    /**
     * @brief createColorTable
     * @param image
     * @return
     */
    static QImage createColorTable(QImage& image);
    /**
     * @brief getColorBox
     * @param color
     * @return
     */
    static QColor getColorBox(QColor color);
    /**
     * @brief getImageColor
     * @param maskColor
     * @param imageColor
     * @return
     */
    static QColor getImageColor(QColor maskColor, QColor imageColor);
    /**
     * @brief updateMaskImages
     * @param folder
     * @param filter
     */
    static void updateMaskImages(QString& folder, QString& filter);
    /**
     * @brief updateMaskImage
     * @param file
     */
    static void updateMaskImage(QString& file);
};

#endif // SPRITECREATOR_H
