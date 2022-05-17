#pragma once

#include <QObject>
#include <QString>

#include "ui_reader/createdgui.h"

#include "coreengine/scriptvariables.h"
#include "coreengine/mainapp.h"

class MovementPlannerAddIn;
using spMovementPlannerAddIn = oxygine::intrusive_ptr<MovementPlannerAddIn>;

class MovementPlannerAddIn : public CreatedGui
{
    Q_OBJECT
public:
    explicit MovementPlannerAddIn();
    explicit MovementPlannerAddIn(QString addIn);
    virtual ~MovementPlannerAddIn() = default;

    QString getAddIn() const;

public slots:
    /**
     * @brief getVariables
     * @return
     */
    inline ScriptVariables* getVariables()
    {
        return &m_Variables;
    }
private:
    QString m_addIn;
    ScriptVariables m_Variables;
};

Q_DECLARE_INTERFACE(MovementPlannerAddIn, "MovementPlannerAddIn");
