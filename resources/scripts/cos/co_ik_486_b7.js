var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(4);
    };

    this.getAiUsePower = function(co, powerSurplus, unitCount, repairUnits, indirectUnits, directUnits, enemyUnits, turnMode)
    {
        return CO.getAiUsePowerAtStart(co, powerSurplus, turnMode);
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
            if (unit.getBaseMaxRange() === 1)
            {
                var animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
                var delay = globals.randInt(135, 265);
                if (animations.length < 5)
                {
                    delay *= i;
                }
                if (i % 2 === 0)
                {
                    animation.setSound("power9_1.wav", 1, delay);
                }
                else
                {
                    animation.setSound("power9_2.wav", 1, delay);
                }
                if (animations.length < 5)
                {
                    animation.addSprite("power9", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                    powerNameAnimation.queueAnimation(animation);
                    animations.push(animation);
                }
                else
                {
                    animation.addSprite("power9", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
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
            if (unit.getBaseMaxRange() === 1)
            {
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
                audio.addMusic("resources/music/cos/ik-486-b7.mp3", 8310, 95764);
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

    this.superPowerFirerangeBonus = 3;
    this.superPowerOffMalus = 0;
    this.superPowerDirectDefBonus = 50;

    this.powerOffMalus = 0;
    this.powerFirerangeBonus = 2;
    this.powerOffBonus = 10;
    this.powerDefBonus = 10;
    this.powerDirectDefBonus = 30;

    this.d2dFirerangeBonus = 0;
    this.d2dOffMalus = 0;

    this.d2dCoZoneFirerangeBonus = 1;
    this.d2dCoZoneOffMalus = -10;
    this.d2dCoZoneOffBonus = 10;
    this.d2dCoZoneDefBonus = 10;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            var indirectAttack = (attacker.getBaseMaxRange() === 1 &&
                                  atkPosX === attacker.getX() &&
                                  atkPosY === attacker.getY() &&
                                  Math.abs(atkPosX - defPosX) + Math.abs(atkPosY - defPosY) > 1);
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (indirectAttack)
                {
                    return CO_IK_486_B7.superPowerOffMalus;
                }
                else
                {
                    return CO_IK_486_B7.powerOffBonus;
                }
            case GameEnums.PowerMode_Power:
                if (indirectAttack)
                {
                    return CO_IK_486_B7.powerOffMalus;
                }
                else
                {
                    return CO_IK_486_B7.powerOffBonus;
                }
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    if (indirectAttack)
                    {
                        return CO_IK_486_B7.d2dCoZoneOffMalus;
                    }
                    return CO_IK_486_B7.d2dCoZoneOffBonus;
                }
                else
                {
                    if (indirectAttack)
                    {
                        return CO_IK_486_B7.d2dOffMalus;
                    }
                }
            }
        }
        return 0;
    };

    this.getFirerangeModifier = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            if (unit.getBaseMaxRange() === 1 &&
                posX === unit.getX() &&
                posY === unit.getY())
            {
                switch (co.getPowerMode())
                {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                    return CO_IK_486_B7.superPowerFirerangeBonus;
                case GameEnums.PowerMode_Power:
                    return CO_IK_486_B7.powerFirerangeBonus;
                default:
                    if (co.inCORange(Qt.point(posX, posY), unit))
                    {
                        return CO_IK_486_B7.d2dCoZoneFirerangeBonus;
                    }
                    return CO_IK_486_B7.d2dFirerangeBonus;
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
                if (defender.getBaseMaxRange() === 1)
                {
                    return CO_IK_486_B7.superPowerDirectDefBonus;
                }
                return CO_IK_486_B7.powerDefBonus;
            case GameEnums.PowerMode_Power:
                if (defender.getBaseMaxRange() === 1)
                {
                    return CO_IK_486_B7.powerDirectDefBonus;
                }
                return CO_IK_486_B7.powerDefBonus;
            default:
                if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                {
                    return CO_IK_486_B7.d2dCoZoneDefBonus;
                }
                break;
            }
        }
        return 0;
    };

    this.getAiCoUnitBonus = function(co, unit, map)
    {
        if (unit.getBaseMaxRange() === 1)
        {
            return 2;
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
                BUILDING.isHq(building))
            {
                return ["ZCOUNIT_CHAPERON"];
            }
        }
        return [];
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("One of Lash latest invented robots. He likes to stay at the same place and defend it against anything that comes to him.");
    };
    this.getHits = function(co)
    {
        return qsTr("Fortresses");
    };
    this.getMiss = function(co)
    {
        return qsTr("Conquering");
    };
    this.getCODescription = function(co)
    {
        return qsTr("His direct units can do indirect attacks when they don't move. However they lose a fraction of firepower when doing so.");
    };
    this.getLongCODescription = function()
    {
        var text = qsTr("\nSpecial Unit:\nChaperon\n") +
               qsTr("\nGlobal Effect: \nDirect Units gain an additional firerange of %0 when they don't move. However firepower is reduced by %1% in doing so.") +
               qsTr("\n\nCO Zone Effect: \nDirect Units gain an additional firerange of %2 when they don't move. However firepower is reduced by %3% in doing so.");
        text = replaceTextArgs(text, [CO_IK_486_B7.d2dFirerangeBonus, CO_IK_486_B7.d2dOffMalus,
                                      CO_IK_486_B7.d2dCoZoneFirerangeBonus, CO_IK_486_B7.d2dCoZoneOffMalus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("The defense of his direct units raises by %0 and their firerange is increased by %1% when they don't move and firepower is reduced by %2%");
        text = replaceTextArgs(text, [CO_IK_486_B7.powerDirectDefBonus, CO_IK_486_B7.powerFirerangeBonus, CO_IK_486_B7.powerOffMalus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Wall of Defense");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("The defense of his direct units raises by %0 and their firerange is increased by %1% when they don't move and firepower is reduced by %2%");
        text = replaceTextArgs(text, [CO_IK_486_B7.superPowerDirectDefBonus, CO_IK_486_B7.superPowerFirerangeBonus, CO_IK_486_B7.superPowerOffMalus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Great Counterstrike");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Direct attack!"),
                qsTr("Function attacking not found use function defending instead!"),
                qsTr("My fortress is unbeatable."),
                qsTr("Make ready to defend yourself!"),
                qsTr("You are too near to my defence position."),
                qsTr("Go to the shortage other their.")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Fools don't attack my fortress."),
                qsTr("Robots rule the world."),
                qsTr("Do function 'Be Happy'")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("You have destroyed my fortress?"),
                qsTr("Tilt! Tilt! Selfdestruction...")];
    };
    this.getName = function()
    {
        return qsTr("IK-486-B7");
    };
}
Constructor.prototype = CO;
var CO_IK_486_B7 = new Constructor();
