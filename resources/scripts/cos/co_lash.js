var Constructor = function()
{
    this.getCOStyles = function()
    {
        return ["+alt", "+alt2"];
    };

    this.init = function(co, map)
    {
        co.setPowerStars(4);
        co.setSuperpowerStars(3);
    };

    this.loadCOMusic = function(co, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/bh_power.mp3", 1091 , 49930);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/bh_superpower.mp3", 3161 , 37731);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/bh_tagpower.mp3", 779 , 51141);
                break;
            default:
                audio.addMusic("resources/music/cos/lash.mp3", 8885, 58311);
                break;
            }
        }
    };

    this.activatePower = function(co, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var units = co.getOwner().getUnits();
        var animations = [];
        var counter = 0;
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            var animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
            var delay = globals.randInt(135, 265);
            if (animations.length < 5)
            {
                delay *= i;
            }
            animation.setSound("power11.wav", 1, delay);
            if (animations.length < 5)
            {
                animation.addSprite("power11", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power11", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                animations[counter].queueAnimation(animation);
                animations[counter] = animation;
                counter++;
                if (counter >= animations.length)
                {
                    counter = 0;
                }
            }
        }
    };

    this.activateSuperpower = function(co, powerMode, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        powerNameAnimation.queueAnimationBefore(dialogAnimation);

        var units = co.getOwner().getUnits();
        var animations = [];
        var counter = 0;
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            var animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
            var delay = globals.randInt(135, 265);
            if (animations.length < 7)
            {
                delay *= i;
            }
            if (i % 2 === 0)
            {
                animation.setSound("power12_1.wav", 1, delay);
            }
            else
            {
                animation.setSound("power12_2.wav", 1, delay);
            }
            if (animations.length < 7)
            {
                animation.addSprite("power12", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 2, delay);
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power12", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 2, delay);
                animations[counter].queueAnimation(animation);
                animations[counter] = animation;
                counter++;
                if (counter >= animations.length)
                {
                    counter = 0;
                }
            }
        }
    };

    this.getCOUnitRange = function(co, map)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "BH";
    };

    this.superPowerTerrainDefenseModifier = 1;
    this.powerOffBonus = 10;
    this.powerDefBonus = 10;

    this.d2dTerrainBonus = 10;
    this.d2dCoZoneTerrainBonus = 10;
    this.d2dCoZoneOffBonus = 10;
    this.d2dCoZoneDefBonus = 10;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            if (map !== null)
            {
                if (map.onMap(atkPosX, atkPosY))
                {
                    var terrainDefense = map.getTerrain(atkPosX, atkPosY).getDefense(attacker);
                    switch (co.getPowerMode())
                    {
                    case GameEnums.PowerMode_Tagpower:
                    case GameEnums.PowerMode_Superpower:
                    case GameEnums.PowerMode_Power:
                        if (attacker.useTerrainDefense())
                        {
                            return terrainDefense * CO_LASH.d2dTerrainBonus + CO_LASH.powerOffBonus;
                        }
                        return CO_LASH.powerOffBonus;
                    default:
                        if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                        {
                            return terrainDefense * CO_LASH.d2dCoZoneTerrainBonus + CO_LASH.d2dCoZoneOffBonus;
                        }
                        return terrainDefense * CO_LASH.d2dTerrainBonus;
                    }
                }
            }
            else if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                return CO_LASH.d2dCoZoneOffBonus;
            }
        }
        return 0;
    };
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            if (co.getPowerMode() > GameEnums.PowerMode_Off)
            {
                return CO_LASH.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_LASH.d2dCoZoneDefBonus;
            }
        }
        return 0;
    };
    this.getTerrainDefenseModifier = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return map.getTerrain(posX, posY).getBaseDefense() * CO_LASH.superPowerTerrainDefenseModifier;
            case GameEnums.PowerMode_Power:
                return 0;
            default:
                return 0;
            }
        }
        return 0;
    };
    this.getMovementcostModifier = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            if (unit.getOwner() === co.getOwner())
            {
                switch (co.getPowerMode())
                {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                case GameEnums.PowerMode_Power:
                    return -999;
                default:
                }
            }
        }
        return 0;
    };

    this.getCOUnits = function(co, building, map)
    {
        if (CO.isActive(co))
        {
            var buildingId = building.getBuildingID();
            if (buildingId === "FACTORY" ||
                    buildingId === "TOWN" ||
                    BUILDING.isHq(building))
            {
                return ["ZCOUNIT_NEOSPIDER_TANK"];
            }
        }
        return [];
    };

    this.getAiCoUnitBonus = function(co, unit, map)
    {
        return 1;
    };
    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("The wunderkind of the Black Hole forces. Small but fierce. Designed most of Black Hole's recent weaponry.");
    };
    this.getHits = function(co)
    {
        return qsTr("Getting her way");
    };
    this.getMiss = function(co)
    {
        return qsTr("Not getting her way");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Skilled at taking advantage of terrain features. Can turn terrain effects into firepower bonuses.");
    };
    this.getLongCODescription = function()
    {
        var text = qsTr("\nSpecial Unit:\nNeo Spider Tank\n\nGlobal Effect: \nLash's units gain +%0% firepower per terrain star.") +
               qsTr("\n\nCO Zone Effect: \nLash's units gain +%1% firepower and +%2% defense.");
        text = replaceTextArgs(text, [CO_LASH.d2dTerrainBonus, CO_LASH.d2dCoZoneOffBonus, CO_LASH.d2dCoZoneDefBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Lash's units gain +%0% firepower and +%1% defence. Their movements are unhindered by terrain.");
        text = replaceTextArgs(text, [CO_LASH.powerOffBonus, CO_LASH.powerDefBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Terrain Tactics");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Terrain stars are multiplied by %0 for all of Lash's units. They gain +%1% firepower and +%2% defence, and their movements are unhindered by terrain.");
        text = replaceTextArgs(text, [CO_LASH.superPowerTerrainDefenseModifier + 1, CO_LASH.powerOffBonus, CO_LASH.powerDefBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Prime Tactics");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Ooh! You're so annoying! I'm gonna have to get rid of you now!"),
                qsTr("You're no fun... I don't like you at all!"),
                qsTr("I've had enough! I'm bored now!"),
                qsTr("Oooo, you're driving me nuts!"),
                qsTr("You want to match wits with me? You're so silly!"),
                qsTr("Tee hee! C'mon! Let's play!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Hee hee hee... Hm? Broken already?"),
                qsTr("Tee hee hee! You're a loser!"),
                qsTr("Huh? That's all you got? Wow, you're no fun!")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("Boo! Nothing's going right! That's enough. I'm going home!"),
                qsTr("Oh, well. I guess I'll have to find someplace new to play. Toodles!")];
    };
    this.getName = function()
    {
        return qsTr("Lash");
    };

    this.showDefaultUnitGlobalBoost = function(co)
    {
        return false;
    };
    this.getCustomUnitGlobalBoostCount = function(co)
    {
        return terrainSpriteManager.getTerrainCount() +
               buildingSpriteManager.getBuildingCount();
    };
    this.getCustomUnitGlobalBoost = function(co, index, info)
    {
        var terrains = terrainSpriteManager.getTerrainsSorted();
        var buildings = buildingSpriteManager.getLoadedBuildings();
        var id = "";
        if (index < terrains.length)
        {
            id = terrains[index];
        }
        else
        {
            id = buildings[index - terrains.length];
        }
        info.setIconId(id);
        info.setLink(id);
        var defenseStars = Global[id].getDefense(null);
        var powerMode = co.getPowerMode();
        var defenseBoost = 0;
        var offensiveBoost = 0;
        if (powerMode === GameEnums.PowerMode_Tagpower ||
            powerMode === GameEnums.PowerMode_Superpower)
        {
            defenseBoost = CO_LASH.powerDefBonus;
            info.addBonusIcon("defenseStar", "+" + defenseStars * CO_LASH.superPowerTerrainDefenseModifier);
            info.addBonusIcon("moveRange", "=1");
            offensiveBoost = CO_LASH.d2dTerrainBonus * defenseStars * (1 + CO_LASH.superPowerTerrainDefenseModifier) + CO_LASH.powerOffBonus;
        }
        else if (powerMode === GameEnums.PowerMode_Power)
        {
            defenseBoost = CO_LASH.powerDefBonus;
            info.addBonusIcon("moveRange", "=1");
            offensiveBoost = CO_LASH.d2dTerrainBonus * defenseStars + CO_LASH.powerOffBonus;
        }
        else
        {
            offensiveBoost = CO_LASH.d2dTerrainBonus * defenseStars;
        }
        info.setOffensiveBoost(offensiveBoost);
        info.setDefensiveBoost(defenseBoost);
    };
    this.showDefaultUnitZoneBoost = function(co)
    {
        return false;
    };
    this.getCustomUnitZoneBoostCount = function(co)
    {
        return terrainSpriteManager.getTerrainCount() +
               buildingSpriteManager.getBuildingCount();
    };
    this.getCustomUnitZoneBoost = function(co, index, info)
    {
        var terrains = terrainSpriteManager.getTerrainsSorted();
        var buildings = buildingSpriteManager.getLoadedBuildings();
        var id = "";
        if (index < terrains.length)
        {
            id = terrains[index];
        }
        else
        {
            id = buildings[index - terrains.length];
        }
        info.setIconId(id);
        info.setLink(id);
        var defenseStars = Global[id].getDefense(null);
        var powerMode = co.getPowerMode();
        var defenseBoost = 0;
        var offensiveBoost = 0;
        if (powerMode === GameEnums.PowerMode_Tagpower ||
            powerMode === GameEnums.PowerMode_Superpower)
        {
            defenseBoost = CO_LASH.powerDefBonus;
            info.addBonusIcon("defenseStar", "+" + defenseStars * CO_LASH.superPowerTerrainDefenseModifier);
            info.addBonusIcon("moveRange", "=1");
            offensiveBoost = CO_LASH.d2dTerrainBonus * defenseStars * (1 + CO_LASH.superPowerTerrainDefenseModifier) + CO_LASH.powerOffBonus;
        }
        else if (powerMode === GameEnums.PowerMode_Power)
        {
            defenseBoost = CO_LASH.powerDefBonus;
            info.addBonusIcon("moveRange", "=1");
            offensiveBoost = CO_LASH.d2dTerrainBonus * defenseStars + CO_LASH.powerOffBonus;
        }
        else
        {
            defenseBoost = CO_LASH.d2dCoZoneDefBonus;
            offensiveBoost = CO_LASH.d2dCoZoneTerrainBonus * defenseStars + CO_LASH.d2dCoZoneOffBonus;
        }
        info.setOffensiveBoost(offensiveBoost);
        info.setDefensiveBoost(defenseBoost);
    };
}

Constructor.prototype = CO;
var CO_LASH = new Constructor();
