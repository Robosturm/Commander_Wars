#ifndef EDITORSELECTION_H
#define EDITORSELECTION_H

#include "oxygine-framework.h"
#include <QObject>
#include <QVector>
#include <QVector3D>
#include "game/terrain.h"
#include "game/gamemap.h"

class EditorSelection;
typedef oxygine::intrusive_ptr<EditorSelection> spEditorSelection;

class EditorSelection : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    /**
     * @brief The EditorMode enum describes what we want to place at the moment
     */
    enum class EditorMode
    {
        Terrain = 0,
        Building,
        Unit,
    };

    explicit EditorSelection();

    void updateTerrainView();
    inline EditorMode getCurrentMode() const
    {
        return m_Mode;
    }
    inline QString getCurrentTerrainID()
    {
        return m_Terrains.at(m_selectedIndex.x() + m_selectedIndex.y() * m_selectedIndex.z())->getTerrainID();
    }
signals:
    sigClicked(qint32 x, qint32 y);
public slots:
    void Clicked(qint32 x, qint32 y);
private:
    // small hints for the ui
    static const qint32 frameSize = 30;
    static const qint32 startH = frameSize + GameMap::Imagesize;
    static const float xFactor;
    static const float yFactor;
    EditorMode m_Mode{EditorMode::Terrain};
    qint32 m_StartIndex{0};
    QVector<spTerrain> m_Terrains;
    oxygine::spSprite m_Box;
    oxygine::spSprite m_CurrentSelector;
    QVector3D m_selectedIndex{0, 0, 0};
};

#endif // EDITORSELECTION_H
