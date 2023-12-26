#include <QDataStream>
#include <QFile>

#include "coreengine/mainapp.h"

#include "game/gamemap.h"
#include "game/player.h"
#include "game/co.h"
#include "game/building.h"
#include "game/unit.h"

#include "menue/editormenue.h"

void GameMap::importAW4Map(QString file, EditorMenue* pMenu)
{
    quint8 sign = 0;
    if (QFile::exists(file))
    {
        clearMap();

        QFile data(file);
        data.open(QFile::ReadOnly);
        QDataStream stream(&data);
        stream.setVersion(QDataStream::Version::Qt_6_5);
        stream >> sign;
        stream >> sign;
        qint32 width = sign;
        stream >> sign;
        qint32 heigth = sign;
        // load 5 players :)
        for (qint32 i = 0; i < 9; i++)
        {
            m_players.append(MemoryManagement::create<Player>(this));
            m_players[i]->init();
            if (i == 0)
            {
                m_players[i]->setBaseGameInput(BaseGameInputIF::createAi(this, GameEnums::AiTypes::AiTypes_Human));
                m_players[i]->setControlType(GameEnums::AiTypes::AiTypes_Human);
            }
            else
            {
                m_players[i]->setBaseGameInput(BaseGameInputIF::createAi(this, GameEnums::AiTypes::AiTypes_Normal));
                m_players[i]->setControlType(GameEnums::AiTypes::AiTypes_Normal);
            }
        }

        m_fields.reserve(heigth);
        m_rowSprites.reserve(heigth);
        for (qint32 y = 0; y < heigth; y++)
        {
            m_fields.push_back(std::vector<spTerrain>(width, spTerrain()));
            auto pActor = MemoryManagement::create<oxygine::Actor>();
            pActor->setPriority(static_cast<qint32>(Mainapp::ZOrder::Terrain) + y);
            m_rowSprites.push_back(pActor);
            addChild(pActor);
            for (qint32 x = 0; x < width; x++)
            {
                spTerrain pTerrain = Terrain::createTerrain(GameMap::PLAINS, x, y, "", this);
                m_rowSprites[y]->addChild(pTerrain);
                m_fields[y][x] = pTerrain;
                pTerrain->setPosition(x * m_imagesize, y * m_imagesize);
            }
        }
        // read map start byte
        stream >> sign;

        for (qint32 x = 0; x < width; x++)
        {
            for (qint32 y = 0; y < heigth; y++)
            {
                quint8 terrainId;
                stream >> terrainId;
                quint8 buildingPlayer;
                stream >> buildingPlayer;
                quint8 unitId;
                stream >> unitId;
                quint8 aiSetting;
                stream >> aiSetting;
                quint8 unitPlayer;
                stream >> unitPlayer;
                quint8 coUnit;
                stream >> coUnit;
                switch (terrainId)
                {
                    // buildings
                    case 17:
                    {
                        spBuilding pBuilding = MemoryManagement::create<Building>("TOWN", this);
                        if (buildingPlayer == 0)
                        {
                            pBuilding->setOwner(nullptr);
                        }
                        else
                        {
                            pBuilding->setOwner(getPlayer(buildingPlayer));
                        }
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 18:
                    {
                        spBuilding pBuilding = MemoryManagement::create<Building>("FACTORY", this);
                        if (buildingPlayer == 0)
                        {
                            pBuilding->setOwner(nullptr);
                        }
                        else
                        {
                            pBuilding->setOwner(getPlayer(buildingPlayer));
                        }
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 19:
                    {
                        spBuilding pBuilding = MemoryManagement::create<Building>("HARBOUR", this);
                        if (buildingPlayer == 0)
                        {
                            pBuilding->setOwner(nullptr);
                        }
                        else
                        {
                            pBuilding->setOwner(getPlayer(buildingPlayer));
                        }
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 20:
                    {
                        spBuilding pBuilding = MemoryManagement::create<Building>("AIRPORT", this);
                        if (buildingPlayer == 0)
                        {
                            pBuilding->setOwner(nullptr);
                        }
                        else
                        {
                            pBuilding->setOwner(getPlayer(buildingPlayer));
                        }
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 21:
                    {
                        spBuilding pBuilding = MemoryManagement::create<Building>("RADAR", this);
                        if (buildingPlayer == 0)
                        {
                            pBuilding->setOwner(nullptr);
                        }
                        else
                        {
                            pBuilding->setOwner(getPlayer(buildingPlayer));
                        }
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 22:
                    {
                        spBuilding pBuilding = MemoryManagement::create<Building>("TOWER", this);
                        if (buildingPlayer == 0)
                        {
                            pBuilding->setOwner(nullptr);
                        }
                        else
                        {
                            pBuilding->setOwner(getPlayer(buildingPlayer));
                        }
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 23:
                    {
                        spBuilding pBuilding = MemoryManagement::create<Building>("TEMPORARY_HARBOUR", this);
                        if (buildingPlayer == 0)
                        {
                            pBuilding->setOwner(nullptr);
                        }
                        else
                        {
                            pBuilding->setOwner(getPlayer(buildingPlayer));
                        }
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 24:
                    {
                        spBuilding pBuilding = MemoryManagement::create<Building>("TEMPORARY_AIRPORT", this);
                        if (buildingPlayer == 0)
                        {
                            pBuilding->setOwner(nullptr);
                        }
                        else
                        {
                            pBuilding->setOwner(getPlayer(buildingPlayer));
                        }
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 25:
                    {
                        spBuilding pBuilding;
                        if (buildingPlayer == 0)
                        {
                            pBuilding = MemoryManagement::create<Building>("SILO_ROCKET", this);
                            pBuilding->setOwner(nullptr);
                        }
                        else
                        {
                            pBuilding = MemoryManagement::create<Building>("HQ", this);
                            pBuilding->setOwner(getPlayer(buildingPlayer));
                        }
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    // terrain
                    case 0:
                    {
                        replaceTerrain(GameMap::PLAINS, x, y);
                        break;
                    }
                    case 1:
                    {
                        replaceTerrain("FOREST", x, y);
                        break;
                    }
                    case 2:
                    {
                        replaceTerrain("MOUNTAIN", x, y);
                        break;
                    }
                    case 3:
                    {
                        replaceTerrain("WASTELAND", x, y);
                        break;
                    }
                    case 4:
                    {
                        replaceTerrain("RUIN", x, y);
                        break;
                    }
                    case 5:
                    {
                        replaceTerrain("FIRE", x, y);
                        break;
                    }
                    case 6:
                    {
                        replaceTerrain("SEA", x, y);
                        break;
                    }
                    case 7:
                    {
                        replaceTerrain("REAF", x, y, false, false, false);
                        break;
                    }
                    case 8:
                    {
                        replaceTerrain("ROUGH_SEA", x, y, false, false, false);
                        break;
                    }
                    case 9:
                    {
                        replaceTerrain("FOG", x, y, false, false, false);
                        break;
                    }
                    case 10:
                    {
                        replaceTerrain("STREET", x, y);
                        break;
                    }
                    case 11:
                    {
                        replaceTerrain("SEA", x, y, false, false, false);
                        replaceTerrain("BRIDGE1", x, y, true, false, false);
                        break;
                    }
                    case 12:
                    {
                        replaceTerrain("RIVER", x, y);
                        break;
                    }
                    case 13:
                    {
                        replaceTerrain("PLASMA", x, y);
                        break;
                    }
                    case 14:
                    {
                        replaceTerrain("METEOR", x, y);
                        break;
                    }
                    case 15:
                    {
                        replaceTerrain("BEACH", x, y);
                        break;
                    }
                    default:
                    {
                        replaceTerrain(GameMap::PLAINS, x, y);
                        break;
                    }
                }
                switch (unitId)
                {
                    case 1:
                    {
                        spUnit pUnit = MemoryManagement::create<Unit>("INFANTRY", getPlayer(unitPlayer), false, this);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 2:
                    {
                        spUnit pUnit = MemoryManagement::create<Unit>("MECH", getPlayer(unitPlayer), false, this);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 3:
                    {
                        spUnit pUnit = MemoryManagement::create<Unit>("MOTORBIKE", getPlayer(unitPlayer), false, this);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 4:
                    {
                        spUnit pUnit = MemoryManagement::create<Unit>("RECON", getPlayer(unitPlayer), false, this);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 5:
                    {
                        spUnit pUnit = MemoryManagement::create<Unit>("FLARE", getPlayer(unitPlayer), false, this);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 6:
                    {
                        spUnit pUnit = MemoryManagement::create<Unit>("FLAK", getPlayer(unitPlayer), false, this);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 7:
                    {
                        spUnit pUnit = MemoryManagement::create<Unit>("APC", getPlayer(unitPlayer), false, this);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 8:
                    {
                        spUnit pUnit = MemoryManagement::create<Unit>("LIGHT_TANK", getPlayer(unitPlayer), false, this);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 9:
                    {
                        spUnit pUnit = MemoryManagement::create<Unit>("HEAVY_TANK", getPlayer(unitPlayer), false, this);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 10:
                    {
                        spUnit pUnit = MemoryManagement::create<Unit>("MEGATANK", getPlayer(unitPlayer), false, this);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 11:
                    {
                        spUnit pUnit = MemoryManagement::create<Unit>("ARTILLERY", getPlayer(unitPlayer), false, this);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 12:
                    {
                        spUnit pUnit = MemoryManagement::create<Unit>("ROCKETTHROWER", getPlayer(unitPlayer), false, this);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 13:
                    {
                        spUnit pUnit = MemoryManagement::create<Unit>("ANTITANKCANNON", getPlayer(unitPlayer), false, this);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 14:
                    {
                        spUnit pUnit = MemoryManagement::create<Unit>("MISSILE", getPlayer(unitPlayer), false, this);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 15:
                    {
                        spUnit pUnit = MemoryManagement::create<Unit>("FIGHTER", getPlayer(unitPlayer), false, this);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 16:
                    {
                        spUnit pUnit = MemoryManagement::create<Unit>("BOMBER", getPlayer(unitPlayer), false, this);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 17:
                    {
                        spUnit pUnit = MemoryManagement::create<Unit>("DUSTER", getPlayer(unitPlayer), false, this);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 18:
                    {
                        spUnit pUnit = MemoryManagement::create<Unit>("WATERPLANE", getPlayer(unitPlayer), false, this);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 19:
                    {
                        spUnit pUnit = MemoryManagement::create<Unit>("K_HELI", getPlayer(unitPlayer), false, this);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 20:
                    {
                        spUnit pUnit = MemoryManagement::create<Unit>("T_HELI", getPlayer(unitPlayer), false, this);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 21:
                    {
                        spUnit pUnit = MemoryManagement::create<Unit>("BATTLESHIP", getPlayer(unitPlayer), false, this);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 22:
                    {
                        spUnit pUnit = MemoryManagement::create<Unit>("ANTICRAFTCARRIER", getPlayer(unitPlayer), false, this);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 23:
                    {
                        spUnit pUnit = MemoryManagement::create<Unit>("CRUISER", getPlayer(unitPlayer), false, this);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 24:
                    {
                        spUnit pUnit = MemoryManagement::create<Unit>("LANDER", getPlayer(unitPlayer), false, this);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 25:
                    {
                        spUnit pUnit = MemoryManagement::create<Unit>("SUBMARINE", getPlayer(unitPlayer), false, this);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 26:
                    {
                        spUnit pUnit = MemoryManagement::create<Unit>("CANNONBOAT", getPlayer(unitPlayer), false, this);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                }
                if (coUnit > 0)
                {
                    getTerrain(x, y)->getUnit()->setUnitRank(GameEnums::UnitRank_CO0);
                }
            }
        }
        QStringList list = file.split("/");
        m_headerInfo.m_mapName = list[list.size() - 1].remove(list[list.size() - 1].lastIndexOf("."), list[list.size() - 1].size());
        m_headerInfo.m_mapDescription = "";
    }
    pMenu->optimizePlayers();
    m_players[0]->setBaseGameInput(BaseGameInputIF::createAi(this, GameEnums::AiTypes::AiTypes_Human));
    m_players[0]->setControlType(GameEnums::AiTypes::AiTypes_Human);
    // update the whole fucking map
    updateSprites();
}

void GameMap::exportAW4Map(QString)
{
//    If Objektverwalter.Spielfeld.Spieler.Length <= 9 Then
//                Using Writer As New IO.FileStream(File, IO.FileMode.OpenOrCreate, IO.FileAccess.Write, IO.FileShare.Write)
//                    'Terrain-Art
//                    Writer.WriteByte(10)
//                    'Breite
//                    Writer.WriteByte(Objektverwalter.Spielfeld.Breite)
//                    'Hoehe
//                    Writer.WriteByte(Objektverwalter.Spielfeld.Hoehe)
//                    'Kartenstart
//                    Writer.WriteByte(255)
//                    'Karte speichern
//                    For X = 0 To Objektverwalter.Spielfeld.Breite - 1
//   For Y = 0 To Objektverwalter.Spielfeld.Hoehe - 1
//       Dim G As ObjektGebaeude = Objektverwalter.Spielfeld.Landschaften(X, Y, ObjektSpielfeld.Bodenschlacht).Gebaeude
//       'Gebaeude abspeichern
//       If G IsNot Nothing Then
//           Select Case G.GebaeudeID
//               Case "STADT"
//                   Writer.WriteByte(16 + 1)
//                   Writer.WriteByte(0)
//                   Writer.WriteByte(G.Spieler)
//               Case "BASIS"
//                   Writer.WriteByte(16 + 2)
//                   Writer.WriteByte(0)
//                   Writer.WriteByte(G.Spieler)
//               Case "WERFT"
//                   Writer.WriteByte(16 + 3)
//                   Writer.WriteByte(6)
//                   Writer.WriteByte(G.Spieler)
//               Case "FLUGHAFEN"
//                   Writer.WriteByte(16 + 4)
//                   Writer.WriteByte(0)
//                   Writer.WriteByte(G.Spieler)
//               Case "RADAR"
//                   Writer.WriteByte(16 + 5)
//                   Writer.WriteByte(0)
//                   Writer.WriteByte(G.Spieler)
//               Case "TURM"
//                   Writer.WriteByte(16 + 6)
//                   Writer.WriteByte(0)
//                   Writer.WriteByte(G.Spieler)
//               Case "TEMPWERFT"
//                   Writer.WriteByte(16 + 7)
//                   Writer.WriteByte(6)
//                   Writer.WriteByte(G.Spieler)
//               Case "TEMPFLUGHAFEN"
//                   Writer.WriteByte(16 + 8)
//                   Writer.WriteByte(0)
//                   Writer.WriteByte(G.Spieler)
//               Case "HQ"
//                   Writer.WriteByte(16 + 0)
//                   Writer.WriteByte(0)
//                   Writer.WriteByte(G.Spieler)
//               Case "RAKETENSILO", "RAKETENSILO+RAKETE"
//                   Writer.WriteByte(16 + 0)
//                   Writer.WriteByte(0)
//                   Writer.WriteByte(G.Spieler)
//               Case Else
//                   Writer.WriteByte(0)
//                   Writer.WriteByte(0)
//                   Writer.WriteByte(0)
//           End Select
//       Else
//           'Landschaft abspeichern.
//           Select Case Objektverwalter.Spielfeld.Landschaften(X, Y, ObjektSpielfeld.Bodenschlacht).getID
//               Case "EBENE"
//                   Writer.WriteByte(0)
//                   Writer.WriteByte(0)
//                   Writer.WriteByte(0)
//               Case "WALD"
//                   Writer.WriteByte(1)
//                   Writer.WriteByte(0)
//                   Writer.WriteByte(0)
//               Case "BERG"
//                   Writer.WriteByte(2)
//                   Writer.WriteByte(0)
//                   Writer.WriteByte(0)
//               Case "OEDLAND"
//                   Writer.WriteByte(3)
//                   Writer.WriteByte(0)
//                   Writer.WriteByte(0)
//               Case "RUINE"
//                   Writer.WriteByte(4)
//                   Writer.WriteByte(0)
//                   Writer.WriteByte(0)
//               Case "FEUER"
//                   Writer.WriteByte(5)
//                   Writer.WriteByte(0)
//                   Writer.WriteByte(0)
//               Case "SEE"
//                   Writer.WriteByte(6)
//                   Writer.WriteByte(6) 'See Terrain
//                   Writer.WriteByte(0)
//               Case "RIFF"
//                   Writer.WriteByte(7)
//                   Writer.WriteByte(6) 'See Terrain
//                   Writer.WriteByte(0)
//               Case "RAUESEE"
//                   Writer.WriteByte(8)
//                   Writer.WriteByte(6) 'See Terrain
//                   Writer.WriteByte(0)
//               Case "NEBEL"
//                   Writer.WriteByte(9)
//                   Writer.WriteByte(6) 'See Terrain
//                   Writer.WriteByte(0)
//               Case "STRAND"
//                   Writer.WriteByte(15)
//                   Writer.WriteByte(6) 'See Terrain
//                   Writer.WriteByte(0)
//               Case "STRASSE"
//                   Writer.WriteByte(10)
//                   Writer.WriteByte(0)
//                   Writer.WriteByte(0)
//               Case "FLUSS"
//                   Writer.WriteByte(12)
//                   Writer.WriteByte(12) 'FLUSS Terrain
//                   Writer.WriteByte(0)
//               Case "PLASMA"
//                   Writer.WriteByte(13)
//                   Writer.WriteByte(0)
//                   Writer.WriteByte(0)
//               Case "METEOR"
//                   Writer.WriteByte(14)
//                   Writer.WriteByte(0)
//                   Writer.WriteByte(0)
//               Case "BRUECKE"
//                   Writer.WriteByte(11)
//                   Writer.WriteByte(6) 'See Terrain
//                   Writer.WriteByte(0)
//               Case Else
//                   Writer.WriteByte(0)
//                   Writer.WriteByte(0)
//                   Writer.WriteByte(0)
//           End Select
//       End If
//       'Einheitspeichern
//       Dim E As ObjektEinheit = Objektverwalter.Spielfeld.Einheiten(X, Y, ObjektSpielfeld.Bodenschlacht)
//       If E IsNot Nothing Then
//           Select Case E.EinheitenID
//               Case "INF"
//                   Writer.WriteByte(1)
//               Case "MECH"
//                   Writer.WriteByte(2)
//               Case "MOTORRAD"
//                   Writer.WriteByte(3)
//               Case "SPAEHER"
//                   Writer.WriteByte(4)
//               Case "FLARE"
//                   Writer.WriteByte(5)
//               Case "FLAK"
//                   Writer.WriteByte(6)
//               Case "TTP"
//                   Writer.WriteByte(7)
//               Case "JGPZ"
//                   Writer.WriteByte(8)
//               Case "KPZ"
//                   Writer.WriteByte(9)
//               Case "MEGAPZ"
//                   Writer.WriteByte(10)
//               Case "ART"
//                   Writer.WriteByte(11)
//               Case "RKW"
//                   Writer.WriteByte(12)
//               Case "PANZERABWEHRKANONE"
//                   Writer.WriteByte(13)
//               Case "MISSILE"
//                   Writer.WriteByte(14)
//               Case "JAEGER"
//                   Writer.WriteByte(15)
//               Case "BOMBER"
//                   Writer.WriteByte(16)
//               Case "AUFKLAERER"
//                   Writer.WriteByte(17)
//               Case "WASSERFLUGZEUG"
//                   Writer.WriteByte(18)
//               Case "K-HELI"
//                   Writer.WriteByte(19)
//               Case "T-HELI"
//                   Writer.WriteByte(20)
//               Case "SCHLFF"
//                   Writer.WriteByte(21)
//               Case "FLTR"
//                   Writer.WriteByte(22)
//               Case "KREUZER"
//                   Writer.WriteByte(23)
//               Case "L-BOOT"
//                   Writer.WriteByte(24)
//               Case "U-BOOT"
//                   Writer.WriteByte(25)
//               Case "KANONENBOOT"
//                   Writer.WriteByte(26)
//               Case Else
//                   Writer.WriteByte(0)
//                   Writer.WriteByte(0)
//                   Writer.WriteByte(0)
//                   Writer.WriteByte(0)
//                   Continue For
//           End Select

//           'Eigentlich KI Stufe
//           If E.AIUse <= ObjektEinheit.AISetting.Halten Then
//               Writer.WriteByte(E.AIUse)
//           Else
//               Writer.WriteByte(ObjektEinheit.AISetting.Standard)
//           End If

//           'Spieler
//           Writer.WriteByte(E.Spieler)
//           If E.EinheitenStufe = ObjektEinheit.Stufe.KO Then
//               Writer.WriteByte(1)
//           Else
//               Writer.WriteByte(0)
//           End If
//       Else
//           Writer.WriteByte(0)
//           Writer.WriteByte(0)
//           Writer.WriteByte(0)
//           Writer.WriteByte(0)
//       End If
//   Next
//                    Next
//                End Using
//            End If
}
