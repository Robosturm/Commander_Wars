#ifndef BASEMENU_H
#define BASEMENU_H

#include <QObject>

class Basemenu : public QObject
{
    Q_OBJECT
public:
    explicit Basemenu();

signals:
    void sigOnUpdate();
public slots:
    bool getFocused() const;
    virtual void setFocused(bool Focused);
protected:
    bool m_Focused{true};
};

#endif // BASEMENU_H
