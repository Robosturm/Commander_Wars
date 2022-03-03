#ifndef DEFAULTWIKIPAGE_H
#define DEFAULTWIKIPAGE_H

#include "wiki/wikipage.h"

class DefaultWikipage;
using spDefaultWikipage = oxygine::intrusive_ptr<DefaultWikipage>;

class DefaultWikipage : public Wikipage
{
    Q_OBJECT
public:
    explicit DefaultWikipage(QString item);
    virtual ~DefaultWikipage() = default;
};

#endif // DEFAULTWIKIPAGE_H
