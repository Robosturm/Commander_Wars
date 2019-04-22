#include "gamerecorder.h"

#include "game/gamemap.h"

#include "coreengine/mainapp.h"

GameRecorder::GameRecorder()
    : QObject()
{
    moveToThread(Mainapp::getInstance()->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

void GameRecorder::serializeObject(QDataStream& pStream)
{
    pStream << getVersion();
    pStream << static_cast<qint32>(m_Record.size());
    for (qint32 i = 0; i < m_Record.size(); i++)
    {
        m_Record[i]->serializeObject(pStream);
    }
    pStream << static_cast<qint32>(destroyedUnits.size());
    for (qint32 i = 0; i < destroyedUnits.size(); i++)
    {
        pStream << destroyedUnits[i];
        pStream << lostUnits[i];
        pStream << damageDealt[i];
        pStream << attackNumbers[i];
        pStream << deployedUnits[i];
    }
}

void GameRecorder::deserializeObject(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    qint32 size = 0;
    pStream >> size;
    for (qint32 i = 0; i < size; i++)
    {
        m_Record.append(new DayToDayRecord());
        m_Record[i]->deserializeObject(pStream);
    }
    pStream >> size;
    for (qint32 i = 0; i < size; i++)
    {
        quint64 value = 0;
        pStream >> value;
        destroyedUnits.append(value);
        pStream >> value;
        lostUnits.append(value);
        pStream >> value;
        damageDealt.append(value);
        pStream >> value;
        attackNumbers.append(value);
        pStream >> value;
        deployedUnits.append(value);
    }
}

void GameRecorder::newDay()
{
    qint32 playerCount = GameMap::getInstance()->getPlayerCount();
    if (destroyedUnits.size() == 0)
    {
        for (qint32 i = 0; i < playerCount; i++)
        {
            destroyedUnits.append(0);
            lostUnits.append(0);
            damageDealt.append(0);
            attackNumbers.append(0);
            deployedUnits.append(0);
        }
    }
    m_Record.append(new DayToDayRecord(playerCount));
}

void GameRecorder::lostUnit(qint32 player)
{
    if (player >= 0 && player < lostUnits.size())
    {
        lostUnits[player]++;
    }
}

void GameRecorder::destroyedUnit(qint32 player)
{
    if (player >= 0 && player < destroyedUnits.size())
    {
        destroyedUnits[player]++;
    }
}

void GameRecorder::buildUnit(qint32 player)
{
    if (player >= 0 && player < deployedUnits.size())
    {
        deployedUnits[player]++;
    }
}

void GameRecorder::attacked(qint32 player, float damage)
{
    if (player >= 0 && player < attackNumbers.size())
    {
        attackNumbers[player]++;
        damageDealt[player] += damage;
    }
}

void GameRecorder::updatePlayerData(qint32 player)
{
    m_Record[m_Record.size() - 1]->addPlayerRecord(player, GameMap::getInstance()->getCurrentDay());
}

void GameRecorder::addSpecialEvent(qint32 player, GameEnums::GameRecord_SpecialEvents event)
{
    m_Record[m_Record.size() - 1]->addSpecialEvent(player, GameMap::getInstance()->getCurrentDay(), event);
}

GameRecorder::Rang GameRecorder::calculateRang(qint32 player, QVector3D& scorePoints)
{

//        Dim SiegerTeam As Char = "q"
//        If Objektverwalter.Spielfeld.Regeln.Spielende = True Then

//            For I = 1 To Objektverwalter.Spielfeld.Regeln.Besiegt.Length - 1
//                If Objektverwalter.Spielfeld.Regeln.Besiegt(I) = False Then
//                    SiegerTeam = Objektverwalter.Spielfeld.Spieler(I).Team
//                End If
//            Next
//        End If

    qint32 score = 0;
//        Dim MapSize As Integer = Spielfeld.GetSpielgroesse
//        'Zeit
//        Dim MTime As Integer = 0
//        If Spielfeld.Regeln.MaxTime <= 0 Then
//            MTime = (Spielfeld.Hoehe + Spielfeld.Breite) / 5 * 2
//        Else
//            MTime = Spielfeld.Regeln.MaxTime
//        End If
//        If Spielfeld.Runde < MTime Then
//            PunkteVerteilung.X = 200 - (Spielfeld.Runde * (100 / MTime))
//        Else
//            PunkteVerteilung.X = 100 - ((Spielfeld.Runde - MTime) * (100 / (3 * MTime)))
//        End If
//        If PunkteVerteilung.X < 0 Then PunkteVerteilung.X = 0
//        If Spielfeld.Spieler(Spieler).Team <> SiegerTeam And Not SiegerTeam = "q" Then
//            Dim LostDay As Integer = 0
//            For I = 0 To Aufzeichnungen.Count - 1
//                If Aufzeichnungen(I).SpielerAufzeichnungen(Spieler - 1, 0) Is Nothing Then
//                    LostDay = I
//                    Exit For
//                Else
//                    If Aufzeichnungen(I).SpielerAufzeichnungen(Spieler - 1, 0).Basen < 0 Then
//                        If I - 1 >= 0 Then
//                            If Aufzeichnungen(I - 1).SpielerAufzeichnungen(Spieler - 1, 0).Basen > 0 Then
//                                LostDay = I
//                                Exit For
//                            End If
//                        Else
//                            LostDay = 0
//                            Exit For
//                        End If
//                    ElseIf I = Aufzeichnungen.Count - 1 Then
//                        LostDay = Spielfeld.Runde
//                    End If
//                End If
//            Next
//            PunkteVerteilung.X *= (0.8) * LostDay / Spielfeld.Runde
//        End If
//        // Force
//        Dim P1 As Integer = (Damage(Spieler - 1) + (DestroyedUnits(Spieler - 1) * 140))
//        If AttackNumbers(Spieler - 1) > 0 Then
//            P1 /= AttackNumbers(Spieler - 1)
//        Else
//            P1 = 0
//        End If
//        If P1 >= 100 Then
//            PunkteVerteilung.Y += P1
//        ElseIf P1 > 50 Then
//            PunkteVerteilung.Y += (2 * P1) - 100
//        Else
//            PunkteVerteilung.Y += P1
//        End If
//        If PunkteVerteilung.Y < 0 Then PunkteVerteilung.Y = 0
//        // technique
//        Dim Truppenlimit As Integer = 0
//        If Spielfeld.Regeln.MaxDeployedUnits <= 0 Then
//            Truppenlimit = Math.DivRem(MapSize, 9, 0)
//        Else
//            Truppenlimit = Spielfeld.Regeln.MaxDeployedUnits
//        End If
//        'New Techscore calculation
//        Dim TechScore1 As Single = 0
//        Dim TechScore2 As Single = 0
//        Dim TechScore3 As Single = 0
//        If LostUnits(Spieler - 1) > 0 Then 'First the more Units you destroy while loosing less Units
//            TechScore1 = (DestroyedUnits(Spieler - 1) / LostUnits(Spieler - 1)) * 1 / 2
//        Else
//            TechScore1 = 1.3
//        End If
//        If TechScore1 > 1.3 Then TechScore1 = 1.3 'Just to take it lower.
//        Dim Deployed As Integer = DeployedUnits(Spieler - 1) 'You can loose 10% of your Army and still get a good Technique Score
//        If Aufzeichnungen(0).SpielerAufzeichnungen(Spieler - 1, 0).Einheiten > 0 Then
//            Deployed += Aufzeichnungen(0).SpielerAufzeichnungen(Spieler - 1, 0).Einheiten
//        End If
//        If Deployed > 0 Then
//            TechScore2 = (1 - (LostUnits(Spieler - 1) / Deployed)) * 1.1
//        Else
//            TechScore2 = 1.3
//        End If
//        If TechScore2 > 1.3 Then TechScore2 = 1.3
//        'You get less points if you produce to much Units
//        If DeployedUnits(Spieler - 1) > 0 Then
//            TechScore3 = Truppenlimit / DeployedUnits(Spieler - 1)
//        Else
//            TechScore3 = 2
//        End If
//        If TechScore3 > 1.3 Then TechScore3 = 1.3
//        PunkteVerteilung.Z = TechScore1 * TechScore2 * TechScore3 * 100
//        If PunkteVerteilung.Z < 0 Then PunkteVerteilung.Z = 0
//        Punkte = PunkteVerteilung.X + PunkteVerteilung.Y + PunkteVerteilung.Z
//        'Rang auswaehlen
        return getRank(score);
}


GameRecorder::Rang GameRecorder::getRank(qint32 score)
{
    if (score >= 330)
    {
        return GameRecorder::Rang::S;
    }
    else if (score >= 280)
    {
        return GameRecorder::Rang::A;
    }
    else if (score >= 250)
    {
        return GameRecorder::Rang::B;
    }
    else if (score >= 200)
    {
        return GameRecorder::Rang::C;
    }
    else
    {
        return GameRecorder::Rang::D;
    }
}
