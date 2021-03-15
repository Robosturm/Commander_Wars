#ifndef CHECKBOX_H
#define CHECKBOX_H

#include "objects/base/tooltip.h"


#include "3rd_party/oxygine-framework/oxygine-framework.h"

class Checkbox;
typedef oxygine::intrusive_ptr<Checkbox> spCheckbox;

class Checkbox : public Tooltip
{
    Q_OBJECT
public:
    explicit Checkbox();

    bool getChecked() const;
    void setChecked(bool Checked);
signals:
    void checkChanged(bool value);
public slots:
private:
    bool m_Checked{false};
};

#endif // CHECKBOX_H
