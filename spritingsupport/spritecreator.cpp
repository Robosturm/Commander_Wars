#include "qdir.h"
#include "qfile.h"
#include "qfileinfo.h"
#include "qdiriterator.h"

#include "coreengine/console.h"
#include "coreengine/mainapp.h"
#include "coreengine/globalutils.h"

#include "spritingsupport/spritecreator.h"

const qint32 SpriteCreator::colorBoxSize = 43;

void SpriteCreator::createColorTableSprites(const QString& folder, const QString& filter, qint32 startIndex, qint32 maxColors)
{
    QStringList filters;
    filters << filter;
    QDirIterator dirIter(folder, filters, QDir::Files, QDirIterator::IteratorFlag::Subdirectories);
    while (dirIter.hasNext())
    {
        dirIter.next();
        QString file = dirIter.fileInfo().absoluteFilePath();
        createColorTableSprite(file, startIndex, maxColors);
    }
}
void SpriteCreator::createColorTableSprite(const QString& file, qint32 startIndex, qint32 maxColors)
{
    QImage org(file);
    QVector<QColor> colorsUnsorted;
    for (qint32 x = 0; x < org.width(); x++)
    {
        for (qint32 y = 0; y < org.height(); y++)
        {
            // color pixel or another one?
            QColor color = org.pixelColor(x, y);
            if (color.alpha() > 0)
            {
                if (!colorsUnsorted.contains(color))
                {
                    colorsUnsorted.append(color);
                }
            }
        }
    }
    QVector<QColor> colors;
    while (colorsUnsorted.size() > 0)
    {
        QColor color = colorsUnsorted[0];
        for (qint32 i = 1; i < colorsUnsorted.size(); i++)
        {
            if (color.red() < colorsUnsorted[i].red())
            {
                color = colorsUnsorted[i];
            }
        }
        colors.append(color);
        colorsUnsorted.removeAll(color);
    }
    qint32 maxUsedColors = maxColors;
    if (colors.size() < maxColors)
    {
        maxUsedColors = colors.size();
    }
    for (qint32 x = 0; x < org.width(); x++)
    {
        for (qint32 y = 0; y < org.height(); y++)
        {
            QColor color = org.pixelColor(x, y);
            if (color.alpha() > 0)
            {
                float progress = colors.indexOf(color) / static_cast<float>(colors.size() - 1);
                qint32 index = static_cast<qint32>(((maxColors - 1) - progress * (maxUsedColors - 1))) * 3 + startIndex;
                org.setPixelColor(x, y, QColor(index, 255 * (1.0f - progress), 255 * (1.0f - progress), 255));
            }
        }
    }
    QFile::remove(file);
    org.save(file);
}

void SpriteCreator::applyImagesTable(QString input, QString inTable, QString outTable, QColor eraseColor)
{
    if (!QFile::exists(inTable) && inTable.endsWith(".png"))
    {
        Console::print("The color table is not an existing file. " + inTable, Console::eLogLevels::eERROR);
        return;
    }
    if (!QFile::exists(outTable) && outTable.endsWith(".png"))
    {
        Console::print("The mask table is not an existing file. " + outTable, Console::eLogLevels::eERROR);
        return;
    }
    QImage inTableImg(inTable);
    QImage outTableImg(outTable);
    QFileInfo inputInfo(input);
    if (inputInfo.isDir())
    {
        QStringList filter;
        filter << "*.png";
        QDirIterator dirIter(input, filter, QDir::Files);
        while (dirIter.hasNext())
        {
            dirIter.next();
            applyImageTable(dirIter.fileInfo().absoluteFilePath(), inTableImg, outTableImg, eraseColor);
        }
    }
    else if (inputInfo.isFile() && input.endsWith(".png"))
    {
        applyImageTable(input, inTableImg, outTableImg, eraseColor);
    }
    else
    {
        Console::print("Input directory or file doesn't exists. " + input, Console::eERROR);
    }
}

