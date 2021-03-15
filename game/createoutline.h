#ifndef CREATEOUTLINE_H
#define CREATEOUTLINE_H


#include "3rd_party/oxygine-framework/oxygine-framework.h"

class CreateOutline
{
public:
    static void addCursorRangeOutline(oxygine::spActor pOutlineSprite, QString prefix, qint32 range, QColor color);
private:
    static void createOuterLeftRightOutline(oxygine::spActor pOutlineSprite, QString prefix, qint32 range, QColor color);
    static void createOuterTopBottomOutline(oxygine::spActor pOutlineSprite, QString prefix, qint32 range, QColor color);
    static void createOutline(oxygine::spActor pOutlineSprite, QString prefix, qint32 i, qint32 range, QColor color);
};

#endif // CREATEOUTLINE_H
