var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(4);
    };

    this.getAiUsePower = function(co, powerSurplus, unitCount, repairUnits, indirectUnits, directUnits, enemyUnits, turnMode)
    {
        return CO.getAiUsePowerAtStart(co, powerSurplus, turnMode);
    };

    this.activatePower = function(co)
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
                var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
                if (globals.randInt(0, 1) === 0)
                {
                    animation.setSound("power9_1.wav");
                }
                else
                {
                    animation.setSound("power9_2.wav");
                }
                if (animations.length < 5)
                {
                    animation.addSprite("power9", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                    powerNameAnimation.queueAnimation(animation);
                    animations.push(animation);
                }
                else
                {
                    animation.addSprite("power9", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
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
        units.remove();
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
            if (unit.getBaseMaxRange() === 1)
            {
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
        }
        units.remove();
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
            audio.addMusic("resources/music/cos/ik-486-b7.mp3", 8310, 95764);
            break;
        }
    };

    this.getCOUnitRange = function(co)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "MA";
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (attacker.getBaseMaxRange() === 1 &&
                    atkPosX === attacker.getX() &&
                    atkPosY === attacker.getY() &&
                    Math.abs(atkPosX - defPosX) + Math.abs(atkPosY - defPosY) > 1)
            {
                return 0;
            }
            else
            {
                return 10;
            }
        case GameEnums.PowerMode_Power:
            if (attacker.getBaseMaxRange() === 1 &&
                    atkPosX === attacker.getX() &&
                    atkPosY === attacker.getY() &&
                    Math.abs(atkPosX - defPosX) + Math.abs(atkPosY - defPosY) > 1)
            {
                return 0;
            }
            else
            {
                return 10;
            }
        default:
            if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                if (attacker.getBaseMaxRange() === 1 &&
                        atkPosX === attacker.getX() &&
                        atkPosY === attacker.getY() &&
                        Math.abs(atkPosX - defPosX) + Math.abs(atkPosY - defPosY) > 1)
                {
                    return -10;
                }
                return 10;
            }
            break;
        }
        return 0;
    };

    this.getFirerangeModifier = function(co, unit, posX, posY)
    {
        if (unit.getBaseMaxRange() === 1 &&
                posX === unit.getX() &&
                posY === unit.getY())
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return 3;
            case GameEnums.PowerMode_Power:
                return 2;
            default:
                if (co.inCORange(Qt.point(posX, posY), unit))
                {
                    return 1;
                }
            }
        }
        return 0;
    };

    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (defender.getBaseMaxRange() === 1)
            {
                return 50;
            }
            break;
        case GameEnums.PowerMode_Power:
            if (defender.getBaseMaxRange() === 1)
            {
                return 30;
            }
            break;
        default:
            if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return 10;
            }
            break;
        }
        return 0;
    };

    this.getAiCoUnitBonus = function(co, unit)
    {
        if (unit.getBaseMaxRange() === 1)
        {
            return 2;
        }
        return 0;
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
        return qsTr("\nGlobal Effect: \nNo Effects.") +
                qsTr("\n\nCO Zone Effect: \nDirect Units gain an additional firerange when they don't move. However firepower is reduced by doing so. Normal attacks are stronger.");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("The defense of his direct units raises and the firerange is increased by 1 when they don't move.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Wall of Defense");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("The defense of his direct units raises extremly and the firerange is increased by 2 when they don't move.");
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
                qsTr("You are to near to my defence position."),
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
