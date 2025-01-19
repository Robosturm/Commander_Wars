var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(4);
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

        var player = co.getOwner();
        var income = 0;
        var playerCounter = map.getPlayerCount();
        for (var i2 = 0; i2 < playerCounter; i2++)
        {
            var enemyPlayer = map.getPlayer(i2);
            if ((enemyPlayer !== player) &&
                (player.checkAlliance(enemyPlayer) === GameEnums.Alliance_Enemy))
            {
                income += enemyPlayer.calcIncome(1.0);
            }
        }
        player.addFunds(income * CO_VARLOT.powerIncome / 100);
    };

    this.activateSuperpower = function(co, powerMode, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        powerNameAnimation.queueAnimationBefore(dialogAnimation);

        var variables = co.getVariables();
        var bonusUnitVariable = variables.createVariable("BONUSUNITS");
        var bonusOffVariable = variables.createVariable("BONUSOFF");
        var bonusUnits = bonusUnitVariable.readDataListInt32();
        var bonusOffs = bonusOffVariable.readDataListInt32();

        var costs = 0;
        var units = co.getOwner().getUnits();
        var animations = [];
        var counter = 0;
        units.randomize();
        var size = units.size();
        for (var i = 0; i < size; i++)
        {
            var unit = units.at(i);

            // apply cost and offense boni
            var healing = unit.getHpRounded() + CO_VARLOT.superPowerHeal;
            if (healing > 10)
            {
                costs += unit.getUnitCosts() * (10 - unit.getHpRounded()) / 10;
                var unitId = unit.getUniqueID();
                bonusUnits.push(unitId);
                bonusOffs.push((healing - 10) * CO_VARLOT.superPowerOverhealOffBonus);
            }
            else
            {
                costs += unit.getUnitCosts() * CO_VARLOT.superPowerHeal / 10;
            }

            var animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
            animation.writeDataInt32(unit.getX());
            animation.writeDataInt32(unit.getY());
            animation.writeDataInt32(4);
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

        bonusUnitVariable.writeDataListInt32(bonusUnits);
        bonusOffVariable.writeDataListInt32(bonusOffs);

        // reduce enemy funds
        var player = co.getOwner();
        var enemyCount = player.getEnemyCount();
        var playerCounter = map.getPlayerCount();
        var playerCosts = costs / enemyCount;
        for (var i2 = 0; i2 < playerCounter; i2++)
        {
            var enemyPlayer = map.getPlayer(i2);
            if ((enemyPlayer !== player) &&
                (player.checkAlliance(enemyPlayer) === GameEnums.Alliance_Enemy))
            {
                var funds = enemyPlayer.getFunds();
                var income = enemyPlayer.calcIncome();
                if (playerCosts > income + funds)
                {
                    enemyPlayer.setFunds(-income);
                }
                else
                {
                    enemyPlayer.addFunds(-playerCosts);
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
                audio.addMusic("resources/music/cos/bh_power.ogg", 1091 , 49930);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/bh_superpower.ogg", 3161 , 37731);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/bh_tagpower.ogg", 779 , 51141);
                break;
            default:
                audio.addMusic("resources/music/cos/varlot.ogg", 2418, 74440);
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
        return "DM";
    };

    this.superPowerHeal = 4;
    this.superPowerOverhealOffBonus = 10;

    this.powerCaptureBonus = 5;
    this.powerIncome = 25;
    this.powerOffBonus = 20;
    this.powerDefBonus = 20;

    this.d2dOffBonus = 0;
    this.d2dDefBonus = 0;

    this.d2dCoZoneOffBonus = 20;
    this.d2dCoZoneDefBonus = 20;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                var variables = co.getVariables();
                var bonusUnitVariable = variables.createVariable("BONUSUNITS");
                var bonusOffVariable = variables.createVariable("BONUSOFF");
                var bonusUnits = bonusUnitVariable.readDataListInt32();
                var bonusOffs = bonusOffVariable.readDataListInt32();
                var unitId = attacker.getUniqueID();
                var index = bonusUnits.indexOf(unitId);
                if (index >= 0)
                {
                    return CO_VARLOT.powerOffBonus + bonusOffs[index];
                }
                return CO_VARLOT.powerOffBonus;
            case GameEnums.PowerMode_Power:
                return CO_VARLOT.powerOffBonus;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return CO_VARLOT.d2dCoZoneOffBonus;
                }
                else if (map === null ||
                         (map !== null && map.getGameRules().getCoGlobalD2D()))
                {
                    return CO_VARLOT.d2dOffBonus;
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
                return CO_VARLOT.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_VARLOT.d2dCoZoneDefBonus;
            }
            else if (map === null ||
                     (map !== null && map.getGameRules().getCoGlobalD2D()))
            {
                return CO_VARLOT.d2dDefBonus;
            }
        }
        return 0;
    };

    this.startOfTurn = function(co, map)
    {
        // reset repair stop
        var variables = co.getVariables();
        var bonusUnitVariable = variables.createVariable("BONUSUNITS");
        var empty = [];
        bonusUnitVariable.writeDataListInt32(empty);
        var bonusOffVariable = variables.createVariable("BONUSOFF");
        bonusOffVariable.writeDataListInt32(empty);
    };

    this.getCaptureBonus = function(co, unit, posX, posY, map)
    {        
        if (CO.isActive(co))
        {
            if (co.getPowerMode() === GameEnums.PowerMode_Power)
            {
                return CO_VARLOT.powerCaptureBonus;
            }
        }
        return 0;
    };

    this.getIncomeReduction = function(co, building, income, map)
    {
        if (CO.isActive(co))
        {
            var unit = map.getTerrain(building.getX(), building.getY()).getUnit();
            if (unit !== null && unit.getOwner() === co.getOwner())
            {
                return income * unit.getCapturePoints() / 20;
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
                return ["ZCOUNIT_SMUGGLER"];
            }
        }
        return [];
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("Varlot joined the military and rose through the ranks, slowly but surely. Currently is a sub-commander, but is hoping to be promoted really soon.");
    };
    this.getHits = function(co)
    {
        return qsTr("Caviar");
    };
    this.getMiss = function(co)
    {
        return qsTr("Ethics");
    };
    this.getCODescription = function(co)
    {
        return qsTr("When Varlot begins to capture an enemy property, he reduces the funding the enemy receives from it.");
    };
    this.getLongCODescription = function(co, map)
    {
        var values = [0, 0];
        if (map === null ||
            (map !== null && map.getGameRules().getCoGlobalD2D()))
        {
            values = [CO_VARLOT.d2dOffBonus , CO_VARLOT.d2dDefBonus];
        }
        var text = qsTr("\nSpecial Unit:\nSmuggler\n") +
            qsTr("\nGlobal Effect: \nWhile Varlot captures an enemy property, the enemy's income for that property is reduced by how far the capture has completed. Varlot's units have +%0% firepower and +%1% defence.") +
            qsTr("\n\nCO Zone Effect: \nVarlot's units gain +%2% firepower and +%3% defence.");
        text = replaceTextArgs(text, [values[0], values[1], CO_VARLOT.d2dCoZoneOffBonus, CO_VARLOT.d2dCoZoneDefBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Varlot's units gain +%0% firepower, +%1% defence, and a +%2% capture bonus. Varlot gains funds equal to %3% of the total income for all enemies.");
        text = replaceTextArgs(text, [CO_VARLOT.powerOffBonus, CO_VARLOT.powerDefBonus, CO_VARLOT.powerCaptureBonus*10, CO_VARLOT.powerIncome]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Acquisition");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Varlot's units gain +%2% firepower and +%3% defence. Varlot's units repair for +%0 HP, gaining an additional +%1% firepower for each point over 10HP they would have been healed. The enemy pays for the repairs, splitting the cost evenly if there are multiple.");
        text = replaceTextArgs(text, [CO_VARLOT.superPowerHeal, CO_VARLOT.superPowerOverhealOffBonus, CO_VARLOT.powerOffBonus, CO_VARLOT.powerDefBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Hostile Takeover");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Move out!"),
                qsTr("Attack!"),
                qsTr("Forward march!"),
                qsTr("Onward to victory!"),
                qsTr("Never surrender!"),
                qsTr("Push forward!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Mission complete."),
                qsTr("Another day, another battle won."),
                qsTr("Maybe I'll be up for promotion soon...")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("I won't get demoted for this loss!"),
                qsTr("Retreat! Retreat!")];
    };
    this.getName = function()
    {
        return qsTr("Varlot");
    };
}

Constructor.prototype = CO;
var CO_VARLOT = new Constructor();
