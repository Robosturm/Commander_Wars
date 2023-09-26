#pragma once

#include <QTextEdit>
#include <QKeyEvent>

class EventTextEdit;
using spEventTextEdit = std::shared_ptr<EventTextEdit>;

class EventTextEdit final : public QTextEdit
{
    Q_OBJECT
public:
    explicit EventTextEdit();
    virtual ~EventTextEdit() = default;
    virtual bool event(QEvent *event) override;
    bool getSingleLine() const;
    void setSingleLine(bool newSingleLine);
signals:
    void returnPressed();
private:
    bool m_singleLine{true};
};
