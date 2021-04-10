var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
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

        var animation = GameAnimationFactory.createAnimation(0, 0);
        animation.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);
        powerNameAnimation.queueAnimation(animation);
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

    this.loadCOMusic = function(co)
    {
        // put the co music in here.
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
                audio.addMusic("resources/music/cos/mary.mp3", 8299, 73941);
                break;
        }
    };

    this.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked)
    {
        var destroyed = false;
        var x = -1;
        var y = -1;
        var unitID = 0;
        if (gotAttacked && defender.getOwner() === co.getOwner())
        {
            if (attacker.getHp() <= 0)
            {
                destroyed = true;
                x = attacker.getX();
                y = attacker.getY();
            }
            else if (attacker.getOwner() === co.getOwner())
            {
                unitID = attacker.getUniqueID();
            }
        }
        else
        {
            if (defender.getHp() <= 0)
            {
                destroyed = true;
                x = defender.getX();
                y = defender.getY();
            }
            else
            {
                unitID = defender.getUniqueID();
            }
        }
        // store variable data

        var variables = co.getVariables();
        if (destroyed)
        {
            var building = map.getTerrain(x, y).getBuilding();
            if (building !== null)
            {
                // store capture bonus
                var xVariable = variables.createVariable("POSXBUILDINGS");
                var yVariable = variables.createVariable("POSYBUILDINGS");
                var pointsX = xVariable.readDataListInt32();
                var pointsY = yVariable.readDataListInt32();
                var found = false;
                for (var i = 0; i < pointsX.length; i++)
                {
                    if (pointsX[i] === x &&
                        pointsY[i] === y)
                    {
                        found = true;
                        break;
                    }
                }
                if (found === false)
                {
                    pointsX.push(x);
                    pointsY.push(y);
                    xVariable.writeDataListInt32(pointsX);
                    yVariable.writeDataListInt32(pointsY);
                }
            }
        }
        else
        {
            // store repair block
            var repairVariable = variables.createVariable("UNITREPAIR");
            var repairs = repairVariable.readDataListInt32();
            repairs.push(unitID);
            repairVariable.writeDataListInt32(repairs);
        }
    };

    this.startOfTurn = function(co)
    {
        // reset repair stop
        var variables = co.getVariables();
        var repairVariable = variables.createVariable("UNITREPAIR");
        var empty = [];
        repairVariable.writeDataListInt32(empty);
    };

    this.canBeRepaired = function(co, unit, posX, posY)
    {
        // disanle enemy repairs
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Power:
                return false;
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
            default:
                var variables = co.getVariables();
                var repairVariable = variables.createVariable("UNITREPAIR");
                var repairs = repairVariable.readDataListInt32();
                var unitId = unit.getUniqueID();
                if (repairs.indexOf(unitId) >= 0)
                {
                    return false;
                }
        }
        return true;
    };

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        // get cop and scop offensive bonus
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return 30;
            case GameEnums.PowerMode_Power:
                return 20;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return 20;
                }
                return 0;
        }
    };
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender)
    {
        if (co.inCORange(Qt.point(defPosX, defPosY), defender) ||
                co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 10;
        }
        return 0;
    };
    this.getDeffensiveReduction = function(co, attacker, atkPosX, atkPosY,
                                  defender, defPosX, defPosY, isDefender)
    {
        // reduce enemy defense
        if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
            co.getPowerMode() === GameEnums.PowerMode_Tagpower)
        {
            if (co.getOwner().isEnemyUnit(defender) &&
                    defender.useTerrainDefense())
            {
                return defender.getTerrainDefense() * 15;
            }
        }
        return 0;
    };

    this.getAdditionalBuildingActions = function(co, building)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return ""
            case GameEnums.PowerMode_Power:
                // disable enemy production line
                if (co.getOwner().isEnemy(building.getOwner()))
                {
                    return "-ACTION_BUILD_UNITS";
                }
                break;
            default:
                break;
        }
        return "";
    };

    this.getCaptureBonus = function(co, unit, posX, posY)
    {
        var applyBonus = false;
        // store capture bonus
        var variables = co.getVariables();
        var xVariable = variables.createVariable("POSXBUILDINGS");
        var yVariable = variables.createVariable("POSYBUILDINGS");
        var pointsX = xVariable.readDataListInt32();
        var pointsY = yVariable.readDataListInt32();
        for (var i = 0; i < pointsX.length; i++)
        {
            if (pointsX[i] === posX &&
                pointsX[i] === posX)
            {
                // apply capture bonus and remove it from the list
                applyBonus = true;
                pointsX.splice(i, 1);
                pointsY.splice(i, 1);
                xVariable.writeDataListInt32(pointsX);
                yVariable.writeDataListInt32(pointsY);
                break;
            }
        }
        if ((co.getPowerMode() === GameEnums.PowerMode_Superpower ||
             co.getPowerMode() === GameEnums.PowerMode_Tagpower) &&
            applyBonus === true)
        {
            return 15;
        }
        else if (applyBonus === true)
        {
            return 5;
        }
        return 0;
    };
    this.getAiCoUnitBonus = function(co, unit)
    {
        return 1;
    };
    this.getCOUnitRange = function(co)
    {
        return 2;
    };
    this.getCOArmy = function()
    {
        return "AC";
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A ruthless commander recruited into the Amber Corona army.");
    };
    this.getHits = function(co)
    {
        return qsTr("Cocktails");
    };
    this.getMiss = function(co)
    {
        return qsTr("Excuses");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Attacked units won't repair from buildings for one turn. Whenever Mary destroys a unit on a property, she gains a capture bonus for that property.");
    };
    this.getLongCODescription = function()
    {
        return qsTr("\nGlobal Effect: \nAttacked units won't repair from buildings for one turn. Whenever Mary destroys a unit on a property, she gains a capture bonus for that property.") +
               qsTr("\n\nCO Zone Effect: \nUnits gain firepower and defense.");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("Deployment and repairs from properties are disabled and she gets an offensive bonus.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Highway of Pain");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("Terrain stars now reduces the foe's defense and Mary's capture bonus is greatly increased. All units gain substantial firepower.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Bloody Mary");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Keep marching guys, the victory is close!"),
                qsTr("No pain! NO GLORY!"),
                qsTr("A drop of blood is a drop of effort...you must be proud of your troops."),
                qsTr("People call me Bloody Mary....and no, you can't drink me."),
                qsTr("We will fight until your last soldier is dead!"),
                qsTr("I am a soldier, I fight where I am told, and I win where I fight.")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Heh, looks like all my effort paid off in the end"),
                qsTr("Let's go, people. Let's have a drink to celebrate this victory."),
                qsTr("All right troops, now you can rest, your job here is done.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("You were better.... This time"),
                qsTr("That is impossible. I need a drink now.")];
    };
    this.getName = function()
    {
        return qsTr("Mary");
    };
}

Constructor.prototype = CO;
var CO_MARY = new Constructor();
