var Constructor = function()
{
    this.getCOStyles = function()
    {
        // string array containing the endings of the alternate co style
        // e.g. co_adder has ["+alt"]
        return ["+alt"];
    };

    this.init = function(co)
    {
        co.setPowerStars(6);
        co.setSuperpowerStars(4);
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
                audio.addMusic("resources/music/cos/von_bolt.mp3", 47693, 113984);
                break;
        }
    };

    this.activatePower = function(co)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        dialogAnimation.queueAnimation(powerNameAnimation);

        CO_VON_BOLT.throwLaserray(co, 1, 2, powerNameAnimation);
    };

    this.activateSuperpower = function(co, powerMode)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        powerNameAnimation.queueAnimationBefore(dialogAnimation);

        CO_VON_BOLT.throwLaserray(co, 3, 3, powerNameAnimation);
    };

    this.throwLaserray = function(co, damage, range, powerNameAnimation)
    {
        // let a meteor fall :D
        var meteorTarget = co.getOwner().getRockettarget(range, damage);

        var animation2 = GameAnimationFactory.createAnimation(0, 0);
        animation2.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation2.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);
        powerNameAnimation.queueAnimation(animation2);

        animation2.setEndOfAnimationCall("CO_VON_BOLT", "postAnimationLaserray");
        CO_VON_BOLT.postAnimationLaserrayTarget = meteorTarget;
        CO_VON_BOLT.postAnimationLaserrayDamage = damage;
        CO_VON_BOLT.postAnimationLaserrayRange = range;
    };

    this.postAnimationLaserrayTarget = null;
    this.postAnimationLaserrayDamage = 0;
    this.postAnimationLaserrayRange = 0;
    this.postAnimationLaserray = function(animation)
    {
        var laserTarget = CO_VON_BOLT.postAnimationLaserrayTarget;
        var damage = CO_VON_BOLT.postAnimationLaserrayDamage;
        var range = CO_VON_BOLT.postAnimationLaserrayRange;
        var fields = globals.getCircle(0, range);
        // check all target fields
        for (var i = 0; i < fields.size(); i++)
        {
            var x = fields.at(i).x + laserTarget.x;
            var y = fields.at(i).y + laserTarget.y;
            // check with which weapon we can attack and if we could deal damage with this weapon
            if (map.onMap(x, y))
            {
                var unit = map.getTerrain(x, y).getUnit();
                if (unit !== null)
                {
                    unit.setHasMoved(true);
                    var hp = unit.getHpRounded();
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
        fields.remove();
        CO_VON_BOLT.postAnimationLaserrayTarget = null;
        CO_VON_BOLT.postAnimationLaserrayDamage = 0;
        CO_VON_BOLT.postAnimationLaserrayRange = 0;
    };

    this.getCOUnitRange = function(co)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "BG";
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return 40;
            case GameEnums.PowerMode_Power:
                return 40;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return 40;
                }
                break;
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
                return 40;
            case GameEnums.PowerMode_Power:
                return 40;
            default:
                if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                {
                    return 40;
                }
                break;
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
            return ["ZCOUNIT_CRYSTAL_TANK"];
        }
        return [];
    };
    this.getAiCoUnitBonus = function(co, unit)
    {
        return 1;
    };
    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("Former commander-in-chief of the Black Hole forces. A mysterious old man who has been alive a very, very long time. Mastermind of the Omega war. Immortal Evil");
    };
    this.getHits = function(co)
    {
        return qsTr("Long life");
    };
    this.getMiss = function(co)
    {
        return qsTr("Young 'uns!");
    };
    this.getCODescription = function(co)
    {
        return qsTr("All units have superior firepower and defense.");
    };
    this.getLongCODescription = function()
    {
        return qsTr("\nSpecial Unit:\nCrystal Tanks\n\nGlobal Effect: \nNo Effects.") +
               qsTr("\n\nCO Zone Effect: \nUnits have increased firepower and increased defense.");
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("Fires shock waves that disables electical systems and all forces in range become paralyzed. Affected units suffer one HP of damage. Firepower and defense rises.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Shockwave");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("Fires shock waves that disables electical systems and all forces in range become paralyzed. Affected units suffer three HP of damage. Firepower and defense rises.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Deus Ex Machina");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("No matter how powerful...hhh...you are, you're...hhh...still a worm on a hook..."),
                qsTr("Well...hhh...let me have a taste...hhh...of your soul."),
                qsTr("Hheh heh heh... Stop panicking... it's...hhh...almost over..."),
                qsTr("Hheh heh heh. The darkness rises..."),
                qsTr("The sweet scent of destruction...hhh...it makes me...hhh...feel young!"),
                qsTr("Hold still...hhh...I'm going to...hhhhh...suck the marrow from your bones.")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("Predator...prey...hhh... I hunt them all..."),
                qsTr("Hehh hhh hhh... Pathetic."),
                qsTr("Even death...hhh...fears me...")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("What? Why are you Youth so strong?"),
                qsTr("Nooo...I want to live forever.")];
    };
    this.getName = function()
    {
        return qsTr("Von Bolt");
    };
}

Constructor.prototype = CO;
var CO_VON_BOLT = new Constructor();
