var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
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
        if(unit.getHidden() === true)
        {
            sprite.loadSprite("submarine+hidden+" + armyName,  false,
                              BATTLEANIMATION_SUBMARINE.getMaxUnitCount(), Qt.point(0, 20));
            sprite.loadSprite("submarine+hidden+" + armyName + "+mask",  true,
                              BATTLEANIMATION_SUBMARINE.getMaxUnitCount(), Qt.point(0, 20));
        }
        else
        {
            sprite.loadSprite("submarine+" + armyName,  false,
                              BATTLEANIMATION_SUBMARINE.getMaxUnitCount(), Qt.point(0, 20));
            sprite.loadSprite("submarine+" + armyName + "+mask",  true,
                              BATTLEANIMATION_SUBMARINE.getMaxUnitCount(), Qt.point(0, 20));
        }
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_SUBMARINE.loadStandingAnimation(sprite, unit, defender, weapon);
        var count = sprite.getUnitCount(5);
        for (var i = 0; i < count; i++)
        {
            if (globals.isEven(i))
            {
                sprite.loadSingleMovingSprite("torpedo", false, Qt.point(70, 30),
                                              Qt.point(60, 0), 400, false,
                                              1, 1, 1, i * 150);
            }
            else
            {
                sprite.loadSingleMovingSprite("torpedo", false, Qt.point(90, 50),
                                              Qt.point(60, 0), 400, false,
                                              1, 0.5, -1, i * 150);
            }
        }
    };

    this.getFireDurationMS = function()
    {
        // the time will be scaled with animation speed inside the engine
        return 1250;
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(5);
        for (var i = 0; i < count; i++)
        {
            if (globals.isEven(i))
            {
                sprite.loadSingleMovingSprite("torpedo", false, Qt.point(127, 30),
                                              Qt.point(-70, 0), 400, true,
                                              1, 1, 1, i * 150, true);

                sprite.loadSingleMovingSprite("unit_explosion",  false, Qt.point(45, 30),
                                              Qt.point(0, 0), 0, false,
                                              1, 1.0, 1, 300 + i * 150);
            }
            else
            {
                sprite.loadSingleMovingSprite("torpedo", false, Qt.point(127, 50),
                                              Qt.point(-60, 0), 400, true,
                                              1, 0.5, -1, i * 150);
                sprite.loadSingleMovingSprite("unit_explosion",  false, Qt.point(57, 50),
                                              Qt.point(0, 0), 0, false,
                                              1, 0.5, -1, 300 + i * 150, true);
            }
            sprite.loadSound("impact_explosion.wav", 1, "resources/sounds/", i * 100);
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
var BATTLEANIMATION_SUBMARINE = new Constructor();