void SpriteCreator::applyImageTable(QString file, QImage inTableImg, QImage outTableImg, QColor eraseColor)
{
    QString orgFile = file;
    QString maskFile = file;
    maskFile = maskFile.replace(".png", "+mask.png");
    QFile::copy(file, orgFile.replace(".png", "_org.png"));
    QImage orgImg(orgFile);
    QImage mainImg(orgImg.size(), QImage::Format_RGBA8888);


    for (qint32 y = 0; y < orgImg.height(); y++)
    {
        qint32 xPos = 0;
        for (qint32 x = 0; x < orgImg.width(); x++)
        {
            // color pixel or another one?
            QColor org = orgImg.pixelColor(xPos, y);
            if (eraseColor != Qt::white &&
                eraseColor != Qt::black)
            {
                while (eraseColor.rgba() == org.rgba())
                {
                    xPos++;
                    if (xPos >= orgImg.width())
                    {
                        break;
                    }
                    org = orgImg.pixelColor(xPos, y);
                }
            }
            if (xPos >= orgImg.width())
            {
                break;
            }
            for (qint32 i = 0; i < inTableImg.width(); i++)
            {
                QColor in = inTableImg.pixelColor(i, 0);
                if (in.rgba() == org.rgba())
                {
                    mainImg.setPixelColor(x, y, outTableImg.pixelColor(i, 0));
                    break;
                }
                else if (i == inTableImg.width() - 1)
                {
                    mainImg.setPixelColor(x, y, org);
                    break;
                }
            }
            xPos++;
        }
    }
    QFile::remove(file);
    mainImg.save(file);
}

void SpriteCreator::createSprites(QString input, QString colorTable, QString maskTable)
{
    if (!QFile::exists(colorTable) && colorTable.endsWith(".png"))
    {
        Console::print("The color table is not an existing file. " + colorTable, Console::eLogLevels::eERROR);
        return;
    }
    QImage colorTableImg(colorTable);
    if (!QFile::exists(maskTable) && maskTable.endsWith(".png"))
    {
        Console::print("The mask table is not an existing file. " + maskTable, Console::eLogLevels::eERROR);
        return;
    }
    QImage maskTableImg(maskTable);
    if (!QFile::exists(maskTable))
    {
        maskTableImg = createColorTable(colorTableImg);
    }
    if (maskTableImg.width() < colorTableImg.width())
    {
        Console::print("The mask table is to small. " + maskTable, Console::eERROR);
        return;
    }
    QFileInfo inputInfo(input);
    if (inputInfo.isDir())
    {
        QStringList filter;
        filter << "*.png";
        QDirIterator dirIter(input, filter, QDir::Files);
        while (dirIter.hasNext())
        {
            dirIter.next();
            createSprites(dirIter.fileInfo().absoluteFilePath(), colorTableImg, maskTableImg);
        }
    }
    else if (inputInfo.isFile() && input.endsWith(".png"))
    {
        createSprites(input, colorTableImg, maskTableImg);
    }
    else
    {
        Console::print("Input directory or file doesn't exists. " + input, Console::eERROR);
    }
}

void SpriteCreator::createSprites(QString file, QImage& colorTable, QImage maskTable)
{
    QString orgFile = file;
    QString maskFile = file;
    maskFile = maskFile.replace(".png", "+mask.png");
    QFile::copy(file, orgFile.replace(".png", "_org.png"));
    QImage orgImg(orgFile);
    QImage mainImg(orgImg.size(), QImage::Format_RGBA8888);
    QImage maskImg(orgImg.size(), QImage::Format_RGBA8888);
    for (qint32 x = 0; x < orgImg.width(); x++)
    {
        for (qint32 y = 0; y < orgImg.height(); y++)
        {
            // color pixel or another one?
            QColor org = orgImg.pixelColor(x, y);
            for (qint32 i = 0; i < colorTable.width(); i++)
            {
                QColor mask = colorTable.pixelColor(i, 0);
                if (mask.rgba() == org.rgba())
                {
                    mainImg.setPixelColor(x, y, Qt::transparent);
                    maskImg.setPixelColor(x, y, maskTable.pixelColor(i, 0));
                    break;
                }
                else if (i == colorTable.width() - 1)
                {
                    mainImg.setPixelColor(x, y, orgImg.pixelColor(x, y));
                    maskImg.setPixelColor(x, y, Qt::transparent);
                }
            }
        }
    }
    QFile::remove(file);
    QFile::remove(maskFile);
    mainImg.save(file);
    maskImg.save(maskFile);
}

