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
        sprite.loadSprite("k_heli+" + armyName,  false,
                          BATTLEANIMATION_K_HELI.getMaxUnitCount(), Qt.point(-40, 30));
        sprite.loadSprite("k_heli+" + armyName + "+mask",  true,
                          BATTLEANIMATION_K_HELI.getMaxUnitCount(), Qt.point(-40, 30));
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_K_HELI.loadStandingAnimation(sprite, unit, defender, weapon);
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
            offset = Qt.point(-5, 41);
            if (armyName === "yc")
            {
                offset = Qt.point(-5, 45);
            }
            else if (armyName === "ge")
            {
                offset = Qt.point(-5, 45);
            }
            else if (armyName === "bm")
            {
                offset = Qt.point(-5, 45);
            }
            else if (armyName === "bh")
            {
                offset = Qt.point(-5, 45);
            }
            sprite.loadMovingSprite("rocket", false, sprite.getMaxUnitCount(), offset,
                                    Qt.point(127, -100), 400, false,
                                    1, 1, -1);
        }
        else
        {
            // mg
            offset = Qt.point(34, 38);
            if (armyName === "yc")
            {
                offset = Qt.point(32, 45);
            }
            else if (armyName === "ge")
            {
                offset = Qt.point(37, 38);
            }
            else if (armyName === "bm")
            {
                offset = Qt.point(34, 31);
            }
            else if (armyName === "bh")
            {
                offset = Qt.point(40, 38);
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

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        if (weapon === 0)
        {
            sprite.loadSprite("unit_explosion",  false, sprite.getMaxUnitCount(), Qt.point(0, 20),
                              1, 1.0, 0, 300);
            sprite.loadMovingSprite("rocket", false, sprite.getMaxUnitCount(), Qt.point(127, 90),
                                    Qt.point(-127, -60), 400, true,
                                    1, 1, 0, 0, true);
        }
        else
        {
            sprite.loadSprite("mg_hit",  false, sprite.getMaxUnitCount(), Qt.point(0, 22),
                              1, 1.0, 0, 0);
        }
    };

    this.getImpactDurationMS = function()
    {
        // should be a second or longer.
        // the time will be scaled with animation speed inside the engine
        return 1500;
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_K_HELI = new Constructor();
