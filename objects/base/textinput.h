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
signals:
    void sigSetText(const QString & text);
public slots:
    QString getCurrentText() const;
    /**
     * @brief setCurrentText changes the text of this textbox
     * @param text
     */
    void setCurrentText(const QString & text);
    qint32 getCursorPosition() const;
    void setCursorPosition(qint32 position);
    virtual void focusedLost() override;

    static qint32 getClickedLinePosition(qint32 x, const QString & line, const QFont & font);
private slots:
    void editFinished();
protected:
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

