var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(4);
        co.setSuperpowerStars(5);
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
                audio.addMusic("resources/music/cos/robosturm.mp3", 1395, 41092);
                break;
            }
        }
    };

    this.activatePower = function(co, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        dialogAnimation.queueAnimation(powerNameAnimation);

        CO_ROBOSTURM.power(co, CO_ROBOSTURM.powerDamage, powerNameAnimation, map);
    };

    this.activateSuperpower = function(co, powerMode, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        powerNameAnimation.queueAnimationBefore(dialogAnimation);

        CO_ROBOSTURM.power(co, CO_ROBOSTURM.superPowerDamage, powerNameAnimation, map);
    };    

    this.power = function(co, value, powerNameAnimation, map)
    {
        var player = co.getOwner();
        var units = player.getUnits();
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
                animation.setSound("power2_1.wav", 1, delay);
            }
            else
            {
                animation.setSound("power2_2.wav", 1, delay);
            }
            if (animations.length < 5)
            {
                animation.addSprite("power2", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power2", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                animations[counter].queueAnimation(animation);
                animations[counter] = animation;
                counter++;
                if (counter >= animations.length)
                {
                    counter = 0;
                }
            }
        }
        var playerCounter = map.getPlayerCount();
        for (var i2 = 0; i2 < playerCounter; i2++)
        {
            var enemyPlayer = map.getPlayer(i2);
            if ((enemyPlayer !== player) &&
                (player.checkAlliance(enemyPlayer) === GameEnums.Alliance_Enemy))
            {

                units = enemyPlayer.getUnits();
                units.randomize();
                for (i = 0; i < units.size(); i++)
                {
                    unit = units.at(i);
                    animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());

                    animation.writeDataInt32(unit.getX());
                    animation.writeDataInt32(unit.getY());
                    animation.writeDataInt32(value);
                    animation.setEndOfAnimationCall("CO_ROBOSTURM", "postAnimationDamage");
                    var delay = globals.randInt(135, 265);
                    if (animations.length < 5)
                    {
                        delay *= i;
                    }
                    animation.setSound("power4.wav", 1, delay);
                    if (animations.length < 5)
                    {
                        animation.addSprite("power4", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                        powerNameAnimation.queueAnimation(animation);
                        animations.push(animation);
                    }
                    else
                    {
                        animation.addSprite("power4", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                        animations[counter].queueAnimation(animation);
                        animations[counter] = animation;
                        counter++;
                        if (counter >= animations.length)
                        {
                            counter = 0;
                        }
                    }
                }
            }
        }
    };

    this.postAnimationDamage = function(postAnimation, map)
    {
        postAnimation.seekBuffer();
        var x = postAnimation.readDataInt32();
        var y = postAnimation.readDataInt32();
        var damage = postAnimation.readDataInt32();
        if (map.onMap(x, y))
        {
            var unit = map.getTerrain(x, y).getUnit();
            if (unit !== null)
            {
                if (damage > 0)
                {
                    var hp = unit.getHpRounded();
                    if (hp - damage <= 0.1)
                    {
                        // set hp to very very low
                        unit.setHp(0.1);
                    }
                    else
                    {
                        unit.setHp(hp - damage);
                    }
                }
                // reduce ammo
                if (unit.getMaxAmmo2() > 0)
                {
                    unit.reduceAmmo2(unit.getAmmo2() * CO_ROBOSTURM.powerAmmoLoose);
                }
                if (unit.getMaxAmmo1() > 0)
                {
                    unit.reduceAmmo1(unit.getAmmo1() * CO_ROBOSTURM.powerAmmoLoose);
                }
            }
        }
    };

    this.superPowerDamage = 2;
    this.superPowerMovementPoints = 3;
    this.superPowerDefBonus = 80;
    this.superPowerOffBonus = 0;

    this.powerDamage = 0;
    this.powerMovementPoints = 2;
    this.powerDefBonus = 60;
    this.powerOffBonus = 0;
    this.powerAmmoLoose = 0.5;

    this.d2dCoZoneOffBonus = -10;
    this.d2dCoZoneDefBonus = 50;

    this.d2dOffBonus = -20;
    this.d2dDefBonus = 20;
    this.d2dMovementPoints = 1;

    this.getCOUnitRange = function(co, map)
    {
        return 2;
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
                return CO_ROBOSTURM.superPowerOffBonus;
            case GameEnums.PowerMode_Power:
                return CO_ROBOSTURM.powerOffBonus;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return CO_ROBOSTURM.d2dCoZoneOffBonus;
                }
                else if (map === null ||
                         (map !== null && map.getGameRules().getCoGlobalD2D()))
                {
                    return CO_ROBOSTURM.d2dOffBonus;
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
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return CO_ROBOSTURM.superPowerDefBonus;
            case GameEnums.PowerMode_Power:
                return CO_ROBOSTURM.powerDefBonus;
            default:
                if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                {
                    return CO_ROBOSTURM.d2dCoZoneDefBonus;
                }
                else if (map === null ||
                         (map !== null && map.getGameRules().getCoGlobalD2D()))
                {
                    return CO_ROBOSTURM.d2dDefBonus;
                }
            }
        }
        return 0;
    };

    this.getMovementpointModifier = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            if (co.getPowerMode() === GameEnums.PowerMode_Power)
            {
                return CO_ROBOSTURM.powerMovementPoints;
            }
            else if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
                     co.getPowerMode() === GameEnums.PowerMode_Tagpower)
            {
                return CO_ROBOSTURM.superPowerMovementPoints;
            }
            else if (map === null ||
                     (map !== null && map.getGameRules().getCoGlobalD2D()))
            {
                return CO_ROBOSTURM.d2dMovementPoints;
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
                return ["ZCOUNIT_TANK_HUNTER"];
            }
        }
        return [];
    };
    // CO - Intel
    this.getCOArmy = function()
    {
        return "MA";
    };

    this.getBio = function(co)
    {
        return qsTr("After Sturm's second defeat, Black Hole was in need of new, strong CO's, so Lash invented a second, mechanical Sturm. That quickly proved to be a bad idea after he refused to take orders from humans and formed his own army.");
    };
    this.getHits = function(co)
    {
        return qsTr("Robots");
    };
    this.getMiss = function(co)
    {
        return qsTr("Humans");
    };
    this.getCODescription = function(co)
    {
        var text = qsTr("His troops gain +%0 movement and higher defense capabilities, but have lower firepower.");
        text = replaceTextArgs(text, [CO_ROBOSTURM.d2dMovementPoints]);
        return text;
    };
    this.getLongCODescription = function(co, map)
    {
        var values = [0, 0, 0];
        if (map === null ||
            (map !== null && map.getGameRules().getCoGlobalD2D()))
        {
            values = [CO_ROBOSTURM.d2dMovementPoints, CO_ROBOSTURM.d2dOffBonus, CO_ROBOSTURM.d2dDefBonus];
        }
        var text = qsTr("\nSpecial Unit:\nTank Hunter\n") +
               qsTr("\nGlobal Effect: \nRobo-Sturm's units gain +%0 movement, +%2% defence, and have %1% firepower.") +
               qsTr("\n\nCO Zone Effect: \nRobo-Sturm's units gain +%3% defense and have %4% firepower.");
        text = replaceTextArgs(text, [values[0], values[1], values[2],
                                      CO_ROBOSTURM.d2dCoZoneDefBonus, CO_ROBOSTURM.d2dCoZoneOffBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("A wave of mechanical energy deals -%1 HP of damage to all enemy units and causes them to lose -%0% of their maximum ammo. Robo-Sturm's units gain +%2 movement, +%3% defence, and have -%4% firepower.");
        text = replaceTextArgs(text, [CO_ROBOSTURM.powerAmmoLoose * 100, CO_ROBOSTURM.powerDamage, CO_ROBOSTURM.powerMovementPoints,
                                      CO_ROBOSTURM.powerDefBonus, CO_ROBOSTURM.powerOffBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Mechanized Storm");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("A massive storm of electric energy deals -%1 HP of damage to all enemy units and causes them to lose -%0% of their maximum ammo. Robo-Sturm's units gain +%2 movement, +%3% defence, and have -%4% firepower.");
        text = replaceTextArgs(text, [CO_ROBOSTURM.powerAmmoLoose * 100, CO_ROBOSTURM.superPowerDamage, CO_ROBOSTURM.superPowerMovementPoints,
                                      CO_ROBOSTURM.superPowerDefBonus, CO_ROBOSTURM.powerOffBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Mechanized Destruction");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("System Runtime. Error! No more enemies found!"),
                qsTr("Sturm is coming. You have no chance."),
                qsTr("Order analysed! Destroy enemies."),
                qsTr("No human can kill a robot."),
                qsTr("This is a war of a new time!"),
				qsTr("Machines will take over this country!"),
                qsTr("The age of man is over.")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Victory obtained. Analysing combat logs..."),
                qsTr("Enemy destroyed! Country conquered."),
                qsTr("Robots are stronger than human beings.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("Error! No program found for this situation!"),
                qsTr("Inaccurate attack! This situation is impossible!")];
    };
    this.getName = function()
    {
        return qsTr("Robo-Sturm");
    };
}

Constructor.prototype = CO;
var CO_ROBOSTURM = new Constructor();
