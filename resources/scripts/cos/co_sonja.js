var Constructor = function()
{
    this.getCOStyles = function()
    {
        return ["+alt", "+alt2", "+alt3", "+alt4", "+alt5", "+alt6"];
    };

    this.init = function(co, map)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(2);
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
                audio.addMusic("resources/music/cos/sonja.mp3", 4059, 60078);
                break;
            }
        }
    };

    this.activatePower = function(co, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var animation2 = GameAnimationFactory.createAnimation(map, 0, 0);
        animation2.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation2.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);
        animation2.setSound("power_clear.wav");
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
        animation2.setSound("power_clear.wav");
        powerNameAnimation.queueAnimation(animation2);
    };

    this.getCOUnitRange = function(co, map)
    {
        return 2;
    };
    this.getCOArmy = function()
    {
        return "YC";
    };

    this.superPowerEnemyTerrainDefenseModifier = 3;

    this.powerOffBonus = 10;
    this.powerDefBonus = 10;
    this.powerVisionBonus = 2;
    this.powerEnemyTerrainDefenseModifier = 2;

    this.d2dCoZoneOffBonus = 10;
    this.d2dCoZoneDefBonus = 10;

    this.d2dVisionBonus = 1;
    this.d2dEnemyTerrainDefenseModifier = 1;
    this.d2dBonusMisfortune = 5;
    this.d2dCounterAttackBonus = 0;

    this.getBonusMisfortune = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            return CO_SONJA.d2dBonusMisfortune;
        }
        return 0;
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            if (co.getPowerMode() > GameEnums.PowerMode_Off)
            {
                if (isDefender && CO_SONJA.counterAttackBonus !== 0)
                {
                    return CO_SONJA.d2dCounterAttackBonus;
                }
                return CO_SONJA.powerOffBonus;
            }
            else if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                if (isDefender && CO_SONJA.counterAttackBonus !== 0)
                {
                    return CO_SONJA.d2dCounterAttackBonus;
                }
                return CO_SONJA.d2dCoZoneOffBonus;
            }
            else if (isDefender && CO_SONJA.counterAttackBonus !== 0)
            {
                return CO_SONJA.d2dCounterAttackBonus;
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
                return CO_SONJA.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_SONJA.d2dCoZoneDefBonus;
            }
        }
        return 0;
    };
    this.getVisionrangeModifier = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
            case GameEnums.PowerMode_Power:
                return CO_SONJA.powerVisionBonus;
            default:
                return CO_SONJA.d2dVisionBonus;
            }
        }
        return 0;
    };
    this.getEnemyTerrainDefenseModifier = function(co, unit, posX, posY, map)
    {
        if (co.getIsCO0() === true)
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return -CO_SONJA.superPowerEnemyTerrainDefenseModifier;
            case GameEnums.PowerMode_Power:
                return -CO_SONJA.powerEnemyTerrainDefenseModifier;
            default:
                return -CO_SONJA.d2dEnemyTerrainDefenseModifier;
            }
        }
        return 0;
    };

    this.getPerfectVision = function(co, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
            case GameEnums.PowerMode_Power:
                return true;
            default:
                return false;
            }
        }
    };

    this.getFirstStrike = function(co, unit, posX, posY, attacker, isDefender, map, atkPosX, atkPosY)
    {
        if (CO.isActive(co))
        {
            if(unit !== null)
            {
                switch (co.getPowerMode())
                {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                    if (isDefender)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                case GameEnums.PowerMode_Power:
                    return false;
                default:
                    return false;
                }
            }
        }
        return false;
    };

    this.getHpHidden = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            return true;
        }
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
                return ["ZCOUNIT_INTEL_TRUCK"];
            }
        }
        return [];
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("Kanbei's cool and collected daughter who likes to plan before acting. She excels in gathering information.");
    };
    this.getHits = function(co)
    {
        return qsTr("Computers");
    };
    this.getMiss = function(co)
    {
        return qsTr("Bugs");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Keeps HP intel hidden from foes and reduces enemy terrain defensive cover. However, she suffers from chronic bad luck.");
    };
    this.getLongCODescription = function()
    {
        var text = qsTr("\nSpecial Unit:\nIntel Truck\n") +
            qsTr("\nGlobal Effect: \nSonja's units gain +%4 vision, +%1 misfortune, and their HP is hidden from enemies. Enemies lose -%0 terrain star from all terrain.") +
            qsTr("\n\nCO Zone Effect: \nSonja's units gain +%2% firepower and +%3% defence.");
        text = replaceTextArgs(text, [CO_SONJA.d2dEnemyTerrainDefenseModifier, CO_SONJA.d2dBonusMisfortune, CO_SONJA.d2dCoZoneOffBonus, CO_SONJA.d2dCoZoneDefBonus, CO_SONJA.d2dVisionBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Sonja's units gain +%0 vision, +%1% firepower, +%2% defence, and can see into hidden locations. Enemies lose -%3 terrain stars from all terrain.");
        text = replaceTextArgs(text, [CO_SONJA.powerVisionBonus, CO_SONJA.powerOffBonus, CO_SONJA.powerDefBonus, CO_SONJA.powerEnemyTerrainDefenseModifier]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Enhanced Vision");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Sonja's units gain +%0 vision, +%1% firepower, and +%2% defence. They can see into hidden locations and strike first, even during counterattacks. Enemies lose -%3 terrain stars from all terrain.");
        text = replaceTextArgs(text, [CO_SONJA.powerVisionBonus, CO_SONJA.powerOffBonus, CO_SONJA.powerDefBonus, CO_SONJA.superPowerEnemyTerrainDefenseModifier]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Counter Break");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("I see right through your plans!"),
                qsTr("I'll show you there's more to combat than power alone!"),
                qsTr("Number of enemy troops... location... Got it! This is so easy!"),
                qsTr("Get me some fresh intel, stat!"),
                qsTr("You can't hide from me!"),
                qsTr("Just like I planned...")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("My strategy was merely better... That is all."),
                qsTr("Perfect planning wins the day!")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("I'm sorry, Father... I've broken my promise..."),
				qsTr("I must review my strategy again..."),
                qsTr("I have learned much from this...")];
    };
    this.getName = function()
    {
        return qsTr("Sonja");
    };
}

Constructor.prototype = CO;
var CO_SONJA = new Constructor();
