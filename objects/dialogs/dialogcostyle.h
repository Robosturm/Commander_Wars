#ifndef DIALOGCOSTYLE_H
#define DIALOGCOSTYLE_H

#include <QObject>
#include <QImage>

#include "3rd_party/oxygine-framework/oxygine/actor/Button.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Box9Sprite.h"
#include "3rd_party/oxygine-framework/oxygine/actor/ColorRectSprite.h"

#include "objects/base/colorselector.h"
#include "objects/base/panel.h"
#include "objects/base/dropdownmenu.h"

class DialogCOStyle;
using spDialogCOStyle = oxygine::intrusive_ptr<DialogCOStyle>;

class DialogCOStyle : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit DialogCOStyle(QString coid);
    virtual ~DialogCOStyle() = default;

    void addCOStyle(QString style, bool select);
signals:
    void sigFinished();
    void sigCancel();
    void sigCOStyleChanged(qint32 index);
    void sigSaveCustomStyle();
    void sigDeleteCustomStyle();
public slots:
    void changeCOStyle(qint32 index);
    void selecetedColorChanged(QColor color);
    void saveCustomStyle();
    void deleteCustomStyle();
private slots:
    void remove();
private:
    void updateSprites();
    void loadAltsForStyle();
private:
    oxygine::spButton m_pOkButton;
    oxygine::spButton m_pDeleteButton;
    oxygine::spBox9Sprite m_pSpriteBox;
    spColorSelector m_pColorSelector;
    spPanel m_pCOPanel;
    QVector<oxygine::spResAnim> m_pResAnims;
    QVector<oxygine::spSprite> m_pCOSprites;
    QVector<oxygine::spResAnim> m_pMiniResAnims;
    QVector<oxygine::spSprite> m_pMiniCOSprites;
    QVector<oxygine::spResAnim> m_pFaceResAnims;
    QVector<oxygine::spSprite> m_pFaceCOSprites;
    QVector<oxygine::spBox9Sprite> m_pCOBoxes;
    QStringList m_Styles;
    spDropDownmenu m_pPredefinedStyles;
    spPanel m_pPixelPanel;
    oxygine::spColorRectSprite m_PixelsSelector;
    QVector<oxygine::spColorRectSprite> m_Pixels;
    QImage m_baseColorTable;
    qint32 m_currentPixel = 0;
    qint32 m_boxWidth = 10;
    QString m_currentCOID;
    QString m_ResFilePath;
    QImage m_colorTable;
    QImage m_maskTable;
    QImage m_customMaskTable;
    qint32 m_CurrentIndex{-1};
    bool m_useColorBox{false};

};

#endif // DIALOGCOSTYLE_H
