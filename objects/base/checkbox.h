#ifndef CHECKBOX_H
#define CHECKBOX_H

#include "objects/base/tooltip.h"

class Checkbox;
using spCheckbox = oxygine::intrusive_ptr<Checkbox>;

class Checkbox final : public Tooltip
{
    Q_OBJECT
public:
    explicit Checkbox();
   virtual ~Checkbox() = default;
    Q_INVOKABLE bool getChecked() const;
    Q_INVOKABLE void setChecked(bool Checked);
signals:
    void checkChanged(bool value);

private:
    bool m_Checked{false};
};

Q_DECLARE_INTERFACE(Checkbox, "Checkbox");


#endif // CHECKBOX_H
