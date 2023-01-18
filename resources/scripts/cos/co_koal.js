var Constructor = function()
{
    this.getCOStyles = function()
    {
        // string array containing the endings of the alternate co style
        
        return ["+alt"];
    };

    this.init = function(co, map)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(2);
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
                audio.addMusic("resources/music/cos/koal.mp3", 14166, 103617);
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
            animation.setSound("power0.wav", 1, delay);
            if (animations.length < 5)
            {
                animation.addSprite("power0", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power0", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
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
        return "BG";
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (map !== null)
        {
            if (map.onMap(atkPosX, atkPosY))
            {
                var terrainID = map.getTerrain(atkPosX, atkPosY).getID();
                var isStreet = (terrainID === "STREET") ||
                               (terrainID === "STREET1") ||
                               (terrainID === "SNOW_STREET") ||
                               (terrainID === "BRIDGE") ||
                               (terrainID === "BRIDGE1") ||
                               (terrainID === "BRIDGE2") ||
                               (terrainID === "WASTE_PATH") ||
                               (terrainID === "DESERT_PATH") ||
                               (terrainID === "DESERT_PATH1");
                switch (co.getPowerMode())
                {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                    if (isStreet)
                    {
                        return 70;
                    }
                    return 10;
                case GameEnums.PowerMode_Power:
                    if (isStreet)
                    {
                        return 50;
                    }
                    return 10;
                default:
                    if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                    {
                        if (isStreet)
                        {
                            return 50;
                        }
                        return 10;
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

    this.getMovementpointModifier = function(co, unit, posX, posY, map)
    {
        if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
            co.getPowerMode() === GameEnums.PowerMode_Tagpower)
        {
            return 2;
        }
		else if (co.getPowerMode() === GameEnums.PowerMode_Power)
        {
            return 1;
        }
        return 0;
    };

    this.getAiCoUnitBonus = function(co, unit, map)
    {
        return 1;
    };
    this.getCOUnits = function(co, building, map)
    {
        var buildingId = building.getBuildingID();
        if (buildingId === "FACTORY" ||
            buildingId === "TOWN" ||
            buildingId === "HQ")
        {
            return ["ZCOUNIT_HOT_TANK"];
        }
        return [];
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A commander of the Black Hole army who is always planning his next destructive act.Endless March");
    };
    this.getHits = function(co)
    {
        return qsTr("Proverbs");
    };
    this.getMiss = function(co)
    {
        return qsTr("Fondue");
    };
    this.getCODescription = function(co)
    {
        return qsTr("A master of road-based battles. Firepower of all units increased on roads.");
    };
    this.getLongCODescription = function()
    {
        return qsTr("\nSpecial Unit:\nHot Tank\n") +
               qsTr("\nGlobal Effect: \nNo Effects.") +
               qsTr("\n\nCO Zone Effect: \nUnits gain increased firepower on street.");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("Movement range for all units is increased by one space. Units have more firepower on roads.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Forced March");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("Movement range for all units is increased by two spaces. Greatly increases firepower of units on roads.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Trail of Woe");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("I will crush your units, one by one!"),
                qsTr("I am a warrior and a scholar. My victory is all but certain."),
                qsTr("Heh heh heh. I'll not surrender! Bend your knees and beg for mercy!"),
                qsTr("Heh heh heh... None shall escape."),
                qsTr("My speed knows no equal!"),
                qsTr("Heh heh heh... Fate has smiled on me this day!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("I have no equal on the field of war!"),
                qsTr("Wallow in your shame, swine! Wallow, I say"),
                qsTr("Victory is no accident.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("He! He! He!...??? What?"),
                qsTr("One time you win and another time you loose.")];
    };
    this.getName = function()
    {
        return qsTr("Koal");
    };
}

Constructor.prototype = CO;
var CO_KOAL = new Constructor();
