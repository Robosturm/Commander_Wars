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
            (armyName !== "bh") &&
            (armyName !== "ma"))
        {
            armyName = "os";
        }
        sprite.loadSprite("lander+" + armyName,  false,
                          BATTLEANIMATION_LANDER.getMaxUnitCount(), Qt.point(0, 20));
        sprite.loadSpriteV2("lander+" + armyName + "+mask", GameEnums.Recoloring_Table,
                          BATTLEANIMATION_LANDER.getMaxUnitCount(), Qt.point(0, 20));
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_LANDER = new Constructor();
