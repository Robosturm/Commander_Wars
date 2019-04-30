#ifndef BUILDLISTDIALOG_H
#define BUILDLISTDIALOG_H

#include <QObject>

#include "oxygine-framework.h"

class BuildListDialog;
typedef oxygine::intrusive_ptr<BuildListDialog> spBuildListDialog;

class BuildListDialog : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit BuildListDialog(qint32 player, QStringList buildList);
    virtual ~BuildListDialog() = default;
signals:
    void editFinished(qint32 player, QStringList color);
    void canceled();
public slots:

private:
    oxygine::spButton m_OkButton;
    oxygine::spButton m_ExitButton;

    QStringList m_CurrentBuildList;
    qint32 m_Player;
};

#endif // BUILDLISTDIALOG_H
