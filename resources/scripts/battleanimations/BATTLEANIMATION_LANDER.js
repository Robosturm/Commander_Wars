var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_LANDER.loadSprite(sprite, unit, defender, weapon, Qt.point(0, 0), 0);
    };

    this.loadSprite = function(sprite, unit, defender, weapon, movement, moveTime)
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

        if (armyName === "ma")
        {
            sprite.loadMovingSprite("lander+" + armyName,  false,
                              BATTLEANIMATION_LANDER.getMaxUnitCount(), Qt.point(0, 20), movement, moveTime, false, -1);
            sprite.loadMovingSpriteV2("lander+" + armyName + "+mask", GameEnums.Recoloring_Table,
                              BATTLEANIMATION_LANDER.getMaxUnitCount(), Qt.point(0, 20), movement, moveTime, false, -1);
        }
        else
        {
            sprite.loadMovingSprite("lander+" + armyName,  false,
                              BATTLEANIMATION_LANDER.getMaxUnitCount(), Qt.point(0, 20), movement, moveTime, false, -1);
            sprite.loadMovingSpriteV2("lander+" + armyName + "+mask", GameEnums.Recoloring_Table,
                              BATTLEANIMATION_LANDER.getMaxUnitCount(), Qt.point(0, 20), movement, moveTime, false, -1);
        }
    };

    this.getDyingDurationMS = function()
    {
        // the time will be scaled with animation speed inside the engine
        return 1200;
    };

    this.hasDyingAnimation = function()
    {
        // return true if the unit has an implementation for loadDyingAnimation
        return true;
    };

    this.loadDyingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_LANDER.loadSprite(sprite, unit, defender, weapon, Qt.point(-140, 0), 1000);
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_LANDER = new Constructor();
