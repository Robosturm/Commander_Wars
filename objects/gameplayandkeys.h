#ifndef GAMEPLAYANDKEYS_H
#define GAMEPLAYANDKEYS_H

#include "qobject.h"

#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "objects/base/panel.h"
#include "objects/base/textbox.h"

class GameplayAndKeys;
typedef oxygine::intrusive_ptr<GameplayAndKeys> spGameplayAndKeys;

class GameplayAndKeys : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit GameplayAndKeys(qint32 heigth);
    virtual ~GameplayAndKeys() = default;
signals:
    void sigShowSelectDefaultRules();
public slots:
    void showSelectDefaultRules();
    void selectDefaultRules(QString filename);
private:
    spPanel m_pOptions;
    spTextbox m_pDefaultRuleFile;
};

#endif // GAMEPLAYANDKEYS_H
