#pragma once

#include "wiki/wikipage.h"

class ActionWikipage;
using spActionWikipage = oxygine::intrusive_ptr<ActionWikipage>;

class ActionWikipage final : public Wikipage
{
    Q_OBJECT
public:
    explicit ActionWikipage(QString item);
    ~ActionWikipage() = default;
};
