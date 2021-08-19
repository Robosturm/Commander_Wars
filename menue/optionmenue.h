#ifndef OPTIONMENUE_H
#define OPTIONMENUE_H

#include <QVector>

#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "objects/base/panel.h"
#include "objects/gameplayandkeys.h"
#include "objects/base/checkbox.h"
#include "menue/basemenu.h"

class OptionMenue;
using spOptionMenue = oxygine::intrusive_ptr<OptionMenue>;

class OptionMenue : public Basemenu
{
    Q_OBJECT
public:
    explicit OptionMenue();
    virtual ~OptionMenue() = default;

    static void showSoundOptions(spPanel pOwner, qint32 sliderOffset, qint32 & y, QObject* pSignalOwner);
signals:
    void sigExitMenue();
    void sigShowMods();
    void sigShowSettings();
    void sigShowGameplayAndKeys();
    void sigChangeScreenSize(qint32 width, qint32 heigth);
    void sigReloadSettings();
    void sigUpdateModCheckboxes();
    void sigOnEnter();
    void sigUpdateModFilter(QString tag);
    void sigLoadModInfo(oxygine::Box9Sprite* pPtrBox,
                        QString name, QString description, QString version,
                        QStringList compatibleMods, QStringList incompatibleMods, QStringList requiredMods,
                        bool isComsetic, QStringList modTags, QString thumbnail);
public slots:
    void exitMenue();
    void showMods();
    void restart();
    void showSettings();
    void showGameplayAndKeys();
    void selectMods(qint32 item);
    void reloadSettings();
    void updateModCheckboxes();
    void onEnter();
    void updateModFilter(QString tag);
private slots:
    void loadModInfo(oxygine::Box9Sprite* pPtrBox,
                     QString name, QString description, QString version,
                     QStringList compatibleMods, QStringList incompatibleMods, QStringList requiredMods,
                     bool isComsetic, QStringList modTags, QString thumbnail);
private:
    spGameplayAndKeys m_pGameplayAndKeys;

    spPanel m_pOptions;
    spPanel m_pMods;
    spPanel m_pModDescription;
    oxygine::spActor m_ModSelector;
    oxygine::spTextField m_ModDescriptionText;
    QVector<oxygine::spActor> m_ModBoxes;
    QVector<spCheckbox> m_ModCheckboxes;
    oxygine::spResAnim m_modThumbnailAnim;
    oxygine::spSprite m_modThumbnail;

    QStringList languages;

    bool restartNeeded{false};
};

#endif // OPTIONMENUE_H
