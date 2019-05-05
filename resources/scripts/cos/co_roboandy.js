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
        var powerNameAnimation = co.createPowerScreen(false);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var units = co.getPlayer().getUnits();
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

        audio.clearPlayList();
        CO_ROBOANDY.loadCOMusic(co);
        audio.playRandom();
    };

    this.activateSuperpower = function(co)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(true);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var units = co.getPlayer().getUnits();
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

        audio.clearPlayList();
        CO_ROBOANDY.loadCOMusic(co);
        audio.playRandom();
    };

    this.loadCOMusic = function(co)
    {
        // put the co music in here.
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/bh_power.mp3");
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/bh_superpower.mp3");
                break;
            default:
                audio.addMusic("resources/music/cos/roboandy.mp3")
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
            case GameEnums.PowerMode_Superpower:
                return 10;
            case GameEnums.PowerMode_Power:
                return 10;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return co.getPowerFilled() * 4;
                }
                break;
        }
        return co.getPowerFilled() * 2;
    };

    this.getBonusMisfortune = function(co, unit, posX, posY)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                return 0;
            case GameEnums.PowerMode_Power:
                return 0;
            default:
                if (co.inCORange(Qt.point(posX, posY), unit))
                {
                    return co.getPowerFilled() * 2;
                }
                break;
        }
        return co.getPowerFilled();
    };


    this.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                if (gotAttacked)
                {
                    if (defender.getHp() <= 0)
                    {
                        attacker.setHp(attacker.getHp() - 3);
                    }
                }
                else
                {
                    attacker.setHp(attacker.getHp() + 3);
                }
                break;
            case GameEnums.PowerMode_Power:
                if (gotAttacked)
                {
                    if (defender.getHp() <= 0)
                    {
                        attacker.setHp(attacker.getHp() - 1);
                    }
                }
                else
                {
                    attacker.setHp(attacker.getHp() + 1);
                }
                break;
            default:
                break;
        }
    };

    // CO - Intel
    this.getBio = function()
    {
        return qsTr("Lash was asked by the military to create artificial intelligence able to direct troops. However, being given orders by a computer freaked out the soldiers. So she fixed the problem by putting it in the original Robo-Andy.");
    };
    this.getHits = function()
    {
        return qsTr("Puppet Shows");
    };
    this.getMiss = function()
    {
        return qsTr("Housework");
    };
    this.getCODescription = function()
    {
        return qsTr("He gets firepower for his powermeter, but his firepower may also drop drastically.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("His units heal 1 HP after any attack or counter-attack, and deal 1 HP damage to the offender if they are destroyed.");
    };
    this.getPowerName = function()
    {
        return qsTr("Cooldown");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("His units heal 3 HP after any attack or counter-attack, and deal 3 HP damage to the offender if they are destroyed.");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("Critical Mass");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("DADADA! ANNOYING PESTS DESERVE TO BE SWATTED!"),
                qsTr("ERROR DETECTED. PREPARE TO BE ELIMINATED!"),
                qsTr("YOU'RE A RUDE ONE! EAT HEAVY METAL!"),
                qsTr("KYAAA! SUCH A JERK! TIME TO DIE!"),
                qsTr("WARNING! WARNING! EXECUTING EMERGENCY VENTING MODULE."),
                qsTr("YOU'RE SO DISAPPOINTING. WITNESS MY POWER!")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("USELESS CREATURE!"),
                qsTr("THANK YOU FOR USING ROBO-ANDY, INFERIOR BEING!"),
                qsTr("FOR MORE EXCITING PERFORMANCES PLEASE UPGRADE TO THE LATEST VERSION.")];
    };
    this.getDefeatSentences = function()
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
