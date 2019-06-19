var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(6);
    };

    this.loadCOMusic = function(co)
    {
        // put the co music in here.
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/power.mp3");
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/superpower.mp3");
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/tagpower.mp3");
                break;
            default:
                audio.addMusic("resources/music/cos/sami.mp3")
                break;
        }
    };

    this.activatePower = function(co)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var units = co.getOwner().getUnits();
        var animations = [];
        var counter = 0;
        var unitInfantryIDs = CO_SAMI.getInfantryIDs();
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            if (unitInfantryIDs.indexOf(unit.getUnitID()) >= 0)
            {
                var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
                if (animations.length < 5)
                {
                    animation.addSprite("power10", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                    powerNameAnimation.queueAnimation(animation);
                    animations.push(animation);
                }
                else
                {
                    animation.addSprite("power10", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
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

        audio.clearPlayList();
        CO_SAMI.loadCOMusic(co);
        audio.playRandom();
    };

    this.activateSuperpower = function(co, powerMode)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var units = co.getOwner().getUnits();
        var animations = [];
        var counter = 0;
        var unitInfantryIDs = CO_SAMI.getInfantryIDs();
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            if (unitInfantryIDs.indexOf(unit.getUnitID())  >= 0)
            {
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
        }
        units.remove();

        audio.clearPlayList();
        CO_SAMI.loadCOMusic(co);
        audio.playRandom();
    };

    this.getCOUnitRange = function(co)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "OS";
    };
    this.getInfantryIDs = function()
    {
        return ["INFANTRY", "MECH", "SNIPER", "MOTORBIKE"];
    };

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        var unitInfantryIDs = CO_SAMI.getInfantryIDs();
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (unitInfantryIDs.indexOf(attacker.getUnitID()) >= 0)
                {
                    return 80;
                }                
                else if (attacker.getBaseMaxRange() === 1)
                {
                    return 0;
                }
                return 10;
            case GameEnums.PowerMode_Power:
                if (unitInfantryIDs.indexOf(attacker.getUnitID()) >= 0)
                {
                    return 50;
                }

                else if (attacker.getBaseMaxRange() === 1)
                {
                    return 0;
                }
                return 10;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    if (unitInfantryIDs.indexOf(attacker.getUnitID()) >= 0)
                    {
                        return 40;
                    }
                    else if (attacker.getBaseMaxRange() === 1)
                    {
                        return 0;
                    }
                    return 10;
                }
                if (unitInfantryIDs.indexOf(attacker.getUnitID()) >= 0)
                {
                    return 10;
                }
                break;
        }
        if (attacker.getBaseMaxRange() === 1)
        {
            return -10;
        }
        return 0;
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
    this.getCaptureBonus = function(co, unit, posX, posY)
    {
        if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
            co.getPowerMode() === GameEnums.PowerMode_Tagpower)
        {
            return 20;
        }
        else
        {
            var hp = unit.getHpRounded();
            return hp / 2;
        }
    };

    this.getMovementpointModifier = function(co, unit, posX, posY)
    {
        var unitTransportIDs = ["APC", "LANDER", "T_HELI", "TRANSPORTPLANE", "BLACK_BOOT"];
        if (unitTransportIDs.indexOf(unit.getUnitID()) >= 0)
        {
            return 1;
        }
        var unitInfantryIDs = CO_SAMI.getInfantryIDs();
        if (unitInfantryIDs.indexOf(unit.getUnitID()) >= 0)
        {
            switch (co.getPowerMode())
            {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                    return 2;
                case GameEnums.PowerMode_Power:
                    return 1;
                default:
                    return 0;
            }
        }
        return 0;
    };

    // CO - Intel
    this.getBio = function()
    {
        return qsTr("A strong-willed Orange Star special forces captain who loves long hair. Despite having short hair. Whatever, IS.");
    };
    this.getHits = function()
    {
        return qsTr("Chocolate");
    };
    this.getMiss = function()
    {
        return qsTr("Cowards");
    };
    this.getCODescription = function()
    {
        return qsTr("As an infantry specialist, her foot soldiers do more damage and capture faster. Non-infantry direct-combat units have weaker firepower.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("Infantry units receive a movement bonus of one space. Their attack also increases. ");
    };
    this.getPowerName = function()
    {
        return qsTr("Double Time");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("All foot soldiers's capturing rate is doubled. Their movement is increased by two spaces and their attack increases greatly. ");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("Victory March");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("You're not bad!  Now it's my turn!"),
                qsTr("All right!  Time to end this!"),
                qsTr("Infantry... Assault!"),
                qsTr("Ready or not, here I come!"),
                qsTr("All right, it's make-or-break time!"),
                qsTr("Move out, grunts!")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("Mission accomplished! Awaiting orders!"),
                qsTr("Commandos always complete their mission."),
                qsTr("Score one for the grunts!")];
    };
    this.getDefeatSentences = function()
    {
        return [qsTr("Things would be easier if we had more infantry units..."),
                qsTr("Next time's for real. I won't lose focus.")];
    };
    this.getName = function()
    {
        return qsTr("Sami");
    };
}

Constructor.prototype = CO;
var CO_SAMI = new Constructor();
