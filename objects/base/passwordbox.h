#ifndef PASSWORDBOX_H
#define PASSWORDBOX_H

#include "objects/base/textbox.h"

class Passwordbox;
using spPasswordbox = std::shared_ptr<Passwordbox>;

class Passwordbox final : public Textbox
{
    Q_OBJECT
public:
    explicit Passwordbox(qint32 width, qint32 heigth = -1);
   virtual ~Passwordbox() = default;
    virtual void update(const oxygine::UpdateState& us) override;

private:
    QString getVisiblePasswordText();
};

Q_DECLARE_INTERFACE(Passwordbox, "Passwordbox");

#endif // PASSWORDBOX_H
