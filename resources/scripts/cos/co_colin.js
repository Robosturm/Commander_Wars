var Constructor = function()
{
    this.getCOStyles = function()
    {
        return ["+alt", "+alt2"];
    };

    this.init = function(co, map)
    {
        co.setPowerStars(2);
        co.setSuperpowerStars(4);
    };

    this.activatePower = function(co, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var animation = GameAnimationFactory.createAnimation(map, 0, 0);
        animation.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);
        animation.setSound("power_colin.wav");
        powerNameAnimation.queueAnimation(animation);

        var player = co.getOwner();
        var funds = player.getFunds();
        if (funds > 0)
        {
            player.addFunds(player.getFunds() * CO_COLIN.powerFundsMultiplier);
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
                audio.addMusic("resources/music/cos/power.ogg", 992, 45321);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/superpower.ogg", 1505, 49515);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/tagpower.ogg", 14611, 65538);
                break;
            default:
                audio.addMusic("resources/music/cos/colin.ogg", 6900, 75513)
                break;
            }
        }
    };

    this.getCOUnitRange = function(co, map)
    {
        return 1;
    };
    this.getCOArmy = function()
    {
        return "BM";
    };

    this.superPowerOffBonus = 3.33;

    this.powerFundsMultiplier = 0.5;
    this.powerOffBonus = -5;
    this.powerDefBonus = 10;

    this.d2dCostModifier = -20;
    this.d2dOffBonus = -10;

    this.d2dCoZoneOffBonus = -5;
    this.d2dCoZoneDefBonus = 10;

    this.getCostModifier = function(co, id, baseCost, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            if (map === null ||
                (map !== null && map.getGameRules().getCoGlobalD2D()))
            {
                return baseCost * CO_COLIN.d2dCostModifier / 100;
            }
        }
        return 0;
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                var bonus = attacker.getOwner().getFunds() / 1000 * CO_COLIN.superPowerOffBonus + CO_COLIN.powerOffBonus;
                return bonus;
            case GameEnums.PowerMode_Power:
                return CO_COLIN.powerOffBonus;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return CO_COLIN.d2dCoZoneOffBonus;
                }
                else if (map === null ||
                         (map !== null && map.getGameRules().getCoGlobalD2D()))
                {
                    return CO_COLIN.d2dOffBonus;
                }
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
                return CO_COLIN.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_COLIN.d2dCoZoneDefBonus;
            }
        }
        return 0;
    };
    this.getAiCoUnitBonus = function(co, unit, map)
    {
        return 1;
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
                return ["ZCOUNIT_LOGIC_TRUCK"];
            }
        }
        return [];
    };
    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("Blue Moon's rich boy CO and Sasha's little brother. A gifted CO with a sharp, if insecure, mind.");
    };
    this.getHits = function(co)
    {
        return qsTr("Olaf and Grit");
    };
    this.getMiss = function(co)
    {
        return qsTr("Black Hole");
    };
    this.getCODescription = function(co)
    {
        return qsTr("The heir to a vast fortune who can purchase units at bargain-basement prices. His troops' low firepower stems from his lack of confidence.");
    };
    this.getLongCODescription = function(co, map)
    {
        var values = [0, 0];
        if (map === null ||
            (map !== null && map.getGameRules().getCoGlobalD2D()))
        {
            values = [CO_COLIN.d2dCostModifier, CO_COLIN.d2dOffBonus];
        }
        var text = qsTr("\nSpecial Unit:\nLogistic Truck\n") +
                qsTr("\nGlobal Effect: \nColin's units have a %0% reduction in deployment costs and have %1% firepower.") +
                qsTr("\n\nCO Zone Effect: \nColin's units have %2% firepower and gain +%3% defence.");
        text = replaceTextArgs(text, [values[0], values[1], CO_COLIN.d2dCoZoneOffBonus, CO_COLIN.d2dCoZoneDefBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Colin boosts his deployment funds by +%0%. His units have %1% firepower and gain +%2% defence.");
        text = replaceTextArgs(text, [CO_COLIN.powerFundsMultiplier * 100, CO_COLIN.powerOffBonus, CO_COLIN.powerDefBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Gold Rush");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Colin uses wealth to increase the strength of units. His units start with %1% firepower, but gain +%0% firepower per 1000 funds he has. His units gain +%2% defence.");
        text = replaceTextArgs(text, [CO_COLIN.superPowerOffBonus, CO_COLIN.powerOffBonus, CO_COLIN.powerDefBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Power of Money");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Let me show you the power of money!"),
                qsTr("This is not a drill... I won't give up so easily!"),
                qsTr("People of Blue Moon! I need your help!"),
                qsTr("I'll give it everything I've got!"),
                qsTr("You're not getting away with this!"),
                qsTr("I'll show you what I can do!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("I-I won! Whew!"),
                qsTr("I won because I tried my best!"),
                qsTr("Whew... I won! I really won!")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("If only Commander Olaf were here..."),
                qsTr("Wait 'til I tell my sis! Then you'll be sorry!")];
    };
    this.getName = function()
    {
        return qsTr("Colin");
    };
    this.getAiUsePower = function(co, powerSurplus, turnMode)
    {
        // cop spam
        if (co.canUseSuperpower())
        {
            return GameEnums.PowerMode_Superpower;
        }
        else if (co.canUsePower())
        {
            return GameEnums.PowerMode_Power;
        }
        return GameEnums.PowerMode_Off;
    };
}

Constructor.prototype = CO;
var CO_COLIN = new Constructor();
