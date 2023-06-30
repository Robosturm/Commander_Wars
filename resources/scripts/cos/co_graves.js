var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(4);
        co.setSuperpowerStars(3);
    };

    this.getAiUsePower = function(co, powerSurplus, unitCount, repairUnits, indirectUnits, directUnits, enemyUnits, turnMode)
    {
        return CO.getAiUsePowerAlways(co, powerSurplus);
    };

    this.getCOStyles = function()
    {
        return ["+alt"];
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
                audio.addMusic("resources/music/cos/graves.mp3")
                break;
            }
        }
    };

    this.activatePower = function(co, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        dialogAnimation.queueAnimation(powerNameAnimation);

        CO_GRAVES.gravesDamage(co, CO_GRAVES.powerDamage, CO_GRAVES.powerStunLevel, powerNameAnimation, map);
    };

    this.activateSuperpower = function(co, powerMode, map)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        powerNameAnimation.queueAnimationBefore(dialogAnimation);

        CO_GRAVES.gravesDamage(co, CO_GRAVES.superPowerDamage, CO_GRAVES.superPowerStunLevel, powerNameAnimation, map);
    };

    this.gravesDamage = function(co, value, stunLevel, animation2, map)
    {
        var player = co.getOwner();
        var counter = 0;
        var playerCounter = map.getPlayerCount();
        var animation = null;
        var animations = [];

        for (var i2 = 0; i2 < playerCounter; i2++)
        {
            var enemyPlayer = map.getPlayer(i2);
            if ((enemyPlayer !== player) &&
                (player.checkAlliance(enemyPlayer) === GameEnums.Alliance_Enemy))
            {

                var units = enemyPlayer.getUnits();
                units.randomize();
                for (i = 0; i < units.size(); i++)
                {
                    var unit = units.at(i);

                    animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
                    var delay = globals.randInt(135, 265);
                    if (animations.length < 5)
                    {
                        delay *= i;
                    }
                    animation.setSound("power4.wav", 1, delay);
                    if (animations.length < 5)
                    {
                        animation.addSprite("power4", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2, delay);
                        animation2.queueAnimation(animation);
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
                    animation.writeDataInt32(unit.getX());
                    animation.writeDataInt32(unit.getY());
                    animation.writeDataInt32(value);
                    animation.writeDataInt32(stunLevel);
                    animation.setEndOfAnimationCall("CO_GRAVES", "postAnimationDamage");

                }
            }
        }
    };

    this.superPowerDamage = 2;
    this.superPowerStunLevel = 4;

    this.powerDamage = 1;
    this.powerStunLevel = 3;
    this.powerOffBonus = 20;
    this.powerDefBonus = 15;

    this.d2dStunLevel = 2;

    this.d2dCoZoneOffBonus = 20;
    this.d2dCoZoneDefBonus = 15;

    this.postAnimationDamage = function(postAnimation, map)
    {
        postAnimation.seekBuffer();
        var x = postAnimation.readDataInt32();
        var y = postAnimation.readDataInt32();
        var damage = postAnimation.readDataInt32();
        var stunLevel = postAnimation.readDataInt32();
        if (map.onMap(x, y))
        {
            var unit = map.getTerrain(x, y).getUnit();
            if (unit !== null)
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
                if (unit.getHpRounded() <= stunLevel)
                {
                    unit.setHasMoved(true);
                }
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

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            if (co.getPowerMode() > GameEnums.PowerMode_Off)
            {
                return CO_GRAVES.powerOffBonus;
            }
            else if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                return CO_GRAVES.d2dCoZoneOffBonus;
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
                return CO_GRAVES.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_GRAVES.d2dCoZoneDefBonus;
            }
        }
        return 0;
    };

    this.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked, weapon, action, map)
    {
        if (CO.isActive(co))
        {
            if (gotAttacked === false && attacker.getOwner() === co.getOwner())
            {
                var stunLevel = 0;
                switch (co.getPowerMode())
                {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                    stunLevel = CO_GRAVES.superPowerStunLevel;
                    break;
                case GameEnums.PowerMode_Power:
                    stunLevel = CO_GRAVES.powerStunLevel;
                    break;
                default:
                    stunLevel = CO_GRAVES.d2dStunLevel;
                    break;
                }
                if (defender.getHpRounded() <= stunLevel)
                {
                    defender.setHasMoved(true);
                }
            }
        }
    };

    this.getAiCoUnitBonus = function(co, unit, map)
    {
        return 1;
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("A former assassin, dissatisfied with where Wars World is headed. Secretly aids Hawke's cause and overtly aids Dark Matter. No one knows where his true loyalties lie.");
    };
    this.getHits = function(co)
    {
        return qsTr("Mystery novels");
    };
    this.getMiss = function(co)
    {
        return qsTr("Romance novels");
    };
    this.getCODescription = function(co)
    {
        var text = qsTr("Enemy units reduced to %0 or less HP by Graves' units become paralyzed and cannot move on their next turn.");
		text = replaceTextArgs(text, [CO_GRAVES.d2dStunLevel]);
        return text;
    };
    this.getLongCODescription = function()
    {
        var text = qsTr("\nGlobal Effect: \nEnemy units reduced to %0 or less HP by Graves' units become paralyzed and cannot move on their next turn.") +
               qsTr("\n\nCO Zone Effect: \nGraves' units gain +%1% firepower and +%2% defence.");
        text = replaceTextArgs(text, [CO_GRAVES.d2dStunLevel, CO_GRAVES.d2dCoZoneOffBonus, CO_GRAVES.d2dCoZoneDefBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Enemy units suffer -%0 HP of damage. Graves' units gain +%1% firepower and +%2% defence. For the rest of the turn, any enemy units reduced to %3 or less HP will become paralyzed and cannot move on their next turn.");
        text = replaceTextArgs(text, [CO_GRAVES.powerDamage, CO_GRAVES.powerOffBonus, CO_GRAVES.powerDefBonus, CO_GRAVES.powerStunLevel]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Plague");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Enemy units suffer -%0 HP of damage. Graves' units gain +%1% firepower and +%2% defence. For the rest of the turn, any enemy units reduced to %3 or less HP will become paralyzed and cannot move on their next turn.");
        text = replaceTextArgs(text, [CO_GRAVES.superPowerDamage, CO_GRAVES.powerOffBonus, CO_GRAVES.powerDefBonus, CO_GRAVES.superPowerStunLevel]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Perdition");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Fear is a valuable tool. I suggest you learn how to use it."),
                qsTr("Do you desire death that greatly?"),
                qsTr("You must give everything if you want to win."),
                qsTr("You are ill prepared to face me."),
                qsTr("A valiant effort, but futile nonetheless."),
                qsTr("Prepare yourself.")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("That was it? I overestimated you."),
                qsTr("Such a victory was... so rudely forced."),
                qsTr("Lives could have been spared had you just accepted your fate.")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("Not as planned, but still not the end."),
                qsTr("It seems that I underestimated your strength.")];
    };
    this.getName = function()
    {
        return qsTr("Graves");
    };
}

Constructor.prototype = CO;
var CO_GRAVES = new Constructor();
