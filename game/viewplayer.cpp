#include "game/viewplayer.h"
#include "game/gamemap.h"

#include "menue/gamemenue.h"

Viewplayer::Viewplayer(GameMenue* pMenu, GameMap* pMap)
    : Player(pMap),
      m_input(pMap)
{
#ifdef GRAPHICSUPPORT
    setObjectName("Viewplayer");
#endif
    Interpreter::setCppOwnerShip(this);
    m_input.init(pMenu);
}

bool Viewplayer::getFieldVisible(qint32 x, qint32 y)
{
    
    switch (m_ViewType)
    {
        case ViewType::CurrentTeam:
        {
            auto* pCurrentPlayer = m_pMap->getCurrentPlayer();
            if (pCurrentPlayer != nullptr)
            {
                return pCurrentPlayer->getFieldVisible(x, y);
            }
            else
            {
                return m_pMap->getPlayer(0)->getFieldVisible(x, y);
            }
        }
        case ViewType::Map:
        {
            return true;
        }
        case ViewType::AllTeams:
        {
            for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
            {
                if (m_pMap->getPlayer(i)->getFieldVisible(x, y))
                {
                    return true;
                }
            }
            return false;
        }
        default:
        {
            for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
            {
                if (m_pMap->getPlayer(i)->getTeam() == m_ViewType)
                {
                    return m_pMap->getPlayer(i)->getFieldVisible(x, y);
                }
            }
        }
    }
    return false;
}

GameEnums::VisionType Viewplayer::getFieldVisibleType(qint32 x, qint32 y) const
{
    
    switch (m_ViewType)
    {
        case ViewType::CurrentTeam:
        {
            auto* pCurrentPlayer = m_pMap->getCurrentPlayer();
            if (pCurrentPlayer != nullptr)
            {
                return pCurrentPlayer->getFieldVisibleType(x, y);
            }
            else
            {
                return m_pMap->getPlayer(0)->getFieldVisibleType(x, y);
            }
        }
        case ViewType::Map:
        {
            return GameEnums::VisionType_Clear;
        }
        case ViewType::AllTeams:
        {
            GameEnums::VisionType ret = GameEnums::VisionType_Shrouded;
            for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
            {
                GameEnums::VisionType cur = m_pMap->getPlayer(i)->getFieldVisibleType(x, y);
                if (cur > ret)
                {
                    ret = cur;
                }
            }
            return ret;
        }
        default:
        {
            for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
            {
                if (m_pMap->getPlayer(i)->getTeam() == m_ViewType)
                {
                    return m_pMap->getPlayer(i)->getFieldVisibleType(x, y);
                }
            }
        }
    }
    return GameEnums::VisionType_Clear;
}

bool Viewplayer::getFieldDirectVisible(qint32 x, qint32 y)
{    
    switch (m_ViewType)
    {
        case ViewType::CurrentTeam:
        {
            auto* pCurrentPlayer = m_pMap->getCurrentPlayer();
            if (pCurrentPlayer != nullptr)
            {
                return pCurrentPlayer->getFieldDirectVisible(x, y);
            }
            else
            {
                return m_pMap->getPlayer(0)->getFieldDirectVisible(x, y);
            }
        }
        case ViewType::Map:
        {
            return true;
        }
        case ViewType::AllTeams:
        {
            for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
            {
                if (m_pMap->getPlayer(i)->getFieldDirectVisible(x, y))
                {
                    return true;
                }
            }
            return false;
        }
        default:
        {
            for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
            {
                if (m_pMap->getPlayer(i)->getTeam() == m_ViewType)
                {
                    return m_pMap->getPlayer(i)->getFieldDirectVisible(x, y);
                }
            }
        }
    }
    return false;
}

qint32 Viewplayer::getViewType() const
{
    return m_ViewType;
}

void Viewplayer::setViewType(const qint32 ViewType)
{
    m_ViewType = ViewType;
}

oxygine::spResAnim Viewplayer::getColorTableAnim() const
{
    return m_pMap->getCurrentPlayer()->getColorTableAnim();
}
