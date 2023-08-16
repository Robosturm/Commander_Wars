#ifndef DEFAULTWIKIPAGE_H
#define DEFAULTWIKIPAGE_H

#include "wiki/wikipage.h"

class DefaultWikipage;
using spDefaultWikipage = std::shared_ptr<DefaultWikipage>;

class DefaultWikipage final : public Wikipage
{
    Q_OBJECT
public:
    explicit DefaultWikipage(QString item);
    ~DefaultWikipage() = default;
};

#endif // DEFAULTWIKIPAGE_H
