#pragma once

#include <QTextEdit>
#include <QKeyEvent>

class EventTextEdit final : public QTextEdit
{
    Q_OBJECT
public:
    EventTextEdit() = default;
   virtual ~EventTextEdit() = default;
    virtual bool event(QEvent *event) override;
    bool getSingleLine() const;
    void setSingleLine(bool newSingleLine);
signals:
    void returnPressed();
private:
    bool m_singleLine{true};
};
