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
        if (weapon === 1 || defender === null)
        {
            sprite.loadSprite("mech+" + armyName,  false,
                              BATTLEANIMATION_MECH.getMaxUnitCount(), Qt.point(-5, 5));
            sprite.loadSprite("mech+" + armyName + "+mask",  true,
                              BATTLEANIMATION_MECH.getMaxUnitCount(), Qt.point(-5, 5));
        }
        else
        {
            BATTLEANIMATION_INFANTRY.loadStandingAnimation(sprite, unit, defender, weapon);
        }
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        if (weapon === 1)
        {
            var weaponRes = "bazooka_os";
            var offset = Qt.point(27, 23);
            var player = unit.getOwner();
            // get army name
            var armyName = player.getArmy().toLowerCase();
            if (armyName === "bg")
            {
                armyName = "bh"
            }
            if (armyName === "yc")
            {
                offset = Qt.point(26, 17);
            }
            else if (armyName === "ge")
            {
                weaponRes = "bazooka_ge";
                offset = Qt.point(28, 15);
            }
            else if (armyName === "bm")
            {
                offset = Qt.point(27, 17);
            }
            else if (armyName === "bh")
            {
                weaponRes = "bazooka_bh"
                offset = Qt.point(25, 14);
            }
            BATTLEANIMATION_MECH.loadStandingAnimation(sprite, unit, defender, weapon);
            sprite.loadMovingSprite(weaponRes, false, sprite.getMaxUnitCount(), offset,
                                    Qt.point(127, 0), 400, false,
                                    1, 1, -1);
        }
        else
        {
            BATTLEANIMATION_INFANTRY.loadFireAnimation(sprite, unit, defender, weapon);
        }
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        if (weapon === 1)
        {
            sprite.loadSprite("unit_explosion",  false, sprite.getMaxUnitCount(), Qt.point(0, 20),
                              1, 1.0, 0, 300);
            sprite.loadMovingSprite("bazooka_os", false, sprite.getMaxUnitCount(), Qt.point(127, 24),
                                    Qt.point(-127, 0), 400, true,
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
    }

    this.getPositionOffset = function(sprite, unit, terrain, unitIdx)
    {
        if (terrain !== null)
        {
            if (terrain.getID() === "MOUNTAIN")
            {
                if (unitIdx >= 4)
                {
                    return Qt.point(-20 * (6 - unitIdx), 0);
                }
            }
        }
        return Qt.point(0, 0);
    };

    this.getFireDurationMS = function()
    {
        // the time will be scaled with animation speed inside the engine
        return 500;
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_MECH = new Constructor();
