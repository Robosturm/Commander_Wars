#ifndef DEFAULTWIKIPAGE_H
#define DEFAULTWIKIPAGE_H

#include "wiki/wikipage.h"

class DefaultWikipage : public Wikipage
{
    Q_OBJECT
public:
    DefaultWikipage(QString item);
};

#endif // DEFAULTWIKIPAGE_H