oxygine::spResAnim SpriteCreator::createAnim(QString input, QString colorTable, QString newTable, bool useColorBox,
                                            qint32 columns, qint32  rows, float scaleFactor)
{
    if (!QFile::exists(colorTable) && colorTable.endsWith(".png"))
    {
        Console::print("The color table is not an existing file. " + colorTable, Console::eLogLevels::eERROR);
        return nullptr;
    }
    QImage colorTableImg(colorTable);
    if (!QFile::exists(newTable) && newTable.endsWith(".png"))
    {
        Console::print("The mask table is not an existing file. " + newTable, Console::eLogLevels::eERROR);
        return nullptr;
    }
    QImage maskTableImg(newTable);
    if (maskTableImg.width() < colorTableImg.width())
    {
        Console::print("The mask table is to small. " + newTable, Console::eERROR);
        return nullptr;
    }
    return createAnim(input, colorTableImg, maskTableImg, useColorBox, columns, rows, scaleFactor);
}

oxygine::spResAnim SpriteCreator::createAnim(QString input, QImage& colorTableImg, QImage& maskTableImg, bool useColorBox,
                                            qint32 columns, qint32  rows, float scaleFactor)
{
    QFileInfo inputInfo(input);
    if (inputInfo.isFile() && inputInfo.exists())
    {
        QImage img = createSprite(input, colorTableImg, maskTableImg, useColorBox, false);
        oxygine::spSingleResAnim pRet = oxygine::spSingleResAnim::create();
        pRet->setResPath(input);
        Mainapp::getInstance()->loadResAnim(pRet, img, columns, rows, scaleFactor);
        return pRet;
    }
    return nullptr;
}

QImage SpriteCreator::createSprite(QString input, QImage& colorTableImg, QImage maskTableImg, bool useColorBox, bool save)
{
    QImage orgImg(input);
    QImage mainImg(orgImg.size(), QImage::Format_RGBA8888);
    for (qint32 x = 0; x < orgImg.width(); x++)
    {
        for (qint32 y = 0; y < orgImg.height(); y++)
        {
            // color pixel or another one?
            QColor org = orgImg.pixelColor(x, y);
            QColor orgBox = getColorBox(org);
            bool colorSet = false;
            for (qint32 i = 0; i < colorTableImg.width(); i++)
            {
                QColor pixel = colorTableImg.pixelColor(i, 0);
                QColor boxColor = getColorBox(pixel);
                if (useColorBox)
                {
                    if (boxColor.rgba() == orgBox.rgba())
                    {
                        mainImg.setPixelColor(x, y, getImageColor(maskTableImg.pixelColor(i, 0), org));
                        colorSet = true;
                        break;
                    }
                    else if (i == colorTableImg.width() - 1)
                    {
                        mainImg.setPixelColor(x, y, orgImg.pixelColor(x, y));
                        colorSet = true;
                    }
                }
                else
                {
                    if (pixel.rgba() == org.rgba())
                    {
                        mainImg.setPixelColor(x, y, maskTableImg.pixelColor(i, 0));
                        colorSet = true;
                        break;
                    }
                    else if (i == colorTableImg.width() - 1)
                    {
                        mainImg.setPixelColor(x, y, orgImg.pixelColor(x, y));
                        colorSet = true;
                    }
                }
            }
            if (!colorSet)
            {
                mainImg.setPixelColor(x, y, orgImg.pixelColor(x, y));
            }
        }
    }
    if (save)
    {
        QFile::remove("temp.png");
        mainImg.save("temp.png");
    }
    return mainImg;
}

