#ifndef WIKIVIEW_H
#define WIKIVIEW_H

#include <QObject>


#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "objects/base/panel.h"
#include "objects/base/textbox.h"
#include "objects/base/dropdownmenu.h"

#include "wiki/wikidatabase.h"

class WikiView;
typedef oxygine::intrusive_ptr<WikiView> spWikiView;

class WikiView : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit WikiView(qint32 viewWidth, qint32 viewHeigth);

signals:
    void sigSearch(bool onlyTag);
    void sigShowWikipage(WikiDatabase::pageData page);
public slots:
    void showWikipage(WikiDatabase::pageData page);
    void searchChanged(QString);
    void search(bool onlyTag);
    void tagChanged(qint32 item);
private:
    spTextbox m_SearchString;
    spDropDownmenu m_Tags;
    spPanel m_MainPanel;
};

#endif // WIKIVIEW_H
