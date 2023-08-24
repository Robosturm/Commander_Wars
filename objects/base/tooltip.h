#ifndef TOOLTIP_H
#define TOOLTIP_H

#include <QObject>
#include <QTimer>

#include "objects/base/focusableobject.h"

class Tooltip;
using spTooltip = std::shared_ptr<Tooltip>;

class Tooltip : public FocusableObject
{
    Q_OBJECT
public:
    explicit Tooltip();
    virtual ~Tooltip();
    Q_INVOKABLE QString getTooltipText() const;
    Q_INVOKABLE void setTooltipText(const QString tooltipText);
signals:
    void sigHideTooltip();
    void sigStartTooltip();
    void sigStopTooltip();
    void sigStartHoveredTimer();
public slots:
    void restartTooltiptimer();
    void stopTooltiptimer();
    void showTooltip();
    void hideTooltip();
    void disableTooltip();
    void enableTooltip();
    void startHoveredTimer();
protected:
    virtual void looseFocusInternal() override;
    void removeTooltip();
private:
    static oxygine::spActor m_Tooltip;
#ifdef GRAPHICSUPPORT
    QString m_tooltipText;
    QTimer m_TooltipTimer;
    QTimer m_TooltipPauseTimer;
    bool m_disabled{false};
    bool m_mouseHovered{true};
#endif
};

Q_DECLARE_INTERFACE(Tooltip, "Tooltip");

#endif // TOOLTIP_H
