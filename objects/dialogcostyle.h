#ifndef DIALOGCOSTYLE_H
#define DIALOGCOSTYLE_H

#include <QObject>

#include <QImage>

#include "oxygine-framework.h"

#include "objects/colorselector.h"

#include "objects/panel.h"

#include "objects/dropdownmenu.h"

class DialogCOStyle;
typedef oxygine::intrusive_ptr<DialogCOStyle> spDialogCOStyle;

class DialogCOStyle : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit DialogCOStyle(QString coid);
    virtual ~DialogCOStyle() = default;

    void addCOStyle(QString style, bool select);
    virtual void update(const oxygine::UpdateState& us) override;
signals:
    void sigFinished();
    void sigCancel();

    void sigCOStyleChanged(qint32 index);
public slots:
    void changeCOStyle(qint32 index);

    void selecetedColorChanged(QColor color);
private:
    oxygine::spBox9Sprite m_pSpriteBox;
    spColorSelector m_pColorSelector;
    spPanel m_pCOPanel;
    QVector<oxygine::spResAnim> m_pResAnims;
    QVector<oxygine::spSprite> m_pCOSprites;
    QVector<oxygine::spBox9Sprite> m_pCOBoxes;

    QStringList m_Styles;
    spDropDownmenu m_pPredefinedStyles;

    spPanel m_pPixelPanel;
    oxygine::spColorRectSprite m_PixelsSelector;
    QVector<oxygine::spColorRectSprite> m_Pixels;
    QImage baseColorTable;
    qint32 currentPixel = 0;

    QString m_currentCOID;
    QString m_ResFilePath;
    QImage colorTable;
    QImage maskTable;
    qint32 m_CurrentIndex{0};
    bool useColorBox{false};
    bool m_update{false};

};

#endif // DIALOGCOSTYLE_H
