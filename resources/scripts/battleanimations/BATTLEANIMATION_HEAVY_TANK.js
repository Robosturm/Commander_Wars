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
        sprite.loadSprite("heavy_tank+" + armyName,  false,
                          BATTLEANIMATION_HEAVY_TANK.getMaxUnitCount(), Qt.point(-10, 5));
        sprite.loadSprite("heavy_tank+" + armyName + "+mask",  true,
                          BATTLEANIMATION_HEAVY_TANK.getMaxUnitCount(), Qt.point(-10, 5));
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
            offset = Qt.point(27, 20);
            // gun
            if (armyName === "yc")
            {
                offset = Qt.point(27, 20);
            }
            else if (armyName === "ge")
            {
                offset = Qt.point(28, 22);
            }
            else if (armyName === "bm")
            {
                offset = Qt.point(28, 16);
            }
            else if (armyName === "bh")
            {
                offset = Qt.point(28, 20);
            }
            sprite.loadSprite("medium_shot",  false, sprite.getMaxUnitCount(), offset,
                              1, 1.0, 0, 0);
            sprite.loadSound("tank_shot.wav", 1, "resources/sounds/", 0);
        }
        else
        {
            // mg
            offset = Qt.point(24, 47);
            if (armyName === "yc")
            {
                offset = Qt.point(32, 48);
            }
            else if (armyName === "ge")
            {
                offset = Qt.point(33, 47);
            }
            else if (armyName === "bm")
            {
                offset = Qt.point(32, 46);
            }
            else if (armyName === "bh")
            {
                offset = Qt.point(33, 47);
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
var BATTLEANIMATION_HEAVY_TANK = new Constructor();
