#ifndef PASSWORDBOX_H
#define PASSWORDBOX_H

#include "objects/base/textbox.h"

class Passwordbox;
typedef oxygine::intrusive_ptr<Passwordbox> spPasswordbox;

class Passwordbox final : public Textbox
{
    Q_OBJECT
public:
    explicit Passwordbox(qint32 width, qint32 heigth = -1);
    ~Passwordbox() = default;
    virtual void update(const oxygine::UpdateState& us) override;

private:
    QString getVisiblePasswordText();
};

#endif // PASSWORDBOX_H
