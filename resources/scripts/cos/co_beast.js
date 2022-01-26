var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
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

            var animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
            animation.writeDataInt32(unit.getX());
            animation.writeDataInt32(unit.getY());
            animation.writeDataInt32(2);
            animation.setEndOfAnimationCall("ANIMATION", "postAnimationHeal");
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

            var animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
            animation.writeDataInt32(unit.getX());
            animation.writeDataInt32(unit.getY());
            animation.writeDataInt32(3);
            animation.setEndOfAnimationCall("ANIMATION", "postAnimationHeal");
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
        units.remove();
    };

    this.loadCOMusic = function(co)
    {
        // put the co music in here.
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Power:
            audio.addMusic("resources/music/cos/power_ids_dc.mp3", 0, 0);
            break;
        case GameEnums.PowerMode_Superpower:
            audio.addMusic("resources/music/cos/power_ids_dc.mp3", 0, 0);
            break;
        case GameEnums.PowerMode_Tagpower:
            audio.addMusic("resources/music/cos/bh_tagpower.mp3", 779 , 51141);
            break;
        default:
            audio.addMusic("resources/music/cos/beast.mp3",  0, 0);
            break;
        }
    };

    this.getCOUnitRange = function(co)
    {
        return 2;
    };
    this.getCOArmy = function()
    {
        return "DM";
    };
    this.coZoneBonus = 60;
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (!isDefender)
            {
                return CO_BEAST.coZoneBonus;
            }
            break;
        default:
            if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker) && !isDefender)
            {
                return CO_BEAST.coZoneBonus;
            }
            break;
        }
        return 0;
    };
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action)
    {
        if (co.inCORange(Qt.point(defPosX, defPosY), defender) ||
                co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 10;
        }
        return 0;
    };
    this.getMovementpointModifier = function(co, unit, posX, posY)
    {
        if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
                co.getPowerMode() === GameEnums.PowerMode_Tagpower)
        {
            return 2;
        }
        return 0;
    };
    this.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked, weapon, action)
    {
        if (co.inCORange(Qt.point(attacker.getX(), attacker.getY()), attacker))
        {
            if (attacker.getOwner() === co.getOwner() && attacker.getHp() > 0)
            {
                attacker.setHp(attacker.getHp() - 1);
            }
        }
    };
    this.getAiCoUnitBonus = function(co, unit)
    {
        return 1;
    };
    this.getCOUnits = function(co, building)
    {
        var buildingId = building.getBuildingID();
        if (buildingId === "FACTORY" ||
            buildingId === "TOWN" ||
            buildingId === "HQ")
        {
            return ["ZCOUNIT_AT_CYCLE"];
        }
        return [];
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A bandit working for Caulder. Believes there is no law but The Beast's Law. Driven by rage and greed.");
    };
    this.getHits = function(co)
    {
        return qsTr("Looting, Robbing and Killing");
    };
    this.getMiss = function(co)
    {
        return qsTr("Duty and Honor");
    };
    this.getCODescription = function(co)
    {
        return qsTr("His units have the potential for high firepower, but their reckless tendencies often get them hurt.");
    };
    this.getLongCODescription = function()
    {
        var text = qsTr("\nSpecial Unit:\nAT Cycle\n") +
                   qsTr("\nGlobal Effect: \nNone.") +
                   qsTr("\n\nCO Zone Effect: \nUnits gain %0% firepower when attacking but also receive 1 HP of extra damage in recoil.");
        text = replaceTextArgs(text, [CO_BEAST.coZoneBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("Restores two HP to all units.");
    };
    this.getPowerName = function(co)
        {
            return qsTr("Steroids");
        };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("Restores 3 HP to all units. Units movement is increases by one space.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Beast's Law");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("The Beast will tear you to shreds!"),
                qsTr("I need a place to aim!"),
                qsTr("Leave their corpses where they fall!"),
                qsTr("Graaahhhh!!"),
                qsTr("Kill anyone who stands in the way!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Gwar har har har!"),
                qsTr("You can't stand up to The Beast!"),
                qsTr("There's no law like the Beast's law!")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("You ain't seen the last of The Beast!"),
                qsTr("Head for the hills, roaches! We're outmanned!")];
    };
    this.getName = function()
    {
        return qsTr("Beast");
    };
}

Constructor.prototype = CO;
var CO_BEAST = new Constructor();
