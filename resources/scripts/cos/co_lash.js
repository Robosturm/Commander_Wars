var Constructor = function()
{
    this.getCOStyles = function()
    {
        // string array containing the endings of the alternate co style
        // e.g. co_adder has ["+alt"]
        return ["+alt"];
    };

    this.init = function(co)
    {
        co.setPowerStars(4);
        co.setSuperpowerStars(3);
    };

    this.loadCOMusic = function(co)
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

    this.activatePower = function(co)
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
            var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
            animation.setSound("power11.wav");
            if (animations.length < 5)
            {
                animation.addSprite("power11", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power11", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
                animations[counter].queueAnimation(animation);
                animations[counter] = animation;
                counter++;
                if (counter >= animations.length)
                {
                    counter = 0;
                }
            }
        }
        units.remove();
    };

    this.activateSuperpower = function(co, powerMode)
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
            var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
            if (globals.randInt(0, 1) === 0)
            {
                animation.setSound("power12_1.wav");
            }
            else
            {
                animation.setSound("power12_2.wav");
            }
            if (animations.length < 5)
            {
                animation.addSprite("power12", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 1.5, globals.randInt(0, 400));
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power12", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 1.5);
                animations[counter].queueAnimation(animation);
                animations[counter] = animation;
                counter++;
                if (counter >= animations.length)
                {
                    counter = 0;
                }
            }
        }
        units.remove();
    };

    this.getCOUnitRange = function(co)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "BH";
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action)
    {
        if (typeof map !== 'undefined')
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
                        return terrainDefense * 15 + 10;
                    }
                    return 10;
                default:
                    if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                    {
                        return terrainDefense * 15 + 10;
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
                                       defender, defPosX, defPosY, isDefender, action)
    {
        if (co.inCORange(Qt.point(defPosX, defPosY), defender) ||
                co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 10;
        }
        return 0;
    };
    this.getTerrainDefenseModifier = function(co, unit, posX, posY)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return map.getTerrain(posX, posY).getBaseDefense();
            case GameEnums.PowerMode_Power:
                return 0;
            default:
                return 0;
        }
    };
    this.getMovementcostModifier = function(co, unit, posX, posY)
    {
        if (unit.getOwner() === co.getOwner())
        {
            switch (co.getPowerMode())
            {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                    return -999;
                case GameEnums.PowerMode_Power:
                    return -999;
                default:
            }
        }
        return 0;
    };

    this.getCOUnits = function(co, building)
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

    this.getAiCoUnitBonus = function(co, unit)
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
}

Constructor.prototype = CO;
var CO_LASH = new Constructor();
