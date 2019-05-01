var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(2);
        co.setSuperpowerStars(4);
    };

    this.loadCOMusic = function(co)
    {
        // put the co music in here.
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/power.mp3");
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/superpower.mp3");
                break;
            default:
                audio.addMusic("resources/music/cos/sensei.mp3")
                break;
        }
    };

    this.activatePower = function(co)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(false);
        dialogAnimation.queueAnimation(powerNameAnimation);

        CO_SENSEI.spawnUnits(co, "INFANTRY", 9, powerNameAnimation);
        audio.clearPlayList();
        CO_SENSEI.loadCOMusic(co);
        audio.playRandom();
    };

    this.activateSuperpower = function(co)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(true);
        dialogAnimation.queueAnimation(powerNameAnimation);

        CO_SENSEI.spawnUnits(co, "MECH", 9, powerNameAnimation);
        audio.clearPlayList();
        CO_SENSEI.loadCOMusic(co);
        audio.playRandom();
    };

    this.spawnUnits = function(co, unitID, hp, powerNameAnimation)
    {
        var buildings = co.getPlayer().getBuildings();
        var animations = [];
        var counter = 0;
        buildings.randomize();
        for (var i = 0; i < buildings.size(); i++)
        {
            var building = buildings.at(i);
            if (building.getBuildingID() === "TOWN")
            {
                if (map.getTerrain(building.getX(), building.getY()).getUnit() === null)
                {
                    map.spawnUnit(building.getX(), building.getY(), unitID, co.getPlayer());
                    map.getTerrain(building.getX(), building.getY()).getUnit().setHp(hp);
                    var animation = GameAnimationFactory.createAnimation(building.getX(), building.getY());
                    if (animations.length < 5)
                    {
                        animation.addSprite("power8", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                        powerNameAnimation.queueAnimation(animation);
                        animations.push(animation);
                    }
                    else
                    {
                        animation.addSprite("power8", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
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
        buildings.remove();
    };

    this.getCOUnitRange = function(co)
    {
        return 2;
    };
    this.getCOArmy = function()
    {
        return "YC";
    };

    this.getInfantryIDS = function()
    {
        return  ["INFANTRY", "MECH", "SNIPER", "MOTORBIKE"];
    }
    this.getSeaUnitIDS = function()
    {
        return ["AIRCRAFTCARRIER", "CRUISER", "BATTLESHIP", "CANNONBOAT", "DESTROYER", "SUBMARINE"];
    };

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        var seaUnitIDs = CO_SENSEI.getSeaUnitIDS();
        var unitInfantryIDs = CO_SENSEI.getInfantryIDS();
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                if (unitInfantryIDs.indexOf(attacker.getUnitID()) >= 0)
                {
                    return 30;
                }
                else if (attacker.getUnitID() === "K_HELI")
                {
                    return 80;
                }
                break;
            case GameEnums.PowerMode_Power:
                if (unitInfantryIDs.indexOf(attacker.getUnitID()) >= 0)
                {
                    return 30;
                }
                else if (attacker.getUnitID() === "K_HELI")
                {
                    return 80;
                }
                break;
            default:
                if (unitInfantryIDs.indexOf(attacker.getUnitID()) >= 0)
                {
                    if (co.inCORange(Qt.point(atkPosX, atkPosY)))
                    {
                        return 30;
                    }
                    return 20;
                }
                else if (attacker.getUnitID() === "K_HELI")
                {
                    if (co.inCORange(Qt.point(atkPosX, atkPosY)))
                    {
                        return 50;
                    }
                    return 30;
                }
                break;
        }
        if (seaUnitIDs.indexOf(attacker.getUnitID()) >= 0)
        {
            return -10;
        }
        return 0;
    };

    this.getMovementpointModifier = function(co, unit, posX, posY)
    {
        var unitTransportIDs = ["APC", "LANDER", "T_HELI", "TRANSPORTPLANE", "BLACK_BOOT"];
        if (unitTransportIDs.indexOf(unit.getUnitID()) >= 0)
        {
            return 1;
        }
        return 0;
    };

    // CO - Intel
    this.getBio = function()
    {
        return qsTr("A former paratrooper rumored to have been quite the commander in his day. Cranky Paratrooper");
    };
    this.getHits = function()
    {
        return qsTr("Lazy, rainy days");
    };
    this.getMiss = function()
    {
        return qsTr("Busy malls");
    };
    this.getCODescription = function()
    {
        return qsTr("Copters have incredibly high firepower, but naval units have weak attacks. Foot soldiers have increased firepower.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("Copter firepower increases and copter movement increases. Infantry firepower rises greatly.");
    };
    this.getPowerName = function()
    {
        return qsTr("Copter Command");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("Copter firepower increases. Infantry units with 9 HP appear in all his cities, ready to be moved.");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("Airborne Assault");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("Nothing good ever comes from war... when this is done, let's go home."),
                qsTr("I've still got what it takes to defeat you youngsters!"),
                qsTr("Paratroopers! Prepare to jump!"),
                qsTr("Ha ha hah!  This will be a crushing victory!"),
                qsTr("See what experience can do for you!?"),
                qsTr("Ha ha! I didn't get old for nothing!")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("Hm hm hmmm... I've still got what it takes!"),
                qsTr("Ah, time for a nap."),
                qsTr("Now we can take things nice and slow again...")];
    };
    this.getDefeatSentences = function()
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
