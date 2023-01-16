var Constructor = function()
{
    this.getCOStyles = function()
    {
        // string array containing the endings of the alternate co style
        
        return ["+alt", "+alt2"];
    };

    this.init = function(co, map)
    {
        co.setPowerStars(4);
        co.setSuperpowerStars(3);
    };

    this.loadCOMusic = function(co, map)
    {
        // put the co music in here.
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
    this.globalTerrainBonus = 0;
    this.zoneTerrainBonus = 10;
    this.terrainDefenseModifier = 1;
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
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
                        return terrainDefense * CO_LASH.globalTerrainBonus + 10;
                    }
                    return 10;
                default:
                    if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                    {
                        return terrainDefense * CO_LASH.zoneTerrainBonus + 10;
                    }
                    break;
                }
            }
        }
        else if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
        {
            return 10;
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
    this.getTerrainDefenseModifier = function(co, unit, posX, posY, map)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return map.getTerrain(posX, posY).getBaseDefense() * CO_LASH.terrainDefenseModifier;
            case GameEnums.PowerMode_Power:
                return 0;
            default:
                return 0;
        }
    };
    this.getMovementcostModifier = function(co, unit, posX, posY, map)
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
        return 0;
    };

    this.getCOUnits = function(co, building, map)
    {
        var buildingId = building.getBuildingID();
        if (buildingId === "FACTORY" ||
            buildingId === "TOWN" ||
            buildingId === "HQ")
        {
            return ["ZCOUNIT_NEOSPIDER_TANK"];
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
        return qsTr("The wunderkind of the Black Hole forces. She's small but fierce. Designed most of Black Hole's recent weaponry.");
    };
    this.getHits = function(co)
    {
        return qsTr("Getting her way");
    };
    this.getMiss = function(co)
    {
        return qsTr("Not getting it");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Skilled at taking advantage of terrain features. Can turn terrain effects into firepower bonuses.");
    };
    this.getLongCODescription = function()
    {
        return qsTr("\nSpecial Unit:\nNeo Spider Tank\n\nGlobal Effect: \nNo Effects.") +
               qsTr("\n\nCO Zone Effect: \nUnits gain increased firepower per defense star.");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("All units' movements are unhindered by terrain.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Terrain Tactics");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("Terrain effects are doubled and used to increase firepower. Additionally, all units movements are unhindered by terrain.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Prime Tactics");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Ooh!  You're so annoying!  I'm gonna have to get rid of you now!"),
                qsTr("You're no fun... I don't like you at all!"),
                qsTr("I've had enough!  I'm bored now!"),
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
            defenseBoost = 10;
            info.addBonusIcon("defenseStar", "+" + defenseStars * CO_LASH.terrainDefenseModifier);
            info.addBonusIcon("moveRange", "=1");
            offensiveBoost = CO_LASH.zoneTerrainBonus * defenseStars * (1 + CO_LASH.terrainDefenseModifier) + 10;
        }
        else if (powerMode === GameEnums.PowerMode_Power)
        {
            defenseBoost = 10;
            info.addBonusIcon("moveRange", "=1");
            offensiveBoost = CO_LASH.zoneTerrainBonus * defenseStars + 10;
        }
        else
        {
            offensiveBoost = CO_LASH.globalTerrainBonus * defenseStars;
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
            defenseBoost = 10;
            info.addBonusIcon("defenseStar", "+" + defenseStars * CO_LASH.terrainDefenseModifier);
            info.addBonusIcon("moveRange", "=1");
            offensiveBoost = CO_LASH.zoneTerrainBonus * defenseStars * (1 + CO_LASH.terrainDefenseModifier) + 10;
        }
        else if (powerMode === GameEnums.PowerMode_Power)
        {
            defenseBoost = 10;
            info.addBonusIcon("moveRange", "=1");
            offensiveBoost = CO_LASH.zoneTerrainBonus * defenseStars + 10;
        }
        else
        {
            offensiveBoost = CO_LASH.zoneTerrainBonus * defenseStars + 10;
        }
        info.setOffensiveBoost(offensiveBoost);
        info.setDefensiveBoost(defenseBoost);
    };
}

Constructor.prototype = CO;
var CO_LASH = new Constructor();
