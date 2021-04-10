var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(4);
        co.setSuperpowerStars(4);
    };

    this.getCOStyles = function()
    {
        // string array containing the endings of the alternate co style
        // e.g. co_adder has ["+alt"]
        return ["+alt"];
    };

    this.activatePower = function(co)
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
        player.addFunds(income / 4);
    };

    this.activateSuperpower = function(co, powerMode)
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
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);

            // apply cost and offense boni
            var healing = unit.getHpRounded() + 4;
            if (healing > 10)
            {
                costs += unit.getUnitCosts() * (10 - unit.getHpRounded()) / 10;
                var unitId = unit.getUniqueID();
                bonusUnits.push(unitId);
                bonusOffs.push((healing - 10) * 10);
            }
            else
            {
                costs += unit.getUnitCosts() * 4 / 10;
            }

            var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
            animation.writeDataInt32(unit.getX());
            animation.writeDataInt32(unit.getY());
            animation.writeDataInt32(4);
            animation.setEndOfAnimationCall("ANIMATION", "postAnimationHeal");
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

        bonusUnitVariable.writeDataListInt32(bonusUnits);
        bonusOffVariable.writeDataListInt32(bonusOffs);

        // reduce enemy funds
        var player = co.getOwner();
        var enemyCount = player.getEnemyCount()
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
                audio.addMusic("resources/music/cos/varlot.mp3", 2418, 74440);
                break;
        }
    };

    this.getCOUnitRange = function(co)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "DM";
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
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
                    return 10 + bonusOffs[index];
                }
                return 10;
            case GameEnums.PowerMode_Power:
                return 10;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return 20;
                }
                break;
        }
        return 0;
    };
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        if (co.inCORange(Qt.point(defPosX, defPosY), defender) ||
            co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 20;
        }
        return 0;
    };

    this.startOfTurn = function(co)
    {
        // reset repair stop
        var variables = co.getVariables();
        var bonusUnitVariable = variables.createVariable("BONUSUNITS");
        var empty = [];
        bonusUnitVariable.writeDataListInt32(empty);
        var bonusOffVariable = variables.createVariable("BONUSOFF");
        bonusOffVariable.writeDataListInt32(empty);
    };

    this.getCaptureBonus = function(co, unit, posX, posY)
    {
        if (co.getPowerMode() === GameEnums.PowerMode_Power)
        {
            return 5;
        }
        return 0;
    };

    this.getIncomeReduction = function(co, building, income)
    {
        var unit = map.getTerrain(building.getX(), building.getY()).getUnit();
        // set income to 0 during scop
        if (unit !== null && unit.getOwner() === co.getOwner())
        {
            return income * unit.getCapturePoints() / 20;
        }
        return 0;
    };
    this.getAiCoUnitBonus = function(co, unit)
    {
        return 1;
    };
    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("This CO joined the military and rose through the ranks, not quickly enough. Currently is a sub-commander.");
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
        return qsTr("Varlot reduces the income from a property he captures by an amount proportional to its Capture Points.");
    };
    this.getLongCODescription = function()
    {
        return qsTr("\nGlobal Effect: \nIncome is reduced for buildings he captures.") +
               qsTr("\n\nCO Zone Effect: \nUnits have increased firepower and defense.");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("His troops get a capture bonus. Varlot also gains a fraction of the enemy income.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Aquisition");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("All units are healed for 4 HP, gaining firepower for each point over 10HP they'd be healed. The enemy pays for these repairs.");
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
        return [qsTr("I won't get demoted for this lost!"),
                qsTr("Retreat!!!")];
    };
    this.getName = function()
    {
        return qsTr("Varlot");
    };
}

Constructor.prototype = CO;
var CO_VARLOT = new Constructor();
