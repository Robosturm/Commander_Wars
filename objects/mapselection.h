#ifndef MAPSELECTION_H
#define MAPSELECTION_H

#include <QObject>
#include <QTimer>

#include <QVector>
#include <QElapsedTimer>


#include "3rd_party/oxygine-framework/oxygine-framework.h"

class MapSelection;
typedef oxygine::intrusive_ptr<MapSelection> spMapSelection;

class MapSelection : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit MapSelection(qint32 heigth, qint32 width, QString folder);
    virtual ~MapSelection();

    QString getCurrentFolder()
    {
        return m_currentFolder;
    }
    QString getCurrentFile()
    {
        return m_currentFolder + m_currentItem;
    }
    virtual void update(const oxygine::UpdateState& us) override;
    /**
     * @brief setSelection
     * @param folder
     * @param files
     */
    void setSelection(QString folder, QStringList files);
    /**
     * @brief setCurrentItem
     * @param item
     */
    void setCurrentItem(QString item);
signals:    
    void itemChanged(QString item);
    void changeSelection(qint32 index);
    void itemClicked(QString item);
    void sigStartItemChangeTimer();
public slots:
    void startItemChangeTimer();
    void changeFolder(QString folder);
    void updateSelection(qint32 startIndex);
    void itemChangeTimerExpired();
private:
    QString m_currentFolder;
    QString m_currentItem;
    QString m_lastItem;
    qint32 m_currentIdx{0};
    QVector<oxygine::spTextField> m_Items;
    qint32 m_itemCount{0};
    static const qint32 m_itemHeigth{35};
    QStringList m_Files;
    qint32 m_currentStartIndex{0};
    oxygine::spBox9Sprite m_SelectedItem;
    qint32 m_spin = 0;
    QElapsedTimer m_timer;
    QTimer m_itemChangedTimer;
    bool m_itemClicked{false};
};

#endif // MAPSELECTION_H
