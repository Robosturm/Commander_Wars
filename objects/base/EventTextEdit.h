#pragma once

#include <QTextEdit>
#include <QKeyEvent>
#include <QInputMethodEvent>

class EventTextEdit;
using spEventTextEdit = std::shared_ptr<EventTextEdit>;

class EventTextEdit final : public QTextEdit
{
    Q_OBJECT
public:
    explicit EventTextEdit();
    virtual ~EventTextEdit() = default;
    virtual bool event(QEvent *event) override;
protected:
    virtual void inputMethodEvent(QInputMethodEvent *event) override;
public:
    bool getSingleLine() const;
    void setSingleLine(bool newSingleLine);
    /**
     * @brief setEditableKeys when disabled all key presses and input method
     * events that would modify the text are swallowed, while cursor movement,
     * selection and copy key presses are still processed by the base class.
     * @param editableKeys
     */
    void setEditableKeys(bool editableKeys);
    bool getEditableKeys() const;
signals:
    void returnPressed();
private:
    bool isEditingKeyPress(QKeyEvent *keyEvent) const;
private:
    bool m_singleLine{true};
    bool m_editableKeys{true};
};
