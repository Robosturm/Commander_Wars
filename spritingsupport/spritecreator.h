#ifndef SPRITECREATOR_H
#define SPRITECREATOR_H

#include <QObject>
#include <QImage>

#include "3rd_party/oxygine-framework/oxygine/res/ResAnim.h"

class SpriteCreator : public QObject
{
    Q_OBJECT
    static const qint32 colorBoxSize;
public:
    /**
     * @brief updateTerrainPaletteMasks
     * @param folder
     * @param filter
     * @param originalPaletteSize
     * @param newPaletteSize
     */
    static void updateTerrainPaletteMasks(const QString& folder, const QString& filter, qint32 originalPaletteSize, qint32 newPaletteSize);
    /**
     * @brief updateTerrainPaletteMask
     * @param folder
     * @param filter
     * @param originalPaletteSize
     * @param newPaletteSize
     */
    static void updateTerrainPaletteMask(const QString& file, qint32 originalPaletteSize, qint32 newPaletteSize);
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
    static void createSprites(QString file, QImage& colorTable, const QImage & maskTable);
    /**
     * @brief createColorTableSprites
     * @param folder
     * @param filter
     * @param startIndex
     * @param maxColors
     */
    static void createColorTableSprites(const QString& folder, const QString& filter, qint32 startIndex, qint32 maxColors);
    /**
     * @brief createColorTableSprite
     * @param file
     * @param startIndex
     * @param maxColors
     */
    static void createColorTableSprite(const QString& file, qint32 startIndex, qint32 maxColors);
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
    static oxygine::spResAnim createAnim(QString input, QString colorTable, QString newTable, bool useColorBox,
                                        qint32 columns, qint32  rows, float scaleFactor, bool addTransparentBorder);
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
    static oxygine::spResAnim createAnim(QString input, QImage& colorTableImg, QImage& maskTableImg,
                                        bool useColorBox, qint32 columns, qint32  rows, float scaleFactor, bool addTransparentBorder);
    /**
     * @brief createSprite
     * @param input
     * @param colorTableImg
     * @param maskTableImg
     * @param useColorBox
     */
    static QImage createSprite(QString input, QImage& colorTableImg, const QImage & maskTableImg, bool useColorBox, bool save);
    /**
     * @brief recolorImageWithTable
     * @param image
     * @param colorTable
     * @return
     */
    static QImage recolorImageWithTable(QImage & image, QImage & colorTable);
    /**
     * @brief mergeImages
     * @param image
     * @param mask
     * @return
     */
    static QImage mergeImages(QImage & image, QImage & mask);
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
    static void updateMaskImages(QString& folder, QString& filter, qint32 min);
    /**
     * @brief updateMaskImage
     * @param file
     */
    static void updateMaskImage(QString& file, qint32 min);
    /**
     * @brief applyImagesTable
     * @param input
     * @param inTable
     * @param outTable
     * @param eraseColor
     */
    static void applyImagesTable(QString input, QString inTable, QString outTable, QColor eraseColor = Qt::white);
    /**
     * @brief applyImageTable
     * @param file
     * @param inTableImg
     * @param outTableImg
     * @param eraseColor
     */
    static void applyImageTable(QString file, const QImage & inTableImg, const QImage & outTableImg, QColor eraseColor);
    /**
     * @brief inversImagesFrames
     * @param folder
     * @param filter
     * @param frames
     */
    static void inversImagesFrames(QString& folder, QString& filter, qint32 frames);
    /**
     * @brief inversImageFrames
     * @param file
     * @param frames
     */
    static void inversImageFrames(QString& file, qint32 frames);
    /**
     * @brief extendMaskImages
     * @param folder
     * @param filter
     */
    static void extendMaskImages(QString& folder, QString& filter);
    /**
     * @brief extendMaskImage
     * @param file
     */
    static void extendMaskImage(QString& file);
    /**
     * @brief addFrameBorders
     * @param image
     * @param columns
     * @param rows
     */
    static void addFrameBorders(QImage & image, qint32 columns, qint32 rows, bool addOnlyTransparentBorder);
    /**
     * @brief preProcessMask
     * @param mask
     * @param overlay
     * @param columns
     * @param rows
     */
    static void preProcessMask(QImage & mask, const QImage & overlay, qint32 columns, qint32 rows);
    /**
     * @brief convertToRgba
     * @param input
     */
    static void convertToRgba(QImage & input);
};

#endif // SPRITECREATOR_H
