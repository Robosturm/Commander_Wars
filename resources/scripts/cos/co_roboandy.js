var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
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
            var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
            if (animations.length < 5)
            {
                animation.addSprite("power3", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power3", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
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
                audio.addMusic("resources/music/cos/bh_power.mp3", 1091 , 49930);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/bh_superpower.mp3", 3161 , 37731);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/bh_tagpower.mp3", 779 , 51141);
                break;
            default:
                audio.addMusic("resources/music/cos/roboandy.mp3", 3653, 46346);
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
                return 10;
            case GameEnums.PowerMode_Power:
                return 10;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return co.getPowerFilled() * 5 + 10;
                }
                break;
        }
        return co.getPowerFilled() * 3;
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
    this.getBonusMisfortune = function(co, unit, posX, posY)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return 0;
            case GameEnums.PowerMode_Power:
                return 0;
            default:
                if (co.inCORange(Qt.point(posX, posY), unit))
                {
                    return co.getPowerFilled() * 3;
                }
        }
        return co.getPowerFilled();
    };


    this.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (gotAttacked && defender.getOwner() === co.getOwner())
                {
                    if (defender.getHp() <= 0)
                    {
                        attacker.setHp(attacker.getHp() - 3);
                    }
                }
                else if (attacker.getOwner() === co.getOwner() && attacker.getHp() > 0)
                {
                    attacker.setHp(attacker.getHp() + 3);
                }
                break;
            case GameEnums.PowerMode_Power:
                if (gotAttacked && defender.getOwner() === co.getOwner())
                {
                    if (defender.getHp() <= 0)
                    {
                        attacker.setHp(attacker.getHp() - 1);
                    }
                }
                else if (attacker.getOwner() === co.getOwner() && attacker.getHp() > 0)
                {
                    attacker.setHp(attacker.getHp() + 1);
                }
                break;
            default:
                break;
        }
    };
    this.getAiCoUnitBonus = function(co, unit)
    {
        return 1;
    };
    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("Lash was asked by the military to create artificial intelligence able to direct troops. However, being given orders by a computer freaked out the soldiers. So she fixed the problem by putting it in the original Robo-Andy.");
    };
    this.getHits = function(co)
    {
        return qsTr("Puppet Shows");
    };
    this.getMiss = function(co)
    {
        return qsTr("Housework");
    };
    this.getCODescription = function(co)
    {
        return qsTr("He gets firepower for his powermeter, but his firepower may also drop drastically.");
    };
    this.getLongCODescription = function()
    {
        return qsTr("\nGlobal Effect: \nUnits gain firepower and misfortune per star.") +
               qsTr("\n\nCO Zone Effect: \nUnits gain even more firepower and misfortune per star.");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("His units heal 1 HP after any attack or counter-attack, and deal 1 HP damage to the offender if they are destroyed.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Cooldown");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("His units heal 3 HP after any attack or counter-attack, and deal 3 HP damage to the offender if they are destroyed.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Critical Mass");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("DADADA! ANNOYING PESTS DESERVE TO BE SWATTED!"),
                qsTr("ERROR DETECTED. PREPARE TO BE ELIMINATED!"),
                qsTr("YOU'RE A RUDE ONE! EAT HEAVY METAL!"),
                qsTr("KYAAA! SUCH A JERK! TIME TO DIE!"),
                qsTr("WARNING! WARNING! EXECUTING EMERGENCY VENTING MODULE."),
                qsTr("YOU'RE SO DISAPPOINTING. WITNESS MY POWER!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("USELESS CREATURE!"),
                qsTr("THANK YOU FOR USING ROBO-ANDY, INFERIOR BEING!"),
                qsTr("FOR MORE EXCITING PERFORMANCES PLEASE UPGRADE TO THE LATEST VERSION.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("HOW MAY I SERVE YOU, INFERIOR BEING?"),
                qsTr("ALT+TAB! ALT+TAB! ALT+TAB!")];
    };
    this.getName = function()
    {
        return qsTr("Robo-Andy");
    };
}

Constructor.prototype = CO;
var CO_ROBOANDY = new Constructor();
