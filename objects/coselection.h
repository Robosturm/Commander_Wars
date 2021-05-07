#ifndef COSELECTION_H
#define COSELECTION_H

#include <QObject>

#include <QStringList>

#include <QVector>

#include <QColor>


#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "objects/base/panel.h"
#include "objects/base/label.h"

class COSelection;
typedef oxygine::intrusive_ptr<COSelection> spCOSelection;

class COSelection : public QObject, public oxygine::Actor
{
    Q_OBJECT

    static constexpr float scale = 1.5f;
public:
    explicit COSelection(QSize maxSize, QStringList coids = {});
    virtual ~COSelection();

signals:
    void armySelectedChange(QString army);
    void coSelected(QString coid);
    void sigHoveredCOChanged(QString coid);
public slots:
    void colorChanged(QColor color);
    void armyChanged(QString army);
    void hoveredCOChanged(QString);
protected:
    void armyBannerClicked(QString army, qint32 index);
    void loadArmy(QString army, qint32& bannerX, qint32& y, qint32 i);
    void addCO(QString coid, QString COArmy, qint32 x, qint32 y, QString army);
private:
    QStringList m_Armies;
    QVector<oxygine::spSprite> m_ArmyBanners;
    spPanel m_ArmyBannerPanel;
    QVector<oxygine::spActor> m_COFields;
    oxygine::spSprite m_Cursor;
    QStringList m_CoIDs;
    QString m_CurrentCO;
    QColor m_CurrentColor;
    spPanel m_CoFieldPanel;
    spLabel m_COName;
    oxygine::spTextField m_COBio;
    oxygine::spTextField m_CODesc;
    spLabel m_COPower;
    spLabel m_COSuperpower;
    oxygine::spSprite m_pCurrentCO;
    spPanel m_CoDescription;
    QStringList m_Coids;
};

#endif // COSELECTION_H
