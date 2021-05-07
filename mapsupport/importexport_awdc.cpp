#include <QDataStream>
#include <QFile>

#include "coreengine/console.h"

#include "game/gamemap.h"
#include "game/player.h"
#include "game/co.h"
#include "game/building.h"
#include "game/unit.h"

#include "menue/editormenue.h"

void GameMap::importAWDCMap(QString file)
{
    quint8 sign = 0;
    if (QFile::exists(file))
    {
        clearMap();

        QFile data(file);
        data.open(QFile::ReadOnly);
        QDataStream stream(&data);
        stream >> sign;
        stream >> sign;
        qint32 width = sign;
        stream >> sign;
        qint32 heigth = sign;
        // load 5 players :)
        for (qint32 i = 0; i < 9; i++)
        {
            m_players.append(spPlayer::create());
            m_players[i]->init();
        }

        for (qint32 y = 0; y < heigth; y++)
        {
            m_fields.append(QVector<spTerrain>());
            for (qint32 x = 0; x < width; x++)
            {
                spTerrain pTerrain = Terrain::createTerrain("PLAINS", x, y, "");
                this->addChild(pTerrain);
                m_fields[y].append(pTerrain);
                pTerrain->setPosition(x * m_imagesize, y * m_imagesize);
                pTerrain->setPriority(static_cast<qint32>(Mainapp::ZOrder::Terrain) + y);
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
                        spBuilding pBuilding = spBuilding::create("TOWN");
                        if (buildingPlayer == 0)
                        {
                            pBuilding->setOwner(nullptr);
                        }
                        else
                        {
                            pBuilding->setOwner(getPlayer(buildingPlayer));
                        }
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 18:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY");
                        if (buildingPlayer == 0)
                        {
                            pBuilding->setOwner(nullptr);
                        }
                        else
                        {
                            pBuilding->setOwner(getPlayer(buildingPlayer));
                        }
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 19:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR");
                        if (buildingPlayer == 0)
                        {
                            pBuilding->setOwner(nullptr);
                        }
                        else
                        {
                            pBuilding->setOwner(getPlayer(buildingPlayer));
                        }
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 20:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT");
                        if (buildingPlayer == 0)
                        {
                            pBuilding->setOwner(nullptr);
                        }
                        else
                        {
                            pBuilding->setOwner(getPlayer(buildingPlayer));
                        }
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 21:
                    {
                        spBuilding pBuilding = spBuilding::create("RADAR");
                        if (buildingPlayer == 0)
                        {
                            pBuilding->setOwner(nullptr);
                        }
                        else
                        {
                            pBuilding->setOwner(getPlayer(buildingPlayer));
                        }
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 22:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER");
                        if (buildingPlayer == 0)
                        {
                            pBuilding->setOwner(nullptr);
                        }
                        else
                        {
                            pBuilding->setOwner(getPlayer(buildingPlayer));
                        }
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 23:
                    {
                        spBuilding pBuilding = spBuilding::create("TEMPORARY_HARBOUR");
                        if (buildingPlayer == 0)
                        {
                            pBuilding->setOwner(nullptr);
                        }
                        else
                        {
                            pBuilding->setOwner(getPlayer(buildingPlayer));
                        }
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 24:
                    {
                        spBuilding pBuilding = spBuilding::create("TEMPORARY_AIRPORT");
                        if (buildingPlayer == 0)
                        {
                            pBuilding->setOwner(nullptr);
                        }
                        else
                        {
                            pBuilding->setOwner(getPlayer(buildingPlayer));
                        }
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 25:
                    {
                        spBuilding pBuilding;
                        if (buildingPlayer == 0)
                        {
                            pBuilding = spBuilding::create("SILO_ROCKET");
                            pBuilding->setOwner(nullptr);
                        }
                        else
                        {
                            pBuilding = spBuilding::create("HQ");
                            pBuilding->setOwner(getPlayer(buildingPlayer));
                        }
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    // terrain
                    case 0:
                    {
                        replaceTerrain("PLAINS", x, y);
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
                        replaceTerrain("REAF", x, y);
                        break;
                    }
                    case 8:
                    {
                        replaceTerrain("ROUGH_SEA", x, y);
                        break;
                    }
                    case 9:
                    {
                        replaceTerrain("FOG", x, y);
                        break;
                    }
                    case 10:
                    {
                        replaceTerrain("STREET", x, y);
                        break;
                    }
                    case 11:
                    {
                        replaceTerrain("BRIDGE", x, y);
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
                        replaceTerrain("PLAINS", x, y);
                        break;
                    }
                }
                switch (unitId)
                {
                    case 1:
                    {
                        spUnit pUnit = spUnit::create("INFANTRY", getPlayer(unitPlayer), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 2:
                    {
                        spUnit pUnit = spUnit::create("MECH", getPlayer(unitPlayer), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 3:
                    {
                        spUnit pUnit = spUnit::create("MOTORBIKE", getPlayer(unitPlayer), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 4:
                    {
                        spUnit pUnit = spUnit::create("RECON", getPlayer(unitPlayer), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 5:
                    {
                        spUnit pUnit = spUnit::create("FLARE", getPlayer(unitPlayer), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 6:
                    {
                        spUnit pUnit = spUnit::create("FLAK", getPlayer(unitPlayer), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 7:
                    {
                        spUnit pUnit = spUnit::create("APC", getPlayer(unitPlayer), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 8:
                    {
                        spUnit pUnit = spUnit::create("LIGHT_TANK", getPlayer(unitPlayer), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 9:
                    {
                        spUnit pUnit = spUnit::create("HEAVY_TANK", getPlayer(unitPlayer), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 10:
                    {
                        spUnit pUnit = spUnit::create("MEGATANK", getPlayer(unitPlayer), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 11:
                    {
                        spUnit pUnit = spUnit::create("ARTILLERY", getPlayer(unitPlayer), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 12:
                    {
                        spUnit pUnit = spUnit::create("ROCKETTHROWER", getPlayer(unitPlayer), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 13:
                    {
                        spUnit pUnit = spUnit::create("ANTITANKCANNON", getPlayer(unitPlayer), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 14:
                    {
                        spUnit pUnit = spUnit::create("MISSILE", getPlayer(unitPlayer), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 15:
                    {
                        spUnit pUnit = spUnit::create("FIGHTER", getPlayer(unitPlayer), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 16:
                    {
                        spUnit pUnit = spUnit::create("BOMBER", getPlayer(unitPlayer), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 17:
                    {
                        spUnit pUnit = spUnit::create("DUSTER", getPlayer(unitPlayer), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 18:
                    {
                        spUnit pUnit = spUnit::create("WATERPLANE", getPlayer(unitPlayer), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 19:
                    {
                        spUnit pUnit = spUnit::create("K_HELI", getPlayer(unitPlayer), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 20:
                    {
                        spUnit pUnit = spUnit::create("T_HELI", getPlayer(unitPlayer), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 21:
                    {
                        spUnit pUnit = spUnit::create("BATTLESHIP", getPlayer(unitPlayer), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 22:
                    {
                        spUnit pUnit = spUnit::create("ANTICRAFTCARRIER", getPlayer(unitPlayer), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 23:
                    {
                        spUnit pUnit = spUnit::create("CRUISER", getPlayer(unitPlayer), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 24:
                    {
                        spUnit pUnit = spUnit::create("LANDER", getPlayer(unitPlayer), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 25:
                    {
                        spUnit pUnit = spUnit::create("SUBMARINE", getPlayer(unitPlayer), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 26:
                    {
                        spUnit pUnit = spUnit::create("CANNONBOAT", getPlayer(unitPlayer), false);
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
        m_mapName = list[list.size() - 1].remove(list[list.size() - 1].lastIndexOf("."), list[list.size() - 1].size());
        m_mapDescription = "";
    }
    EditorMenue::getInstance()->optimizePlayers();
    // update the whole fucking map
    updateSprites();
}

void GameMap::exportAWDCMap(QString)
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
