var Constructor = function()
{
    this.getCOStyles = function()
    {
        return ["+alt"];
    };

    this.init = function(co, map)
    {
        co.setPowerStars(2);
        co.setSuperpowerStars(4);
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
                audio.addMusic("resources/music/cos/sasha.ogg", 15916, 105733);
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
        animation2.setSound("power_sasha.wav");
        powerNameAnimation.queueAnimation(animation2);

        var player = co.getOwner();
        // i prefer this version you could change it to 10% per funds easily
        var reduction = co.getOwner().getFunds() / CO_SASHA.powerFundsPerReduction;
        var playerCounter = map.getPlayerCount();
        for (var i2 = 0; i2 < playerCounter; i2++)
        {
            var enemyPlayer = map.getPlayer(i2);
            if ((enemyPlayer !== player) &&
                (player.checkAlliance(enemyPlayer) === GameEnums.Alliance_Enemy))
            {
                var enemyCo = enemyPlayer.getCO(0);
                if (enemyCo !== null)
                {
                    enemyCo.setPowerFilled(enemyCo.getPowerFilled() - reduction);
                }
                enemyCo = enemyPlayer.getCO(1);
                if (enemyCo !== null)
                {
                    enemyCo.setPowerFilled(enemyCo.getPowerFilled() - reduction);
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
        var size = units.size();
        for (var i = 0; i < size; i++)
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
        return 2;
    };
    this.getCOArmy = function()
    {
        return "BM";
    };

    this.superPowerIncomeBonus = 0.5;
    this.powerFundsPerReduction = 5000;

    this.d2dPercentIncomeBonus = 0.1;
    this.d2dFlatIncomeBonus = 0;

    this.powerOffBonus = 20;
    this.powerDefBonus = 10;

    this.d2dCoZoneOffBonus = 20;
    this.d2dCoZoneDefBonus = 10;

    this.getBonusIncome = function(co, building, income, map)
    {
        if (CO.isActive(co))
        {
            if (map === null ||
                (map !== null && map.getGameRules().getCoGlobalD2D()))
            {
                return income * CO_SASHA.d2dPercentIncomeBonus + CO_SASHA.d2dFlatIncomeBonus;
            }
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
                return CO_SASHA.powerOffBonus;
            }
            else if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                return CO_SASHA.d2dCoZoneOffBonus;
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
                return CO_SASHA.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_SASHA.d2dCoZoneDefBonus;
            }
        }
        return 0;
    };
    this.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked, weapon, action, map)
    {
        if (CO.isActive(co) && defender !== null)
        {
            if (gotAttacked === false && attacker.getOwner() === co.getOwner())
            {
                switch (co.getPowerMode())
                {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                    if (atkDamage > 0)
                    {
                        co.getOwner().addFunds(atkDamage / 10.0 * defender.getUnitCosts() * CO_SASHA.superPowerIncomeBonus);
                    }
                    break;
                case GameEnums.PowerMode_Power:
                    break;
                default:
                    break;
                }
            }
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
                return ["ZCOUNIT_LOGIC_TRUCK"];
            }
        }
        return [];
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("Colin's older sister. Normally ladylike, but becomes daring when she gets angry.");
    };
    this.getHits = function(co)
    {
        return qsTr("Truffles");
    };
    this.getMiss = function(co)
    {
        return qsTr("Pork rinds");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Being the heir to a vast fortune, she gets additional funds from allied properties.");
    };
    this.getLongCODescription = function(co, map)
    {
        var values = [0, 0];
        if (map === null ||
            (map !== null && map.getGameRules().getCoGlobalD2D()))
        {
            values = [CO_SASHA.d2dPercentIncomeBonus * 100, CO_SASHA.d2dFlatIncomeBonus];
        }
        var text = qsTr("\nSpecial Unit:\nLogistic Truck\n") +
               qsTr("\nGlobal Effect: \nSasha's properties produce an additional +%0% funding, as well as an additional flat +%1 funds.") +
               qsTr("\n\nCO Zone Effect: \nSasha's units gain +%2% firepower and +%3% defence.");
        text = replaceTextArgs(text, [values[0], values[1], CO_SASHA.d2dCoZoneOffBonus, CO_SASHA.d2dCoZoneDefBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("All enemies lose one CO Power charge star for every %0 funds Sasha currently has. Sasha's units gain +%1% firepower and +%2% defence.");
        text = replaceTextArgs(text, [CO_SASHA.powerFundsPerReduction, CO_SASHA.powerOffBonus, CO_SASHA.powerDefBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Market Crash");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Sasha's units gain +%1% firepower and +%2% defence. For this turn, Sasha receives %0% of all combat damage dealt to enemies as funds.");
        text = replaceTextArgs(text, [CO_SASHA.superPowerIncomeBonus * 100, CO_SASHA.powerOffBonus, CO_SASHA.powerDefBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("War Bonds");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Wealth is power."),
                qsTr("I won't go easy on you."),
                qsTr("I'll keep fighting until peace is restored."),
                qsTr("I will not back down."),
                qsTr("I'll show you my true strength."),
                qsTr("You've made me very, very... upset.")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Even kittens have claws."),
                qsTr("Money is power."),
                qsTr("Bravo!")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("This is like some horrible dream."),
                qsTr("This is such a nightmare.")];
    };
    this.getName = function()
    {
        return qsTr("Sasha");
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
var CO_SASHA = new Constructor();
