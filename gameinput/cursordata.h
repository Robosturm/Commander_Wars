#ifndef CURSORDATA_H
#define CURSORDATA_H

#include <QObject>


#include "3rd_party/oxygine-framework/oxygine-framework.h"

class CursorData;
typedef oxygine::intrusive_ptr<CursorData> spCursorData;
class CursorData : public QObject, public oxygine::ref_counter
{
    Q_OBJECT
public:
    explicit CursorData();
    virtual ~CursorData() = default;
signals:

public slots:
    QString getCursor() const;
    void setCursor(const QString &value);

    qint32 getXOffset() const;
    void setXOffset(const qint32 &value);

    qint32 getYOffset() const;
    void setYOffset(const qint32 &value);

    float getScale() const;
    void setScale(const float &value);
private:
    QString m_cursor{"cursor+default"};
    qint32 m_xOffset{0};
    qint32 m_yOffset{0};
    float m_scale{1.0};
};

#endif // CURSORDATA_H
