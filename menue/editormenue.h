#ifndef EDITORMENUE_H
#define EDITORMENUE_H

#include "menue/ingamemenue.h"

#include <QObject>

#include "objects/editorselection.h"

class EditorMenue : public InGameMenue
{
    Q_OBJECT
public:
    EditorMenue();
public slots:
    void cursorMoved();

private:
    spEditorSelection m_EditorSelection{new EditorSelection()};
};

#endif // EDITORMENUE_H
