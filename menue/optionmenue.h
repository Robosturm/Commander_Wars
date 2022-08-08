#ifndef OPTIONMENUE_H
#define OPTIONMENUE_H

#include <QVector>

#include "3rd_party/oxygine-framework/oxygine/actor/TextField.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Sprite.h"

#include "objects/base/panel.h"
#include "objects/gameplayandkeys.h"
#include "objects/base/checkbox.h"
#include "objects/base/dropdownmenu.h"
#include "menue/basemenu.h"

class OptionMenue;
using spOptionMenue = oxygine::intrusive_ptr<OptionMenue>;

class OptionMenue : public Basemenu
{
    Q_OBJECT
public:
    explicit OptionMenue(const QString & xmlFile);
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
    void sigUpdateModFilter(QString tag);
    void sigLoadModInfo(oxygine::Box9Sprite* pPtrBox,
                        QString name, QString description, QString version,
                        QStringList compatibleMods, QStringList incompatibleMods, QStringList requiredMods,
                        bool isComsetic, QStringList modTags, QString thumbnail);
    void sigShowGamepadInfo();
    void sigShowResetBox();
public slots:
    void exitMenue();
    void showMods();
    void restart();
    void showSettings();
    void showGameplayAndKeys();
    void selectMods(qint32 item);
    void reloadSettings(const QString & xmlFile);
    void updateModCheckboxes();
    void updateModFilter(QString tag);
    void onReset();
    void showResetBox();
protected slots:
    virtual void onEnter() override;
private slots:
    void loadModInfo(oxygine::Box9Sprite* pPtrBox,
                     QString name, QString description, QString version,
                     QStringList compatibleMods, QStringList incompatibleMods, QStringList requiredMods,
                     bool isComsetic, QStringList modTags, QString thumbnail);
    void showGamepadInfo();
private:
    void updateModSelection();
    void loadModPanels();
private:
    QString m_xmlFile;
    // spGameplayAndKeys m_pGameplayAndKeys;
    // spPanel m_pOptions;

    struct GamemodeMods
    {
        GamemodeMods(QStringList enableMods, QStringList disableMods)
            : m_enableMods(enableMods),
              m_disableMods(disableMods)
        {
        }
        QStringList m_enableMods;
        QStringList m_disableMods;
    };
    static QVector<GamemodeMods> m_gamemodeMods;
    spPanel m_pMods;
    spPanel m_pModDescription;
    spDropDownmenu m_pModSelection;
    oxygine::spActor m_ModSelector;
    oxygine::spTextField m_ModDescriptionText;
    QVector<oxygine::spActor> m_ModBoxes;
    QVector<spCheckbox> m_ModCheckboxes;
    oxygine::spResAnim m_modThumbnailAnim;
    oxygine::spSprite m_modThumbnail;

    QStringList m_languages;

    bool m_restartNeeded{false};
};

Q_DECLARE_INTERFACE(OptionMenue, "OptionMenue");

#endif // OPTIONMENUE_H
