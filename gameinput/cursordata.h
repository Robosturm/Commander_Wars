#ifndef CURSORDATA_H
#define CURSORDATA_H

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine/core/intrusive_ptr.h"

class CursorData;
using spCursorData = oxygine::intrusive_ptr<CursorData>;

class CursorData final : public QObject, public oxygine::ref_counter
{
    Q_OBJECT
public:
    explicit CursorData();
   virtual ~CursorData() = default;

    Q_INVOKABLE QString getCursor() const;
    Q_INVOKABLE void setCursor(const QString value);
    Q_INVOKABLE qint32 getXOffset() const;
    Q_INVOKABLE void setXOffset(const qint32 value);
    Q_INVOKABLE qint32 getYOffset() const;
    Q_INVOKABLE void setYOffset(const qint32 value);
    Q_INVOKABLE float getScale() const;
    Q_INVOKABLE void setScale(const float value);

private:
    QString m_cursor{"cursor+default"};
    qint32 m_xOffset{0};
    qint32 m_yOffset{0};
    float m_scale{1.0};
};

Q_DECLARE_INTERFACE(CursorData, "CursorData");

#endif // CURSORDATA_H
