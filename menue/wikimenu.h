#ifndef WIKIMENU_H
#define WIKIMENU_H

#include "3rd_party/oxygine-framework/oxygine-framework.h"
#include "menue/basemenu.h"
#include "wiki/wikiview.h"

class Wikimenu;
using spWikimenu = oxygine::intrusive_ptr<Wikimenu>;

class Wikimenu : public Basemenu
{
    Q_OBJECT
public:
    explicit Wikimenu();
    virtual ~Wikimenu() = default;
signals:
    void sigExitMenue();
    void sigOnEnter();
public slots:
    void exitMenue();
    void onEnter();
    WikiView* getWikiView()
    {
        return m_pWikiView.get();
    }
private:
    spWikiView m_pWikiView;
};

#endif // WIKIMENU_H
