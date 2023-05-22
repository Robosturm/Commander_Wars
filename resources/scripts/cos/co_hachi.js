var Constructor = function()
{
    this.getCOStyles = function()
    {
        return ["+alt", "+alt2"];
    };

    this.init = function(co, map)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(2);
    };

    this.activatePower = function(co, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var animation2 = GameAnimationFactory.createAnimation(map, 0, 0);
        animation2.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation2.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);
        powerNameAnimation.queueAnimation(animation2);
    };

    this.activateSuperpower = function(co, powerMode, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        powerNameAnimation.queueAnimationBefore(dialogAnimation);

        var animation2 = GameAnimationFactory.createAnimation(map, 0, 0);
        animation2.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation2.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);
        powerNameAnimation.queueAnimation(animation2);
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
                audio.addMusic("resources/music/cos/hachi.mp3", 12402, 68059);
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
        return "OS";
    };

    this.powerCostReduction = 0.5;
    this.powerOffBonus = 10;
    this.powerDefBonus = 10;

    this.d2dCostReduction = 0.1;

    this.d2dCoZoneOffBonus = 10;
    this.d2dCoZoneDefBonus = 10;

    this.getCostModifier = function(co, id, baseCost, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
            case GameEnums.PowerMode_Power:
                return -baseCost * CO_HACHI.powerCostReduction;
            default:
                break;
            }
            return -baseCost * CO_HACHI.d2dCostReduction;
        }
        return 0;
    };

    this.getAdditionalBuildingActions = function(co, building, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (building.getBuildingID() === "TOWN" &&
                        building.getOwner() === co.getOwner())
                {
                    return "ACTION_BUILD_UNITS";
                }
                break;
            case GameEnums.PowerMode_Power:
                break;
            default:
                break;
            }
        }
        return "";
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
                return CO_HACHI.powerOffBonus;
            case GameEnums.PowerMode_Power:
                return CO_HACHI.powerOffBonus;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return CO_HACHI.d2dCoZoneOffBonus;
                }
                break;
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
                return CO_HACHI.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_HACHI.d2dCoZoneDefBonus;
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
                    buildingId === "HQ" ||
                    buildingId === "FORTHQ")
            {
                return ["ZCOUNIT_SMUGGLER"];
            }
        }
        return [];
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("Owner of the Battle Maps shop. Rumored to be Orange Star's former commander-in-chief.");
    };
    this.getHits = function(co)
    {
        return qsTr("Tea");
    };
    this.getMiss = function(co)
    {
        return qsTr("Medicine");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Uses secret trade routes to get slightly lower deployment costs for all units.");
    };
    this.getLongCODescription = function()
    {
        var text = qsTr("\nSpecial Unit:\nSmuggler\n") +
               qsTr("\nGlobal Effect: \nUnits are %0% cheaper") +
               qsTr("\n\nCO Zone Effect: \nUnits have %1% more firepower and %2% defense.");
        text = replaceTextArgs(text, [CO_HACHI.d2dCostReduction * 100, CO_HACHI.d2dCoZoneOffBonus, CO_HACHI.d2dCoZoneDefBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Speaks with such authority that he obtains %0% lower deployment costs.");
        text = replaceTextArgs(text, [CO_HACHI.powerCostReduction * 100]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Barter");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Merchant pals gather from around the globe and help him deploy ground units from any allied city with %0% lower deployment costs.");
        text = replaceTextArgs(text, [CO_HACHI.powerCostReduction * 100]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Merchant Union");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("This brings back memories!"),
                qsTr("Runnin' away won't prove anything! Stand your ground, soldier!"),
                qsTr("No need to get all worked up!"),
                qsTr("Hey, I'm no retiree!"),
                qsTr("I may be old, but I can still rumble!"),
                qsTr("This is my best seller!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Battles cost too much!"),
                qsTr("Rematches are free!"),
                qsTr("Thank you, come again!")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("I'm getting too old for this."),
                qsTr("Shop is closing for today...")];
    };
    this.getName = function()
    {
        return qsTr("Hachi");
    };
}

Constructor.prototype = CO;
var CO_HACHI = new Constructor();
