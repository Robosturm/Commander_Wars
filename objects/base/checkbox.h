#ifndef CHECKBOX_H
#define CHECKBOX_H

#include "objects/base/tooltip.h"

class Checkbox;
using spCheckbox = oxygine::intrusive_ptr<Checkbox>;

class Checkbox : public Tooltip
{
    Q_OBJECT
public:
    explicit Checkbox();
    virtual ~Checkbox() = default;
    bool getChecked() const;
    void setChecked(bool Checked);
signals:
    void checkChanged(bool value);

private:
    bool m_Checked{false};
};

#endif // CHECKBOX_H
