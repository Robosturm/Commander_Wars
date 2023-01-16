var Constructor = function()
{
    this.getCOStyles = function()
    {
        // string array containing the endings of the alternate co style
        
        return ["+alt", "+alt2"];
    };

    this.init = function(co, map)
    {
        co.setPowerStars(2);
        co.setSuperpowerStars(4);
    };

    this.loadCOMusic = function(co, map)
    {
        // put the co music in here.
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/power.mp3", 992, 45321);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/superpower.mp3", 1505, 49515);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/tagpower.mp3", 14611, 65538);
                break;
            default:
                audio.addMusic("resources/music/cos/sensei.mp3", 304, 63603);
                break;
        }
    };

    this.activatePower = function(co, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        dialogAnimation.queueAnimation(powerNameAnimation);

        CO_SENSEI.spawnUnits(co, "INFANTRY", 9, powerNameAnimation, map);
    };

    this.activateSuperpower = function(co, powerMode, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        powerNameAnimation.queueAnimationBefore(dialogAnimation);

        CO_SENSEI.spawnUnits(co, "MECH", 9, powerNameAnimation, map);
    };

    this.spawnUnits = function(co, unitID, hp, powerNameAnimation, map)
    {
        var buildings = co.getOwner().getBuildings();
        var animations = [];
        var counter = 0;
        buildings.randomize();
        var size = buildings.size();
        for (var i = 0; i < size; i++)
        {
            var building = buildings.at(i);
            if (building.getBuildingID() === "TOWN")
            {
                if (map.getTerrain(building.getX(), building.getY()).getUnit() === null)
                {
                    var animation = GameAnimationFactory.createAnimation(map, building.getX(), building.getY());
                    animation.writeDataInt32(building.getX());
                    animation.writeDataInt32(building.getY());
                    animation.writeDataString(unitID);
                    animation.writeDataInt32(co.getOwner().getPlayerID());
                    animation.writeDataInt32(hp);
                    animation.setStartOfAnimationCall("ANIMATION", "postAnimationSpawnUnit");
                    var delay = globals.randInt(135, 265);
                    if (animations.length < 5)
                    {
                        delay *= i;
                    }
                    if (i % 2 === 0)
                    {
                        animation.setSound("power8_1.wav", 1, delay);
                    }
                    else
                    {
                        animation.setSound("power8_2.wav", 1, delay);
                    }
                    if (animations.length < 5)
                    {
                        animation.addSprite("power8", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                        powerNameAnimation.queueAnimation(animation);
                        animations.push(animation);
                    }
                    else
                    {
                        animation.addSprite("power8", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                        animations[counter].queueAnimation(animation);
                        animations[counter] = animation;
                        counter++;
                        if (counter >= animations.length)
                        {
                            counter = 0;
                        }
                    }
                }
            }
        }
    };

    this.getCOUnitRange = function(co, map)
    {
        return 1;
    };
    this.getCOArmy = function()
    {
        return "YC";
    };

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (attacker.getUnitType() === GameEnums.UnitType_Infantry)
                {
                    return 30;
                }
                else if (attacker.getUnitID() === "K_HELI")
                {
                    return 80;
                }
                else if (attacker.getUnitType() === GameEnums.UnitType_Naval)
                {
                    return 0;
                }
                return 10
            case GameEnums.PowerMode_Power:
                if (attacker.getUnitType() === GameEnums.UnitType_Infantry)
                {
                    return 30;
                }
                else if (attacker.getUnitID() === "K_HELI")
                {
                    return 80;
                }
                else if (attacker.getUnitType() === GameEnums.UnitType_Naval)
                {
                    return 0;
                }
                return 10
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    if (attacker.getUnitType() === GameEnums.UnitType_Infantry)
                    {
                        return 40;
                    }
                    else if (attacker.getUnitID() === "K_HELI")
                    {
                        return 50;
                    }
                    else if (attacker.getUnitType() === GameEnums.UnitType_Naval)
                    {
                        return 0;
                    }
                    return 10;
                }
                else if (attacker.getUnitID() === "K_HELI")
                {
                    return 30;
                }
                break;
        }
        if (attacker.getUnitType() === GameEnums.UnitType_Naval)
        {
            return -10;
        }
        return 0;
    };
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        if (co.inCORange(Qt.point(defPosX, defPosY), defender) ||
                co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 10;
        }
        return 0;
    };

    this.getMovementpointModifier = function(co, unit, posX, posY, map)
    {
        if (unit.isTransporter())
        {
            return 1;
        }
        return 0;
    };
    this.getAiCoUnitBonus = function(co, unit, map)
    {
        if (unit.getUnitID() === "K_HELI")
        {
            return 8;
        }
        else if (unit.getUnitType() === GameEnums.UnitType_Infantry)
        {
            return 5;
        }
        else if (unit.getUnitType() === GameEnums.UnitType_Air)
        {
            return 3;
        }
        return 0;
    };
    this.getCOUnits = function(co, building, map)
    {
        var buildingId = building.getBuildingID();
        if (buildingId === "FACTORY" ||
            buildingId === "TOWN" ||
            buildingId === "HQ")
        {
            return ["ZCOUNIT_COMMANDO"];
        }
        return [];
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A former paratrooper rumored to have been quite the commander in his day. Cranky Paratrooper");
    };
    this.getHits = function(co)
    {
        return qsTr("Lazy, rainy days");
    };
    this.getMiss = function(co)
    {
        return qsTr("Busy malls");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Copters have incredibly high firepower, but naval units are weaker. Foot soldiers have increased firepower.");
    };
    this.getLongCODescription = function()
    {
        return qsTr("\nSpecial Unit:\nCommando\n") +
               qsTr("\nGlobal Effect: \nCopters have increased firepower and Naval Units are weaker.") +
               qsTr("\n\nCO Zone Effect: \nCopters have way more firepower and infantries higher firepower as well.");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("Copter firepower increases. Infantry units with 9 HP appear in all his cities, ready to be moved.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Copter Command");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("Copter firepower increases. Mech units with 9 HP appear in all his cities, ready to be moved.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Airborne Assault");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Nothing good ever comes from war... when this is done, let's go home."),
                qsTr("I've still got what it takes to defeat you youngsters!"),
                qsTr("Paratroopers! Prepare to jump!"),
                qsTr("Ha ha hah!  This will be a crushing victory!"),
                qsTr("See what experience can do for you!?"),
                qsTr("Ha ha! I didn't get old for nothing!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Hm hm hmmm... I've still got what it takes!"),
                qsTr("Ah, time for a nap."),
                qsTr("Now we can take things nice and slow again...")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("Hah ha! We were defeated fair and square. Score one for the young 'uns."),
                qsTr("Eh? The battle's over? When did that happen?")];
    };
    this.getName = function()
    {
        return qsTr("Sensei");
    };
}

Constructor.prototype = CO;
var CO_SENSEI = new Constructor();
