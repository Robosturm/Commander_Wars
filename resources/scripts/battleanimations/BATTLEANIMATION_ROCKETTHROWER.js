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
        if ((armyName !== "ac") &&
			(armyName !== "yc") &&
            (armyName !== "ge") &&
            (armyName !== "bm") &&
			(armyName !== "dm") &&
			(armyName !== "ti") &&
            (armyName !== "bh"))
        {
            armyName = "os";
        }
        sprite.loadSprite("rocketthrower+" + armyName,  false,
                          BATTLEANIMATION_ROCKETTHROWER.getMaxUnitCount(), Qt.point(-10, 5));
        sprite.loadSprite("rocketthrower+" + armyName + "+mask",  true,
                          BATTLEANIMATION_ROCKETTHROWER.getMaxUnitCount(), Qt.point(-10, 5));
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_ROCKETTHROWER.loadStandingAnimation(sprite, unit, defender, weapon);
        var player = unit.getOwner();
        // get army name
        var armyName = player.getArmy().toLowerCase();
        if (armyName === "bg")
        {
            armyName = "bh"
        }
        var offset = Qt.point(0, 31);
        // rocket
        if (armyName === "yc")
        {
            offset = Qt.point(0, 32);
        }
        else if (armyName === "ge")
        {
            offset = Qt.point(0, 24);
        }
        else if (armyName === "bm")
        {
            offset = Qt.point(0, 32);
        }
        else if (armyName === "bh")
        {
            offset = Qt.point(0, 33);
        }
        else if (armyName === "ac")
        {
            offset = Qt.point(5, 38);
        }
        else if (armyName === "dm")
        {
            offset = Qt.point(5, 37);
        }
        else if (armyName === "ti")
        {
            offset = Qt.point(-5, 31);
        }
        sprite.loadMovingSprite("rocket_up", false, sprite.getMaxUnitCount(), offset,
                                Qt.point(128, 64), 400, false,
                                1, 1, -1);
    };

    this.getFireDurationMS = function()
    {
        // the time will be scaled with animation speed inside the engine
        return 500;
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("unit_explosion",  false, sprite.getMaxUnitCount(), Qt.point(0, 20),
                          1, 1.0, 0, 300);
        sprite.loadMovingSprite("rocket_down", false, sprite.getMaxUnitCount(), Qt.point(127, 80),
                                Qt.point(-128, -64), 400, true,
                                1, 1, 0, 0, true);
    };

    this.getImpactDurationMS = function()
    {
        // should be a second or longer.
        // the time will be scaled with animation speed inside the engine
        return 1500;
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ROCKETTHROWER = new Constructor();
