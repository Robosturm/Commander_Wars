var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(5);
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
            if (animations.length < 5)
            {
                animation.addSprite("power2", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power2", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
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
            audio.addMusic("resources/music/cos/power.mp3", 992, 45321);
            break;
        case GameEnums.PowerMode_Superpower:
            audio.addMusic("resources/music/cos/superpower.mp3", 1505, 49515);
            break;
        case GameEnums.PowerMode_Tagpower:
            audio.addMusic("resources/music/cos/tagpower.mp3", 14611, 65538);
            break;
        default:
            audio.addMusic("resources/music/cos/amy.mp3", 3444, 604297)
            break;
        }
    };

    this.getCOUnitRange = function(co)
    {
        return 2;
    };
    this.getCOArmy = function()
    {
        return "PF";
    };
    this.hoverCraftBoost = 80;
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (attacker.getMovementType() === "MOVE_HOVERCRAFT")
            {
                return 80;
            }
            return 10;
        case GameEnums.PowerMode_Power:
            if (attacker.getMovementType() === "MOVE_HOVERCRAFT")
            {
                return 80;
            }
            return 10;
        default:
            if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                if (attacker.getMovementType() === "MOVE_HOVERCRAFT")
                {
                    return CO_AMY.hoverCraftBoost;
                }
                return 10;
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
            if (isDefender === true)
            {
                return 99999;
            }
            return 110;
        case GameEnums.PowerMode_Power:
            return 10;
        default:
            if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return 10;
            }
            break;
        }
        return 0;
    };
    this.getMovementpointModifier = function(co, unit, posX, posY)
    {
        if (co.getPowerMode() === GameEnums.PowerMode_Power)
        {
            if (unit.getMovementType() === "MOVE_HOVERCRAFT")
            {
                return 1;
            }
        }
        return 0;
    };
    this.getCostModifier = function(co, id, baseCost)
    {
        if (co.getPowerMode() === GameEnums.PowerMode_Power)
        {
            var hoverUnits = ["HEAVY_HOVERCRAFT", "HOVERCRAFT", "HOVERFLAK", "ARTILLERYCRAFT"]
            if (hoverUnits.indexOf(id) >= 0)
            {
                return -baseCost * 0.3;
            }
        }
        return 0;

    };

    this.getMovementcostModifier = function(co, unit, posX, posY)
    {
        if (map.getTerrain(posX, posY).getTerrainID() === "REAF")
        {
            return -999;
        }
        return 0;
    };

    this.getAiCoUnitBonus = function(co, unit)
    {
        if (unit.getMovementType() === "MOVE_HOVERCRAFT")
        {
            return 3;
        }
        return 0;
    };

    // CO - Intel
    this.getBio = function(co)
    {
        return qsTr("The Commander in Chief of Pink Frontier. A former physicist who was thrust into war emerging as a hardened naval commander.");
    };
    this.getHits = function(co)
    {
        return qsTr("Quantum Theory");
    };
    this.getMiss = function(co)
    {
        return qsTr("Hawke");
    };
    this.getCODescription = function(co)
    {
        return qsTr("She can command hovercraft units at optimal level. Reefs pose no problem for Amy's units.");
    };
    this.getLongCODescription = function()
    {
        var text = qsTr("\nGlobal Effect: \nReaf costs are always 1.") +
                qsTr("\n\nCO Zone Effect: \nHovercrafts gain %0% firepower boost.");
        text = replaceTextArgs(text, [CO_AMY.hoverCraftBoost]);
        return text;
    };
    this.getPowerDescription = function(co)
    {
        return qsTr("Hovercraft units are strengthened, they gain one extra movement and get cheaper.");
    };
    this.getPowerName = function(co)
    {
        return qsTr("Fluid Dynamics");
    };
    this.getSuperPowerDescription = function(co)
    {
        return qsTr("Her defence raises extremly. Her troops get nearly invincible when attacking.");
    };
    this.getSuperPowerName = function(co)
    {
        return qsTr("Photon Discharge");
    };
    this.getPowerSentences = function(co)
    {
        return [qsTr("EMP? Why, it stands for Electromagnetic Pulse? You'll soon understand."),
                qsTr("Quantum... you probably don't even know what it means."),
                qsTr("Do you know how much energy splitting of the atom produces? Let's see."),
                qsTr("Allow me demonstrate how flimsy hovercrafts are not."),
                qsTr("For your sake, I hope your machines are magnetically shielded."),
                qsTr("Without science we wouldn't be doing what we are now.")];
    };
    this.getVictorySentences = function(co)
    {
        return [qsTr("That one was for you Lara."),
                qsTr("Theoretically speaking, it appears you lost."),
                qsTr("For lack of a better word, I feel fully ionised!")];
    };
    this.getDefeatSentences = function(co)
    {
        return [qsTr("My theory didn't work in practice!"),
                qsTr("Ok, I'll need to put more effort to put my theory into practice.")];
    };
    this.getName = function()
    {
        return qsTr("Amy");
    };
}

Constructor.prototype = CO;
var CO_AMY = new Constructor();
