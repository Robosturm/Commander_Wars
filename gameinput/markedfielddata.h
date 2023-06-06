#ifndef MARKEDFIELDDATA_H
#define MARKEDFIELDDATA_H

#include <QObject>
#include <QVector>
#include <QPoint>
#include <QColor>

#include "3rd_party/oxygine-framework/oxygine/core/intrusive_ptr.h"

class MarkedFieldData;
using spMarkedFieldData = oxygine::intrusive_ptr<MarkedFieldData>;

class MarkedFieldData final : public QObject, public oxygine::ref_counter
{
    Q_OBJECT
public:
    struct ZInformation
    {
        qint32 singleValue;
        QVector<qint32> ownUnitValues;
        QVector<qint32> enemyUnitValues;
        QStringList valueNames;
        QColor enemyColor;
    };
    explicit MarkedFieldData();
    ~MarkedFieldData() = default;

    QVector<QPoint>* getPoints()
    {
        return &m_MarkedFields;
    }
    QVector<ZInformation>* getZInformation()
    {
        return &m_ZInformation;
    }
    Q_INVOKABLE QVector<QPoint> getJsPoints()
    {
        return m_MarkedFields;
    }
    Q_INVOKABLE void addPoint(QPoint point)
    {
        m_MarkedFields.append(point);
    }
    Q_INVOKABLE void addPointXY(qint32 x, qint32 y)
    {
        m_MarkedFields.append(QPoint(x, y));
    }
    Q_INVOKABLE void addZInformation(qint32 info)
    {
        ZInformation infoStruct;
        infoStruct.singleValue = info;
        m_ZInformation.append(infoStruct);
    }
    Q_INVOKABLE void addComplexZInformation(const QStringList valueNames,
                                const QVector<qint32> ownUnitValues,
                                const QVector<qint32> enemyUnitValues,
                                QColor enemyColor)
    {
        ZInformation infoStruct;
        infoStruct.ownUnitValues = ownUnitValues;
        infoStruct.enemyUnitValues = enemyUnitValues;
        infoStruct.valueNames = valueNames;
        infoStruct.enemyColor = enemyColor;
        m_ZInformation.append(infoStruct);
    }
    Q_INVOKABLE bool getShowZData() const;
    Q_INVOKABLE void setShowZData(bool value);
    Q_INVOKABLE bool getAllFields() const;
    Q_INVOKABLE void setAllFields(bool value);
    Q_INVOKABLE QColor getColor() const;
    Q_INVOKABLE void setColor(const QColor Color);
    Q_INVOKABLE QColor getZLabelColor() const;
    Q_INVOKABLE void setZLabelColor(const QColor ZLabelColor);
    Q_INVOKABLE QString getZLabelText() const;
    Q_INVOKABLE void setZLabelText(const QString ZLabelText);
    /**
     * @brief remove
     */
    Q_INVOKABLE void remove()
    {
    }
private:
    bool m_allFields{false};
    bool m_showZData{false};
    QVector<QPoint> m_MarkedFields;
    /**
         * @brief m_ZInformation shown information over the selected field. For example the damage
         */
    QVector<ZInformation> m_ZInformation;
    QColor m_Color;

    QString m_ZLabelText;
    QColor m_ZLabelColor;
};

#endif // MARKEDFIELDDATA_H
