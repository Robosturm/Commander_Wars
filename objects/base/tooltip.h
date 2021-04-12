#ifndef TOOLTIP_H
#define TOOLTIP_H

#include <QObject>
#include <QTimer>

#include "objects/base/focusableobject.h"

class Tooltip;
typedef oxygine::intrusive_ptr<Tooltip> spTooltip;

class Tooltip : public FocusableObject
{
    Q_OBJECT
public:
    explicit Tooltip();
    virtual ~Tooltip();
signals:
    void sigHideTooltip();
    void sigStartTooltip();
    void sigStopTooltip();
public slots:
    QString getTooltipText() const;
    void setTooltipText(const QString &tooltipText);

    void restartTooltiptimer();
    void stopTooltiptimer();

    void showTooltip();
    void hideTooltip();
    void disableTooltip();
    void enableTooltip();
protected:
    virtual void looseFocusInternal() override;
    void removeTooltip();
protected:
    oxygine::spActor m_Tooltip;
private:
    QString m_tooltipText;
    QTimer m_TooltipTimer;
    bool m_disabled{false};
    bool m_mouseHovered{false};
};

#endif // TOOLTIP_H
