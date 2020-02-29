var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        var player = unit.getOwner();
        // get army name
        var armyName = player.getArmy().toLowerCase();
        if (armyName === "bg")
        {
            armyName = "bh"
        }
        if ((armyName !== "yc") &&
            (armyName !== "ge") &&
            (armyName !== "bm") &&
            (armyName !== "bh") &&
            (armyName !== "ma"))
        {
            armyName = "os";
        }
        var offset = Qt.point(-5, 5);
        if (armyName === "ma")
        {
            offset = Qt.point(-15, 5);
        }
        sprite.loadSprite("flak+" + armyName,  false,
                          BATTLEANIMATION_FLAK.getMaxUnitCount(), offset);
        sprite.loadSprite("flak+" + armyName + "+mask",  true,
                          BATTLEANIMATION_FLAK.getMaxUnitCount(), offset);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        // mg
        var player = unit.getOwner();
        // get army name
        var armyName = player.getArmy().toLowerCase();
        if (armyName === "bg")
        {
            armyName = "bh"
        }
        if ((armyName !== "yc") &&
            (armyName !== "ge") &&
            (armyName !== "bm") &&
            (armyName !== "bh") &&
            (armyName !== "ma"))
        {
            armyName = "os";
        }

        var offset = Qt.point(-5, 5);
        if (armyName === "yc")
        {
            offset = Qt.point(37, 30);
        }
        else if (armyName === "ge")
        {
            offset = Qt.point(38, 28);
        }
        else if (armyName === "bm")
        {
            offset = Qt.point(35, 24);
        }
        else if (armyName === "bh")
        {
            offset = Qt.point(-38, 5);
        }
        else if (armyName === "ma")
        {
            offset = Qt.point(24, 48);
        }
        if (armyName === "bh" ||
            armyName === "os")
        {
            if (defender.getUnitType() === GameEnums.UnitType_Air)
            {
                sprite.loadSprite("flak+" + armyName + "+fire+air",  false,
                                  BATTLEANIMATION_FLAK.getMaxUnitCount(), offset, 2);
                sprite.loadSprite("flak+" + armyName + "+fire+air+mask",  true,
                                  BATTLEANIMATION_FLAK.getMaxUnitCount(), offset, 2);
            }
            else
            {
                sprite.loadSprite("flak+" + armyName + "+fire+ground",  false,
                                  BATTLEANIMATION_FLAK.getMaxUnitCount(), offset, 2);
                sprite.loadSprite("flak+" + armyName + "+fire+ground+mask",  true,
                                  BATTLEANIMATION_FLAK.getMaxUnitCount(), offset, 2);
            }
        }
        else
        {
            BATTLEANIMATION_FLAK.loadStandingAnimation(sprite, unit, defender, weapon);
            sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), offset,
                              2, 1, 0, 0);
        }
    };

    this.getFireDurationMS = function()
    {
        // the time will be scaled with animation speed inside the engine
        return 850;
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("mg_hit",  false, sprite.getMaxUnitCount(), Qt.point(0, 22),
                          1, 1.0, 0, 0);
        sprite.loadSound("mg_impact.wav", 1, "resources/sounds/", 0);
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_FLAK = new Constructor();
