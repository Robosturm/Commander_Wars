#pragma once

#include "wiki/wikipage.h"
#include "wiki/wikidatabase.h"

class ActionWikipage;
using spActionWikipage = oxygine::intrusive_ptr<ActionWikipage>;

class ActionWikipage final : public Wikipage
{
    Q_OBJECT
public:
    explicit ActionWikipage(const PageData * item);
    ~ActionWikipage() = default;
};