QImage SpriteCreator::createColorTable(QImage& image)
{
    qint32 maxSize = 200;
    QImage ret(maxSize, 1, QImage::Format_RGBA8888);
    qint32 width = 0;
    for (qint32 x = 0; x < image.width(); x++)
    {
        for (qint32 y = 0; y < image.height(); y++)
        {
            QColor pixel = image.pixelColor(x, y);
            if (pixel.alpha() == 255)
            {
                pixel = getColorBox(pixel);
                bool found = false;
                for (qint32 i = 0; i < width; i++)
                {
                    if (ret.pixelColor(i, 0) == pixel)
                    {
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    ret.setPixelColor(width, 0, pixel);
                    width++;
                }
                if (width >= maxSize)
                {
                    break;
                }
            }
        }
    }
    return ret.copy(0, 0, width, 1);
}

QColor SpriteCreator::getColorBox(QColor color)
{
    qint32 red = color.red();
    qint32 green = color.green();
    qint32 blue = color.blue();
    return QColor((red / colorBoxSize) * colorBoxSize,
                  (green / colorBoxSize) * colorBoxSize,
                  (blue / colorBoxSize) * colorBoxSize,
                  color.alpha());
}

QColor SpriteCreator::getImageColor(QColor maskColor, QColor imageColor)
{
    qint32 red = maskColor.red() + imageColor.red() % colorBoxSize;
    qint32 green = maskColor.green() + imageColor.green() % colorBoxSize;
    qint32 blue = maskColor.blue() + imageColor.blue() % colorBoxSize;
    if (red > 255)
    {
        red = 255;
    }
    if (green > 255)
    {
        green = 255;
    }
    if (blue > 255)
    {
        blue = 255;
    }
    return QColor(red,
                  green,
                  blue,
                  imageColor.alpha());
}

void SpriteCreator::updateMaskImages(QString& folder, QString& filter, qint32 min)
{
    QStringList filters;
    filters << filter;
    QDirIterator dirIter(folder, filters, QDir::Files, QDirIterator::IteratorFlag::Subdirectories);
    while (dirIter.hasNext())
    {
        dirIter.next();
        QString file = dirIter.fileInfo().absoluteFilePath();
        updateMaskImage(file, min);
    }
}

void SpriteCreator::updateMaskImage(QString& file, qint32 min)
{
    QImage mask(file);
    QVector<QColor> colors;
    QColor maskMin(min, min, min);
    QColor maskMax (255, 255, 255);
    for (qint32 x = 0; x < mask.width(); x++)
    {
        for (qint32 y = 0; y < mask.height(); y++)
        {
            // color pixel or another one?
            QColor org = mask.pixelColor(x, y);
            if (!colors.contains(org) && org.alpha() == 255)
            {
                qint32 sum = org.red() + org.green() + org.blue();
                bool inserted = false;
                if (colors.size() == 1)
                {
                    if (sum > colors[0].red() + colors[0].green() + colors[0].blue())
                    {
                        colors.append(org);
                    }
                    else
                    {
                        colors.insert(0, org);
                    }
                }
                else
                {
                    if (colors.size() > 0)
                    {
                        if (sum < colors[0].red() + colors[0].green() + colors[0].blue())
                        {
                            colors.insert(0, org);
                            inserted = true;
                        }
                        else
                        {
                            for (qint32 i = 0; i < colors.size() - 1; i++)
                            {
                                if (sum > colors[i].red() + colors[i].green() + colors[i].blue() &&
                                    sum < colors[i + 1].red() + colors[i + 1].green() + colors[i + 1].blue())
                                {
                                    colors.insert(i + 1, org);
                                    inserted = true;
                                }
                            }
                        }
                    }
                    if (!inserted)
                    {
                        colors.append(org);
                    }
                }
            }
        }
    }
    for (qint32 x = 0; x < mask.width(); x++)
    {
        for (qint32 y = 0; y < mask.height(); y++)
        {
            // color pixel or another one?
            QColor org = mask.pixelColor(x, y);
            if (org.alpha() == 255)
            {
                qint32 index = colors.indexOf(org);
                float factor = static_cast<float>(index) / static_cast<float>(colors.size() - 1);
                QColor newColor(factor * (maskMax.red() - maskMin.red()) + maskMin.red(),
                                factor * (maskMax.green() - maskMin.green()) + maskMin.green(),
                                factor * (maskMax.blue() - maskMin.blue()) + maskMin.blue());
                mask.setPixelColor(x, y, newColor);
            }
        }
    }
    QFile::remove(file);
    mask.save(file);
}

void SpriteCreator::inversImagesFrames(QString& folder, QString& filter, qint32 frames)
{
    QStringList filters;
    filters << filter;
    QDirIterator dirIter(folder, filters, QDir::Files, QDirIterator::IteratorFlag::Subdirectories);
    while (dirIter.hasNext())
    {
        dirIter.next();
        QString file = dirIter.fileInfo().absoluteFilePath();
        inversImageFrames(file, frames);
    }
}

void SpriteCreator::inversImageFrames(QString& file, qint32 frames)
{
    QImage picture(file);
    QImage newPicture(picture.size(), picture.format());
    qint32 frameWidth = picture.width() / frames;
    for (qint32 i = 0; i < frames; i++)
    {
        for (qint32 x = 0; x < frameWidth; x++)
        {
            for (qint32 y = 0; y < picture.height(); y++)
            {
                // color pixel or another one?
                QColor org = picture.pixelColor(picture.width() - (i + 1) * frameWidth + x, y);
                newPicture.setPixelColor(x + i * frameWidth, y, org);
            }
        }
    }
    QFile::remove(file);
    newPicture.save(file);
}

void SpriteCreator::extendMaskImages(QString& folder, QString& filter)
{
    QStringList filters;
    filters << filter;
    QDirIterator dirIter(folder, filters, QDir::Files, QDirIterator::IteratorFlag::Subdirectories);
    while (dirIter.hasNext())
    {
        dirIter.next();
        QString file = dirIter.fileInfo().absoluteFilePath();
        extendMaskImage(file);
    }
}

void SpriteCreator::extendMaskImage(QString& file)
{
    constexpr qint32 alpha = 0;
    QImage picture(file);
    QImage newPicture(picture.size(), picture.format());
    for (qint32 x = 0; x < picture.width(); x++)
    {
        for (qint32 y = 0; y < picture.height(); y++)
        {
            // color pixel or another one?
            QColor org = picture.pixelColor(x, y);
            if (org.alpha() == alpha)
            {
                if (y + 1 < picture.height() &&
                    picture.pixelColor(x, y + 1).alpha() != alpha)
                {
                    QColor color = picture.pixelColor(x, y + 1);
                    newPicture.setPixelColor(x, y, color);
                }
                else if (y - 1 >= 0 &&
                         picture.pixelColor(x, y - 1).alpha() != alpha)
                {
                    QColor color = picture.pixelColor(x, y - 1);
                    newPicture.setPixelColor(x, y, color);
                }
                else if (x + 1 < picture.width() &&
                    picture.pixelColor(x + 1, y).alpha() != alpha)
                {
                    QColor color = picture.pixelColor(x + 1, y);
                    newPicture.setPixelColor(x, y, color);
                }
                else if (x - 1 >= 0 &&
                         picture.pixelColor(x - 1, y).alpha() != alpha)
                {
                    QColor color = picture.pixelColor(x - 1, y);
                    newPicture.setPixelColor(x, y, color);
                }
                else
                {
                    QColor color = picture.pixelColor(x, y);
                    newPicture.setPixelColor(x, y, color);
                }
            }
            else
            {
                QColor color = picture.pixelColor(x, y);
                newPicture.setPixelColor(x, y, color);
            }
        }
    }
    QFile::remove(file);
    newPicture.save(file);
}

void SpriteCreator::addTransparentBorder(QImage & image, qint32 columns, qint32 rows)
{
    if ((columns > 1) || (rows > 1))
    {
        QImage newImg(image.width() + columns,
                     image.height() + rows, image.format());
        qint32 frameWidth = image.width() / columns;
        qint32 frameHeigth = image.height() / rows;

        for (qint32 column = 0; column < columns; ++column)
        {
            for (qint32 row = 0; row < rows; ++row)
            {
                for (qint32 x = 0; x < frameWidth; ++x)
                {
                    for (qint32 y = 0; y < frameHeigth; ++y)
                    {
                        qint32 posX = x + column * frameWidth;
                        qint32 posY = y + row * frameHeigth;
                        newImg.setPixel(posX + column, posY + row, image.pixel(posX, posY));
                    }
                }
                qint32 posX = (column + 1) * frameWidth + column;
                for (qint32 y = 0; y < frameHeigth; ++y)
                {
                    newImg.setPixel(posX, row * frameHeigth + y + row, Qt::transparent);
                }
                qint32 posY = (row + 1) * frameHeigth + row;
                for (qint32 x = 0; x <= frameWidth; ++x)
                {
                    newImg.setPixel(column * frameWidth + x + column, posY, Qt::transparent);
                }
            }
        }
        image = newImg;
    }
}

void SpriteCreator::preProcessMask(QImage & mask, const QImage & overlay, qint32 columns, qint32 rows)
{
    constexpr qint32 alpha = 0;
    QImage newPicture(mask.size(), mask.format());
    qint32 frameWidth = mask.width() / columns;
    qint32 frameHeigth = mask.height() / rows;
    for (qint32 x = 0; x < mask.width(); x++)
    {
        for (qint32 y = 0; y < mask.height(); y++)
        {
            // color pixel or another one?
            QColor org = mask.pixelColor(x, y);
            QColor overlayColor = overlay.pixelColor(x, y);
            if (org.alpha() == alpha &&
                overlayColor.alpha() != alpha)
            {


                if (y + 1 < mask.height() &&
                   (y + 1) % frameHeigth != 0 &&
                   mask.pixelColor(x, y + 1).alpha() != alpha)
                {
                    QColor color = mask.pixelColor(x, y + 1);
                    newPicture.setPixelColor(x, y, color);
                }
                else if (y - 1 >= 0 &&
                        ((y - 1) % frameHeigth != 0 || y == 0) &&
                        mask.pixelColor(x, y - 1).alpha() != alpha)
                {
                    QColor color = mask.pixelColor(x, y - 1);
                    newPicture.setPixelColor(x, y, color);
                }
                else if (x + 1 < mask.width() &&
                        (x + 1) % frameWidth != 0 &&
                         mask.pixelColor(x + 1, y).alpha() != alpha)
                {
                    QColor color = mask.pixelColor(x + 1, y);
                    newPicture.setPixelColor(x, y, color);
                }
                else if (x - 1 >= 0 &&
                        ((x - 1) % frameWidth != 0 || x == 0) &&
                        mask.pixelColor(x - 1, y).alpha() != alpha)
                {
                    QColor color = mask.pixelColor(x - 1, y);
                    newPicture.setPixelColor(x, y, color);
                }
                else
                {
                    QColor color = mask.pixelColor(x, y);
                    newPicture.setPixelColor(x, y, color);
                }
            }
            else
            {
                QColor color = mask.pixelColor(x, y);
                newPicture.setPixelColor(x, y, color);
            }
        }
    }
    mask = newPicture;
}
