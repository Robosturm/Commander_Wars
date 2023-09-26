#pragma once

#include <QObject>
#include <QElapsedTimer>

#include "3rd_party/oxygine-framework/oxygine/actor/TextField.h"
#include "3rd_party/oxygine-framework/oxygine/text_utils/Node.h"

#include "objects/base/textinput.h"
#include "objects/base/panel.h"

class MultilineTextbox;
using spMultilineTextbox = std::shared_ptr<MultilineTextbox>;

class MultilineTextbox final : public TextInput
{
    Q_OBJECT
public:
    explicit MultilineTextbox(qint32 width, qint32 heigth);
    virtual ~MultilineTextbox() = default;
    virtual void update(const oxygine::UpdateState& us) override;
    /**
     * @brief setEnabled
     * @param value
     */
    virtual void setEnabled(bool value) override;
signals:
    void sigTextChanged(QString text);
    void sigEnterPressed(QString text);
public slots:
    virtual void focusedLost() override;
protected:
    virtual bool onEditFinished() override;
private:
    bool getNewFocusPosition(oxygine::text::Node* pNode, qint32 x, qint32 y);
    bool calculateNewFocusPosition(oxygine::text::Node* pNode, qint32 x, qint32 y);
protected:
    spPanel m_pPanel;
    oxygine::spTextField m_textfield;
private:
    QString m_lastDrawText;
};
