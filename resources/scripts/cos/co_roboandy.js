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
                audio.addMusic("resources/music/cos/roboandy.mp3", 3653, 46346);
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

    this.superPowerDamage = 3;
    this.superPowerHeal = 3;
    this.powerDamage = 1;
    this.powerHeal = 1;
    this.powerOffBonus = 10;
    this.powerDefBonus = 10;
    this.d2dOffMissfortuneBonus = 1;
    this.d2dOffBonus = 3;
    this.d2dCoZoneMissfortuneBonus = 3;
    this.d2dCoZoneOffBonus = 5;
    this.d2dCoZoneOffBaseBonus = 10;
    this.d2dCoZoneDefBonus = 10;

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
            case GameEnums.PowerMode_Power:
                return CO_ROBOANDY.powerOffBonus;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return co.getPowerFilled() * CO_ROBOANDY.d2dCoZoneOffBonus + CO_ROBOANDY.d2dCoZoneOffBaseBonus;
                }
                break;
            }
            return co.getPowerFilled() * CO_ROBOANDY.d2dOffBonus;
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
                return CO_ROBOANDY.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_ROBOANDY.d2dCoZoneDefBonus;
            }
        }
        return 0;
    };
    this.getBonusMisfortune = function(co, unit, posX, posY, map)
    {
        if (CO.isActive(co))
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
            case GameEnums.PowerMode_Power:
                return 0;
            default:
                if (CO.getGlobalZone())
                {
                    return co.getPowerFilled() * CO_ROBOANDY.d2dOffMissfortuneBonus;
                }
                else if (co.inCORange(Qt.point(posX, posY), unit))
                {
                    return co.getPowerFilled() * CO_ROBOANDY.d2dCoZoneMissfortuneBonus;
                }
            }
        }
        return 0;
    };

    this.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked, weapon, action, map)
    {
        if (CO.isActive(co))
        {
            let healing = 0;
            let damage = 0;
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                healing = CO_ROBOANDY.superPowerDamage;
                damage = CO_ROBOANDY.superPowerHeal;
                break;
            case GameEnums.PowerMode_Power:
                healing = CO_ROBOANDY.powerHeal;
                damage = CO_ROBOANDY.powerDamage;
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
                    let hp = defender.getHp();
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
                    let hp = attacker.getHp();
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
                    buildingId === "HQ" ||
                    buildingId === "FORTHQ")
            {
                return ["ZCOUNIT_AT_CYCLE"];
            }
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
        let text = qsTr("\nSpecial Unit:\nAT Cycle\n") +
               qsTr("\nGlobal Effect: \nUnits gain %0% firepower and %1% misfortune per star.") +
               qsTr("\n\nCO Zone Effect: \nUnits gain %2% firepower and %3% misfortune per star.");
        text = replaceTextArgs(text, [CO_ROBOANDY.d2dOffBonus, CO_ROBOANDY.d2dOffMissfortuneBonus, CO_ROBOANDY.d2dCoZoneOffBonus, CO_ROBOANDY.d2dCoZoneMissfortuneBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        let text = qsTr("His units heal %0 HP after any attack or counter-attack, and deal %1 HP damage to the offender if they are destroyed.");
        text = replaceTextArgs(text, [CO_ROBOANDY.powerHeal, CO_ROBOANDY.powerDamage]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Cooldown");
    };
    this.getSuperPowerDescription = function(co)
    {
        let text = qsTr("His units heal %0 HP after any attack or counter-attack, and deal %1 HP damage to the offender if they are destroyed.");
        text = replaceTextArgs(text, [CO_ROBOANDY.superPowerHeal, CO_ROBOANDY.superPowerDamage]);
        return text;
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
