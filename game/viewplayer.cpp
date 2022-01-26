#include "game/viewplayer.h"
#include "game/gamemap.h"

Viewplayer::Viewplayer()
{
    setObjectName("Viewplayer");
}

bool Viewplayer::getFieldVisible(qint32 x, qint32 y)
{
    
    switch (m_ViewType)
    {
        case ViewType::CurrentTeam:
        {
            return m_pMap->getCurrentPlayer()->getFieldVisible(x, y);
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

GameEnums::VisionType Viewplayer::getFieldVisibleType(qint32 x, qint32 y)
{
    
    switch (m_ViewType)
    {
        case ViewType::CurrentTeam:
        {
            return m_pMap->getCurrentPlayer()->getFieldVisibleType(x, y);
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
            return m_pMap->getCurrentPlayer()->getFieldDirectVisible(x, y);
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

void Viewplayer::setViewType(const qint32 &ViewType)
{
    m_ViewType = ViewType;
}
