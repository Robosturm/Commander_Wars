#ifndef DEFAULTWIKIPAGE_H
#define DEFAULTWIKIPAGE_H

#include "wiki/wikipage.h"

class DefaultWikipage;
typedef oxygine::intrusive_ptr<DefaultWikipage> spDefaultWikipage;

class DefaultWikipage : public Wikipage
{
    Q_OBJECT
public:
    explicit DefaultWikipage(QString item);
    virtual ~DefaultWikipage() = default;
};

#endif // DEFAULTWIKIPAGE_H
