#ifndef CURSORDATA_H
#define CURSORDATA_H

#include <QObject>

class CursorData : public QObject
{
    Q_OBJECT
public:
    explicit CursorData();
    virtual ~CursorData() = default;
    CursorData(CursorData&& move) = default;
    CursorData(const CursorData& copy) = default;
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
    QString cursor{"cursor+default"};
    qint32 xOffset{0};
    qint32 yOffset{0};
    float scale{1.0};
};

#endif // CURSORDATA_H
