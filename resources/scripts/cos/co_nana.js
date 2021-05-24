var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(5);
        co.setSuperpowerStars(4);
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
                audio.addMusic("resources/music/cos/nana.mp3", 409, 66237);
                break;
        }
    };

    this.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked)
    {
        if (!gotAttacked && attacker.getOwner() === co.getOwner())
        {
            var owner = co.getOwner();
            var powerMode = co.getPowerMode();
            var damage = 0;
            var explosionRange = 1;
            var friendlyFire = true;
            var i = 0;
            var defX = defender.getX();
            var defY = defender.getY();
            var unit = null;
            var point = Qt.point(0, 0);
            var hp = 0;
            switch (powerMode)
            {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                    damage = 2;
                    explosionRange = 2;
                    friendlyFire = false;
                    break;
                case GameEnums.PowerMode_Power:
                    friendlyFire = false;
                    damage = 1;
                    if (attacker.getBaseMaxRange() === 1 && damage > 0)
                    {
                        damage = 2;
                        var width = map.getMapWidth();
                        var heigth = map.getMapHeight();
                        var maxRange = width;
                        if (heigth > width)
                        {
                            maxRange = heigth;
                        }
                        var found = false;
                        // deal direct chain damage to nearest enemy
                        for (var curRange = 1; curRange < maxRange; curRange++)
                        {
                            var directCircle = globals.getCircle(curRange, curRange);
                            for (i = 0; i < directCircle.size(); i++)
                            {
                                point = directCircle.at(i);
                                if (map.onMap(defX + point.x, defY + point.y))
                                {
                                    unit = map.getTerrain(defX + point.x, defY + point.y).getUnit();
                                    if (unit !== null &&
                                        owner.isEnemyUnit(unit))
                                    {
                                        hp = unit.getHpRounded();
                                        if (hp <= damage)
                                        {
                                            // set hp to very very low
                                            unit.setHp(0.001);
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
                            directCircle.remove()
                            if (found === true)
                            {
                                break;
                            }
                        }
                    }
                    break;
                default:
                    if (co.inCORange(Qt.point(attacker.getX(), attacker.getY()), attacker))
                    {
                        damage = 1;
                    }
                    break;
            }

            // deal indirect unit splash damage
            if (attacker.getBaseMaxRange() > 1 && damage > 0)
            {
                var circle = globals.getCircle(1, explosionRange);
                for (i = 0; i < circle.size(); i++)
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
                                if (hp <= damage)
                                {
                                    // set hp to very very low
                                    unit.setHp(0.001);
                                }
                                else
                                {
                                    unit.setHp(hp - damage);
                                }
                            }
                        }
                    }
                }
                circle.remove()
            }
        }
    };

    this.getCOUnitRange = function(co)
    {
        return 2;
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action)
    {
        if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker) ||
                co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 10;
        }
        return 0;
    };
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action)
    {
        if (co.inCORange(Qt.point(defPosX, defPosY), defender) ||
                co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 10;
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
    this.getCOUnits = function(co, building)
    {
        var buildingId = building.getBuildingID();
        if (buildingId === "FACTORY" ||
            buildingId === "TOWN" ||
            buildingId === "HQ")
        {
            return ["ZCOUNIT_SIEGE_CANNON"];
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
        return qsTr("Indirect attacks deal one HP of damage to all nearby units, no matter their allegiance.");
    };
    this.getLongCODescription = function()
    {
        return qsTr("\nSpecial Unit:\nSiege Cannon\n") +
               qsTr("\nGlobal Effect: \nNo Effects.") +
               qsTr("\n\nCO Zone Effect: \nIndirect attacks deal one HP of damage to all nearby units, no matter their allegiance.");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("Direct attacks deal one HP of damage to the nearest enemy unit. Nana's units are no longer affected by collateral damage.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Chain Reaction");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("Indirect attacks deal two HP of damage to all units within a large blast radius. Nana's units are no longer affected by collateral damage.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Explosive Tantrum");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("Let's go, boys!"),
                qsTr("You made me mad!"),
                qsTr("Stop annoying me!"),
                qsTr("I'm gonna kick your butt!"),
                qsTr("A battle with me is a greater honor than you deserve!"),
                qsTr("I want to end this fast!"),
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
                qsTr("This should have been easy!")];
    };
    this.getName = function()
    {
        return qsTr("Nana");
    };
}

Constructor.prototype = CO;
var CO_NANA = new Constructor();
