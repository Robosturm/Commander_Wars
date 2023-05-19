var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
    };

    this.getCOStyles = function()
    {
        return ["+alt"];
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
            if (i % 2 === 0)
            {
                animation.setSound("power10_1.wav", 1, delay);
            }
            else
            {
                animation.setSound("power10_2.wav", 1, delay);
            }
            if (animations.length < 5)
            {
                animation.addSprite("power14", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power14", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
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

    this.loadCOMusic = function(co, map)
    {
        if (CO.isActive(co))
        {
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
                audio.addMusic("resources/music/cos/sabaki.mp3", 42287, 86073);
                break;
            }
        }
    };

    this.getCOUnitRange = function(co, map)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "AC";
    };

    this.superPowerHeal = 1;

    this.powerHeal = 0.5;
    this.powerDefBonus = 10;
    this.powerOffBonus = 10;

    this.d2dCoZoneDefBonus = 10;
    this.d2dCoZoneOffBonus = 10;
    this.d2dCoZoneHeal = 0.4;

    this.d2dHeal = 0.0;
    
    this.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked, weapon, action, map)
    {
        if (CO.isActive(co))
        {
            if (gotAttacked === false && attacker.getOwner() === co.getOwner())
            {
                var healPercent = 0.0;
                switch (co.getPowerMode())
                {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                    healPercent = CO_SABAKI.superPowerHeal;
                    break;
                case GameEnums.PowerMode_Power:
                    healPercent = CO_SABAKI.powerHeal;
                    break;
                default:
                    if (co.inCORange(Qt.point(attacker.getX(), attacker.getY()), attacker))
                    {
                        healPercent = CO_SABAKI.d2dCoZoneHeal;
                    }
                    else
                    {
                        healPercent = CO_SABAKI.d2dHeal;
                    }
                    break;

                }
                if (atkDamage > 0)
                {
                    attacker.setHp(attacker.getHp() + atkDamage * healPercent);
                }
            }
        }
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return CO_SABAKI.powerOffBonus;
        }
        else if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
        {
            return CO_SABAKI.d2dCoZoneOffBonus;
        }
        return 0;
    };

    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        if (co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return CO_SABAKI.powerDefBonus;
        }
        else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
        {
            return CO_SABAKI.d2dCoZoneDefBonus;
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
                    buildingId === "HQ" ||
                    buildingId === "FORTHQ")
            {
                return ["ZCOUNIT_CRYSTAL_TANK"];
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
        return qsTr("A former Yellow Comet CO that started a rebellion, when she saw the state of Amber Corona. An expert at salvaging parts and field repairs.");
    };
    this.getHits = function(co)
    {
        return qsTr("Spare parts");
    };
    this.getMiss = function(co)
    {
        return qsTr("Poverty");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Highly skilled at salvaging parts, Sabaki's units can drain HP from enemies.");
    };
    this.getLongCODescription = function()
    {
        let text = qsTr("\nSpecial Unit:\nCrystal Tanks\n\nGlobal Effect: \nUnits have %0% lifesteal.") +
            qsTr("\n\nCO Zone Effect: \nUnits have %1% lifesteal.");
        text = replaceTextArgs(text, [CO_SABAKI.d2dHeal * 100, CO_SABAKI.d2dCoZoneHeal * 100]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        let text = qsTr("Units have %0% lifesteal.");
        text = replaceTextArgs(text, [CO_SABAKI.powerHeal * 100]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Speed Salvage");
    };
    this.getSuperPowerDescription = function(co)
    {
        let text = qsTr("Units have %0% lifesteal.");
        text = replaceTextArgs(text, [CO_SABAKI.superPowerHeal * 100]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Rejuvenation Raid");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Grab those parts! There's a tank that needs fixing!"),
				qsTr("Get those machines up and running! We need to push forward!"),
				qsTr("Every machine has a weakness, you just have to know where to look."),
				qsTr("You may think I'm nuts, but you're about to get screwed..."),
				qsTr("Time to throw a wrench in their plans."),
				qsTr("Let's see how you react when the hammer falls...")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("My army acts like a well-oiled machine."),
				qsTr("Nothing's more satisfying than killing an enemy with their own gun."),
				qsTr("Your army just fell apart out there.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("The revolution isn't over."),
				qsTr("Even by healing my troops.. I couldn't win.")];
    };
    this.getName = function()
    {
        return qsTr("Sabaki");
    };
}

Constructor.prototype = CO;
var CO_SABAKI = new Constructor();
