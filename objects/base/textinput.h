#pragma once

#include "objects/base/tooltip.h"
#include "objects/base/EventTextEdit.h"

#include <QObject>
#include <QElapsedTimer>

class TextInput : public Tooltip
{
    Q_OBJECT
public:
    enum
    {
        BLINKFREQG = 250
    };
    explicit TextInput();
    virtual ~TextInput();
    Q_INVOKABLE QString getCurrentText() const;
    /**
     * @brief setCurrentText changes the text of this textbox
     * @param text
     */
    Q_INVOKABLE void setCurrentText(const QString text);
    Q_INVOKABLE qint32 getCursorPosition() const;
    Q_INVOKABLE void setCursorPosition(qint32 position);
    Q_INVOKABLE bool getReadonly() const;
    /**
     * @brief setReadonly changes the readonly mode of this textbox.
     * In readonly mode the shown text can't be modified by user input,
     * but it can still be selected and copied, the texteditor can be focused
     * and the cursor can still be moved.
     * @param readonly
     */
    Q_INVOKABLE void setReadonly(bool readonly);
signals:
    void sigSetText(const QString text);
    void sigSetReadonlyToEdit(const bool editable);
public slots:
    void focusedLost() override;

private slots:
    void editFinished();
protected:
    static qint32 getClickedLinePosition(qint32 x, const QString & line, const QFont & font);
    bool doHandleEvent(QEvent *event) override;
    virtual bool onEditFinished() = 0;
    void focused() override;
    void looseFocusInternal() override;
    /**
     * @brief getDrawText
     * @param text
     * @return the draw text in pseudo html format
     */
    QString getDrawText(const QString & text);
    void inputMethodEvent(QInputMethodEvent* event) override;
    void inputMethodQueryEvent(QInputMethodQueryEvent* event) override;
protected:
    std::shared_ptr<EventTextEdit> m_lineEdit;
    bool m_LineEditFocused{false};
    QElapsedTimer m_toggle;
    qint32 m_focusPosition{-1};
    bool m_multiline{false};
    bool m_readonly{false};
};

Q_DECLARE_INTERFACE(TextInput, "TextInput");
