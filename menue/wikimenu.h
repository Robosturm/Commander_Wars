#ifndef WIKIMENU_H
#define WIKIMENU_H

#include <QObject>

#include "oxygine-framework.h"

#include "objects/panel.h"

#include "objects/textbox.h"
#include "objects/dropdownmenu.h"

#include "wiki/wikidatabase.h"

class Wikimenu : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit Wikimenu();
signals:
    void sigExitMenue();
    void sigSearch(bool onlyTag);
    void sigShowWikipage(WikiDatabase::pageData page);
public slots:
    void exitMenue();
    void showWikipage(WikiDatabase::pageData page);
    void searchChanged(QString);
    void search(bool onlyTag);
    void tagChanged(qint32 item);
private:
    spTextbox m_SearchString;
    spDropDownmenu m_Tags;
    spPanel m_MainPanel;
};

#endif // WIKIMENU_H
