var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
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
            animation.setSound("power3.wav", 1, delay);
            if (animations.length < 5)
            {
                animation.addSprite("power3", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power3", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
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
        units.remove();
    };

    this.loadCOMusic = function(co, map)
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

    this.getCOUnitRange = function(co, map)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "MA";
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
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
                                       defender, defPosX, defPosY, isAttacker, action, luckmode, map)
    {
        if (co.inCORange(Qt.point(defPosX, defPosY), defender) ||
                co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 10;
        }
        return 0;
    };
    this.getBonusMisfortune = function(co, unit, posX, posY, map)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
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


    this.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked, weapon, action, map)
    {
        var healing = 0;
        var damage = 0;
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                healing = 3;
                damage = 3;
                break;
            case GameEnums.PowerMode_Power:
                healing = 1;
                damage = 1;
                break;
            default:
                break;
        }
        if (healing > 0 || damage > 0)
        {
            if (attacker.getOwner() === co.getOwner() && attacker.getHp() > 0)
            {
                attacker.setHp(attacker.getHp() + healing);
            }
            else if (!gotAttacked && attacker.getOwner() === co.getOwner() && attacker.getHp() <= 0)
            {
                var hp = defender.getHp();
                if (hp > damage)
                {
                    defender.setHp(hp - damage);
                }
                else
                {
                    defender.setHp(0.00001);
                }
            }
            else if (gotAttacked && defender.getOwner() === co.getOwner() && defender.getHp() <= 0)
            {
                var hp = attacker.getHp();
                if (hp > damage)
                {
                    attacker.setHp(hp - damage);
                }
                else
                {
                    attacker.setHp(0.00001);
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
        var buildingId = building.getBuildingID();
        if (buildingId === "FACTORY" ||
            buildingId === "TOWN" ||
            buildingId === "HQ")
        {
            return ["ZCOUNIT_AT_CYCLE"];
        }
        return [];
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
        return qsTr("\nSpecial Unit:\nAT Cycle\n") +
               qsTr("\nGlobal Effect: \nUnits gain firepower and misfortune per star.") +
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
