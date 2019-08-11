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
            (armyName !== "bh"))
        {
            armyName = "os";
        }
        sprite.loadSprite("light_tank+" + armyName,  false,
                          BATTLEANIMATION_LIGHT_TANK.getMaxUnitCount(), Qt.point(-5, 5));
        sprite.loadSprite("light_tank+" + armyName + "+mask",  true,
                          BATTLEANIMATION_LIGHT_TANK.getMaxUnitCount(), Qt.point(-5, 5));
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadAnimation("loadStandingAnimation", unit);
        var player = unit.getOwner();
        // get army name
        var armyName = player.getArmy().toLowerCase();
        if (armyName === "bg")
        {
            armyName = "bh"
        }
        var offset = Qt.point(0, 0);
        if (weapon === 0)
        {
            // gun
            offset = Qt.point(21, 21);
            if (armyName === "yc")
            {
                offset = Qt.point(29, 16);
            }
            else if (armyName === "ge")
            {
                offset = Qt.point(32, 21);
            }
            else if (armyName === "bm")
            {
                offset = Qt.point(32, 16);
            }
            else if (armyName === "bh")
            {
                offset = Qt.point(31, 21);
            }
            sprite.loadSprite("medium_shot",  false, sprite.getMaxUnitCount(), offset,
                              1, 0.5, 0, 0);
        }
        else
        {
            // mg
            offset = Qt.point(24, 40);
            if (armyName === "yc")
            {
                offset = Qt.point(35, 36);
            }
            else if (armyName === "ge")
            {
                offset = Qt.point(36, 37);
            }
            else if (armyName === "bm")
            {
                offset = Qt.point(35, 31);
            }
            else if (armyName === "bh")
            {
                offset = Qt.point(32, 36);
            }
            sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), offset,
                              1, 1, 0, 0);
        }
    };

    this.getFireDurationMS = function()
    {
        // the time will be scaled with animation speed inside the engine
        return 500;
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_LIGHT_TANK = new Constructor();
