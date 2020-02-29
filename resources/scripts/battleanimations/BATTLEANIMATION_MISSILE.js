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
            offset = Qt.point(-35, 5);
        }
        sprite.loadSprite("missile+" + armyName,  false,
                          BATTLEANIMATION_MISSILE.getMaxUnitCount(), offset);
        sprite.loadSprite("missile+" + armyName + "+mask",  true,
                          BATTLEANIMATION_MISSILE.getMaxUnitCount(), offset);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_MISSILE.loadStandingAnimation(sprite, unit, defender, weapon);
        var player = unit.getOwner();
        // get army name
        var armyName = player.getArmy().toLowerCase();
        if (armyName === "bg")
        {
            armyName = "bh"
        }
        var offset = Qt.point(5, 30);
        // rocket
        if (armyName === "yc")
        {
            offset = Qt.point(7, 30);
        }
        else if (armyName === "ge")
        {
            offset = Qt.point(8, 30);
        }
        else if (armyName === "ma")
        {
            offset = Qt.point(5, 40);
        }
        else if (armyName === "bm")
        {
            offset = Qt.point(8, 30);
        }
        else if (armyName === "bh")
        {
            offset = Qt.point(8, 28);
        }
        sprite.loadMovingSprite("rocket_up", false, sprite.getMaxUnitCount(), offset,
                                Qt.point(128, 64), 400, false,
                                1, 1, -1);
        sprite.loadSound("rocket_launch.wav", 1, "resources/sounds/", 0);
    };

    this.getFireDurationMS = function()
    {
        // the time will be scaled with animation speed inside the engine
        return 500;
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("unit_explosion",  false, sprite.getMaxUnitCount(), Qt.point(0, 60),
                          1, 1.0, 0, 300);
        sprite.loadMovingSprite("rocket_up", false, sprite.getMaxUnitCount(), Qt.point(127, 0),
                                Qt.point(-128, 64), 400, true,
                                1, 1, 0, 0, true);
        sprite.loadSound("rocket_flying.wav", 1, "resources/sounds/", 0);
        sprite.loadSound("impact_explosion.wav", 1, "resources/sounds/", 200);
    };

    this.getImpactDurationMS = function()
    {
        // should be a second or longer.
        // the time will be scaled with animation speed inside the engine
        return 1500;
    }
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_MISSILE = new Constructor();
