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
        sprite.loadSpriteV2("t_heli+" + armyName + "+mask", GameEnums.Recoloring_Table,
                          BATTLEANIMATION_T_HELI.getMaxUnitCount(), Qt.point(0, 40));
        sprite.loadSprite("t_heli+" + armyName,  false,
                          BATTLEANIMATION_T_HELI.getMaxUnitCount(), Qt.point(0, 40), -1, 1.0, 0, 0,
                          false, false, 50);
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_T_HELI = new Constructor();
