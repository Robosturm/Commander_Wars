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
signals:
    void sigSetText(const QString text);
public slots:
    virtual void focusedLost() override;

private slots:
    void editFinished();
protected:
    static qint32 getClickedLinePosition(qint32 x, const QString & line, const QFont & font);
    virtual bool doHandleEvent(QEvent *event) override;
    virtual bool onEditFinished() = 0;
    virtual void focused() override;
    virtual void looseFocusInternal() override;
    /**
     * @brief getDrawText
     * @param text
     * @return the draw text in pseudo html format
     */
    QString getDrawText(const QString & text);
    virtual void inputMethodQuery(Qt::InputMethodQuery query, QVariant arg) override;
protected:
    EventTextEdit* m_lineEdit;
    bool m_LineEditFocused{false};
    QElapsedTimer m_toggle;
    qint32 m_focusPosition{-1};
    bool m_multiline{false};
};

Q_DECLARE_INTERFACE(TextInput, "TextInput");
