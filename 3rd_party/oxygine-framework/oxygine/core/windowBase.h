#pragma once
#include <QObject>
#include <QKeyEvent>

class WindowBaseClass : public QObject
{
    Q_OBJECT
public:
    WindowBaseClass() = default;
    ~WindowBaseClass() = default;

    QSize size() const;
    qint32 x() const;
    qint32 y() const;
    QPoint position() const;
    void setPosition(qint32 x, qint32 y);
    void setPosition(QPoint pos);
    void setTitle(const QString & title);
    bool isActive() const;
    float devicePixelRatio() const;
    QPoint mapFromGlobal(QPoint pos) const;
    QPoint mapToGlobal(QPoint pos) const;
signals:
    void activeChanged();

protected:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);
};
