#ifndef MARKEDFIELDDATA_H
#define MARKEDFIELDDATA_H

#include <QObject>
#include <QVector>
#include <QPoint>
#include <QColor>

#include "3rd_party/oxygine-framework/oxygine-framework.h"

class MarkedFieldData;
typedef oxygine::intrusive_ptr<MarkedFieldData> spMarkedFieldData;
class MarkedFieldData : public QObject, public oxygine::ref_counter
{
    Q_OBJECT
public:
    explicit MarkedFieldData();
    virtual ~MarkedFieldData() = default;

    QVector<QPoint>* getPoints()
    {
        return &m_MarkedFields;
    }
    QVector<qint32>* getZInformation()
    {
        return &m_ZInformation;
    }
public slots:
    void addPoint(QPoint point)
    {
        m_MarkedFields.append(point);
    }
    void addPointXY(qint32 x, qint32 y)
    {
        m_MarkedFields.append(QPoint(x, y));
    }
    void addZInformation(qint32 info)
    {
       m_ZInformation.append(info);
    }
    bool getShowZData() const;
    void setShowZData(bool value);

    bool getAllFields() const;
    void setAllFields(bool value);
    QColor getColor() const;
    void setColor(const QColor &Color);

    QColor getZLabelColor() const;
    void setZLabelColor(const QColor &ZLabelColor);
    QString getZLabelText() const;
    void setZLabelText(const QString &ZLabelText);
private:
    bool allFields{false};
    bool showZData{false};
    QVector<QPoint> m_MarkedFields;
    /**
     * @brief m_ZInformation shown information over the selected field. For example the damage
     */
    QVector<qint32> m_ZInformation;

    QColor m_Color;

    QString m_ZLabelText;
    QColor m_ZLabelColor;
};

#endif // MARKEDFIELDDATA_H
