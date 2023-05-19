var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(1);
        co.setSuperpowerStars(4);
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
                animation.setSound("power7_1.wav", 1, delay);
            }
            else
            {
                animation.setSound("power7_2.wav", 1, delay);
            }
            if (animations.length < 5)
            {
                animation.addSprite("power7", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power7", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
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
                audio.addMusic("resources/music/cos/bh_power.mp3", 1091 , 49930);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/bh_superpower.mp3", 3161 , 37731);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/bh_tagpower.mp3", 779 , 51141);
                break;
            default:
                audio.addMusic("resources/music/cos/epoch.mp3", 100, 79210)
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
        return "MA";
    };
    this.getHpHidden = function(co, unit, posX, posY, map)
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
                break;
            }
        }
        return false;

    };

    this.superPowerMovementBonus = 1;
    this.superPowerFirerangeBonus = 1;
    this.superPowerLuckBonus = 15;
    this.superPowerOffBonus = 30;
    this.superPowerDefBonus = 30;

    this.powerLuckBonus = 10;
    this.powerOffBonus = 20;
    this.powerDefBonus = 20;

    this.d2dLuckBonus = 0;
    this.d2dOffBonus = 0;
    this.d2dDefBonus = 0;

    this.d2dCoZoneLuckBonus = 10;
    this.d2dCoZoneOffBonus = 20;
    this.d2dCoZoneDefBonus = 10;

    this.getBonusLuck = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return CO_EPOCH.superPowerLuckBonus;
            case GameEnums.PowerMode_Power:
                return CO_EPOCH.powerLuckBonus;
            default:
                if (co.inCORange(Qt.point(posX, posY), unit))
                {
                    return CO_EPOCH.d2dCoZoneLuckBonus;
                }
                return CO_EPOCH.d2dLuckBonus;
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
                return CO_EPOCH.superPowerOffBonus;
            case GameEnums.PowerMode_Power:
                return CO_EPOCH.powerOffBonus;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return CO_EPOCH.d2dCoZoneOffBonus;
                }
                return CO_EPOCH.d2dOffBonus;
            }
        }
        return 0;
    };
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return CO_EPOCH.superPowerDefBonus;
            case GameEnums.PowerMode_Power:
                return CO_EPOCH.powerDefBonus;
            default:
                if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                {
                    return CO_EPOCH.d2dCoZoneDefBonus;
                }
                return CO_EPOCH.d2dDefBonus;
            }
        }
        return 0;
    };
    this.getFirerangeModifier = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            if (unit.getBaseMaxRange() > 1)
            {
                switch (co.getPowerMode())
                {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                    return CO_EPOCH.superPowerFirerangeBonus;
                case GameEnums.PowerMode_Power:
                    break;
                default:
                    break;
                }
            }
        }
        return 0;
    };
    this.getMovementpointModifier = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            if (unit.getBaseMaxRange() === 1)
            {
                if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
                    co.getPowerMode() === GameEnums.PowerMode_Tagpower)
                {
                    return CO_EPOCH.superPowerMovementBonus;
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
                    buildingId === "HQ" ||
                    buildingId === "FORTHQ")
            {
                return ["ZCOUNIT_AUTO_TANK"];
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
        return qsTr("The first prototype for the Robot Commanding Officer Series invented by Lash.");
    };
    this.getHits = function(co)
    {
        return qsTr("Tests");
    };
    this.getMiss = function(co)
    {
        return qsTr("Upgrades");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Epoch is blessed with simple programming so it has only basic command abilities. Powers charge at an unmatched speed.");
    };
    this.getLongCODescription = function()
    {
        let text = qsTr("\nSpecial Unit:\nAuto Tank\n") +
               qsTr("\nGlobal Effect: \nUnits have %0% increased firepower and %1% luck.") +
               qsTr("\n\nCO Zone Effect: \nUnits have %2% increased firepower and %3% luck.");
        text = replaceTextArgs(text, [CO_EPOCH.d2dOffBonus, CO_EPOCH.d2dLuckBonus,
                                      CO_EPOCH.d2dCoZoneOffBonus, CO_EPOCH.d2dCoZoneLuckBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        let text = qsTr("Firepower is increased by %0% and defence by %1% and luck raises by %2% and unit HP is hidden from opponents.");
        text = replaceTextArgs(text, [CO_EPOCH.powerOffBonus, CO_EPOCH.powerDefBonus, CO_EPOCH.powerLuckBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Short Circuit");
    };
    this.getSuperPowerDescription = function(co)
    {
        let text = qsTr("Direct combat units gain %0 movement and indirect combat units receive %1 extra range. Firepower is increased by %2% and defence by %3% and luck raises by %4%. And hides HP from opponents.");
        text = replaceTextArgs(text, [CO_EPOCH.superPowerMovementBonus, CO_EPOCH.superPowerFirerangeBonus,
                                      CO_EPOCH.superPowerOffBonus, CO_EPOCH.superPowerDefBonus, CO_EPOCH.superPowerLuckBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Shocker");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Error: Overcharged. Initiate safety mode."),
                qsTr("Power surge detected."),
                qsTr("Overload! Release energy!"),
                qsTr("System Over. Overload equals very yes."),
                qsTr("Blackout!"),
                qsTr("Register your firmware for continued use!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Epoch Succesful. Upgrade irrelevant"),
                qsTr("Test Completed. Anaylsing battle log..."),
                qsTr("Victory recorded in database and filed under category 'insulting'")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("Noooo... No upgrade please.")];
    };
    this.getName = function()
    {
        return qsTr("Epoch");
    };
}

Constructor.prototype = CO;
var CO_EPOCH = new Constructor();
