var Constructor = function()
{
    this.init = function(co, map)
    {
        co.setPowerStars(5);
        co.setSuperpowerStars(4);
    };

    this.getCOStyles = function()
    {
        return ["+alt"];
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
                audio.addMusic("resources/music/cos/power.mp3", 992, 45321);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/superpower.mp3", 1505, 49515);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/tagpower.mp3", 14611, 65538);
                break;
            default:
                audio.addMusic("resources/music/cos/nana.mp3", 409, 66237);
                break;
            }
        }
    };

    this.superPowerExplosionRange = 2;
    this.superPowerExplosionDamage = 2;
    this.powerOffBonus = 10;
    this.powerDefBonus = 10;
    this.powerExplosionDamage = 2;

    this.d2dCoZoneOffBonus = 10;
    this.d2dCoZoneDefBonus = 10;
    this.d2dCoZoneExplosionDamage = 1;

    this.d2dExplosionRange = 1;
    this.d2dExplosionDamage = 0;

    this.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked, weapon, action, map)
    {
        if (CO.isActive(co) && defender !== null)
        {
            if (!gotAttacked && attacker.getOwner() === co.getOwner())
            {
                var owner = co.getOwner();
                var powerMode = co.getPowerMode();
                var damage = 0;
                var explosionRange = CO_NANA.d2dExplosionRange;
                var friendlyFire = true;
                var defX = defender.getX();
                var defY = defender.getY();
                var unit = null;
                var point = Qt.point(0, 0);
                var hp = 0;
                switch (powerMode)
                {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                    damage = CO_NANA.superPowerExplosionRange;
                    explosionRange = CO_NANA.superPowerExplosionDamage;
                    friendlyFire = false;
                    break;
                case GameEnums.PowerMode_Power:
                    if (attacker.getBaseMaxRange() === 1)
                    {
                        damage = CO_NANA.powerExplosionDamage;
                        var width = map.getMapWidth();
                        var heigth = map.getMapHeight();
                        var maxRange = width;
                        if (heigth > width)
                        {
                            maxRange = heigth;
                        }
                        var found = false;
                        // deal direct chain damage to nearest enemy
                        for (var curRange = 1; curRange < maxRange; ++curRange)
                        {
                            var directCircle = globals.getCircle(curRange, curRange);
                            var size = directCircle.size();
                            for (var i = 0; i < size; i++)
                            {
                                point = directCircle.at(i);
                                if (map.onMap(defX + point.x, defY + point.y))
                                {
                                    unit = map.getTerrain(defX + point.x, defY + point.y).getUnit();
                                    if (unit !== null &&
                                        owner.isEnemyUnit(unit))
                                    {
                                        hp = unit.getHpRounded();
                                        if (hp - damage <= 0.1)
                                        {
                                            // set hp to very very low
                                            unit.setHp(0.1);
                                        }
                                        else
                                        {
                                            unit.setHp(hp - damage);
                                        }
                                        found = true;
                                        break;
                                    }
                                }
                            }
                            if (found === true)
                            {
                                break;
                            }
                        }
                    }
                    // indirect damage setup
                    friendlyFire = false;
                    damage = CO_NANA.d2dExplosionDamage;
                    break;
                default:
                    if (co.inCORange(Qt.point(attacker.getX(), attacker.getY()), attacker))
                    {
                        damage = CO_NANA.d2dCoZoneExplosionDamage;
                    }
                    else
                    {
                        damage = CO_NANA.d2dExplosionDamage;
                    }
                    break;
                }

                // deal indirect unit splash damage
                if (attacker.getBaseMaxRange() > 1 && damage > 0)
                {
                    var circle = globals.getCircle(1, explosionRange);
                    for (var i = 0; i < circle.size(); i++)
                    {
                        point = circle.at(i);
                        if (map.onMap(defX + point.x, defY + point.y))
                        {
                            unit = map.getTerrain(defX + point.x, defY + point.y).getUnit();
                            if (unit !== null)
                            {
                                if (owner !== unit.getOwner() || friendlyFire === true)
                                {
                                    hp = unit.getHpRounded();
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
                            }
                        }
                    }
                }
            }
        }
    };

    this.getCOUnitRange = function(co, map)
    {
        return 2;
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
        if (CO.isActive(co))
        {
            if (co.getPowerMode() > GameEnums.PowerMode_Off)
            {
                return CO_NANA.powerOffBonus;
            }
            else if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                return CO_NANA.d2dCoZoneOffBonus;
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
                return CO_NANA.powerDefBonus;
            }
            else if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return CO_NANA.d2dCoZoneDefBonus;
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
                return ["ZCOUNIT_SIEGE_CANNON"];
            }
        }
        return [];
    };

    this.getCOArmy = function()
    {
        return "PF";
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("The lovable, cute, and amazingly talented granddaughter of Hachi. Known to be incredibly scary when angry.");
    };
    this.getHits = function(co)
    {
        return qsTr("Artemis");
    };
    this.getMiss = function(co)
    {
        return qsTr("Ozzy");
    };
    this.getCODescription = function(co)
    {
        return qsTr("Indirect attacks deal damage to all nearby units, no matter their allegiance.");
    };
    this.getLongCODescription = function()
    {
        var text = qsTr("\nSpecial Unit:\nSiege Cannon\n") +
               qsTr("\nGlobal Effect: \nAttacks made by Nana's indirect units deal -%0 HP of damage to all adjacent units, no matter their allegiance.") +
               qsTr("\n\nCO Zone Effect: \nNana's units gain +%2% firepower and +%3% defence. Attacks made by her indirect units deal -%1 HP of damage to all adjacent units, no matter their allegiance.");
        text = replaceTextArgs(text, [CO_NANA.d2dExplosionDamage, CO_NANA.d2dCoZoneExplosionDamage, CO_NANA.d2dCoZoneOffBonus, CO_NANA.d2dCoZoneDefBonus]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        var text = qsTr("Nana's units gain +%2% firepower and +%3% defence. Attacks made by her direct combat units deal -%0 HP of damage to the nearest enemy unit. Attacks made by her indirect units deal -%1 HP of splash damage to all adjacent units, except for her own.");
        text = replaceTextArgs(text, [CO_NANA.powerExplosionDamage, CO_NANA.d2dCoZoneExplosionDamage, CO_NANA.powerOffBonus, CO_NANA.powerDefBonus]);
        return text;
    };
    this.getPowerName = function(co)
    {
        return qsTr("Chain Reaction");
    };
    this.getSuperPowerDescription = function(co)
    {
        var text = qsTr("Nana's units gain +%2% firepower and +%3% defence. Attacks made by her indirect units deal -%0 HP of splash damage to all units, except for her own, in a %1-space radius from the target.");
        text = replaceTextArgs(text, [CO_NANA.superPowerExplosionDamage, CO_NANA.superPowerExplosionRange, CO_NANA.powerOffBonus, CO_NANA.powerDefBonus]);
        return text;
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Explosive Tantrum");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Let's go, boys!"),
                qsTr("You've made me mad!"),
                qsTr("Stop annoying me!"),
                qsTr("I'm gonna kick your butt!"),
                qsTr("I wanna end this fast!"),
                qsTr("Don't be such a meanie!")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Aww, you put up a good fight though!"),
                qsTr("Hehe! You're funny!"),
                qsTr("You look so cute when you lose!")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("Now I'm mad..."),
                qsTr("This should've been easy!")];
    };
    this.getName = function()
    {
        return qsTr("Nana");
    };
}

Constructor.prototype = CO;
var CO_NANA = new Constructor();
