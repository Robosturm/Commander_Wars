#ifndef GAMEPLAYANDKEYS_H
#define GAMEPLAYANDKEYS_H

#include "qobject.h"

#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"

#include "objects/base/panel.h"
#include "objects/base/textbox.h"

class GameplayAndKeys;
using spGameplayAndKeys = oxygine::intrusive_ptr<GameplayAndKeys>;

class GameplayAndKeys : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit GameplayAndKeys(qint32 heigth);
    virtual ~GameplayAndKeys() = default;
signals:
    void sigShowSelectDefaultRules();
    void sigShowSelectDefaultBannlist();
public slots:
    void showSelectDefaultRules();
    void selectDefaultRules(QString filename);
    void showSelectDefaultBannlist();
    void selectSelectDefaultBannlist(QString filename);
private:
    spPanel m_pOptions;
    spTextbox m_pDefaultRuleFile;
    spTextbox m_pDefaultBannlistFile;
};

#endif // GAMEPLAYANDKEYS_H
