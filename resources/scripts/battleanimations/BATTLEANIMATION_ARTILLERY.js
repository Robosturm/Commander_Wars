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
        sprite.loadSprite("artillery+" + armyName,  false,
                          BATTLEANIMATION_ARTILLERY.getMaxUnitCount(), Qt.point(-5, 5));
        sprite.loadSprite("artillery+" + armyName + "+mask",  true,
                          BATTLEANIMATION_ARTILLERY.getMaxUnitCount(), Qt.point(-5, 5));
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_ARTILLERY.loadStandingAnimation(sprite, unit, defender, weapon);
        var player = unit.getOwner();
        // get army name
        var armyName = player.getArmy().toLowerCase();
        if (armyName === "bg")
        {
            armyName = "bh"
        }
        var offset = Qt.point(30, 37);
        // gun
        if (armyName === "yc")
        {
            offset = Qt.point(30, 37);
        }
        else if (armyName === "ge")
        {
            offset = Qt.point(30, 37);
        }
        else if (armyName === "bm")
        {
            offset = Qt.point(30, 37);
        }
        else if (armyName === "bh")
        {
            offset = Qt.point(30, 37);
        }
        sprite.loadSprite("artillery_shot",  false, sprite.getMaxUnitCount(), offset,
                          1, 1.0, 0, 0);
        sprite.loadSound("tank_shot.wav", 1, "resources/sounds/", 0);
    };

    this.getFireDurationMS = function()
    {
        // the time will be scaled with animation speed inside the engine
        return 500;
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ARTILLERY = new Constructor();
