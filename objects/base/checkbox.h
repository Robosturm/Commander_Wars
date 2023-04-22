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
    ~Checkbox() = default;
public slots:
    bool getChecked() const;
    void setChecked(bool Checked);
signals:
    void checkChanged(bool value);

private:
    bool m_Checked{false};
};

Q_DECLARE_INTERFACE(Checkbox, "Checkbox");


#endif // CHECKBOX_H
