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
        sprite.loadSprite("recon+" + armyName,  false,
                          BATTLEANIMATION_RECON.getMaxUnitCount(), Qt.point(-5, 5));
        sprite.loadSprite("recon+" + armyName + "+mask",  true,
                          BATTLEANIMATION_RECON.getMaxUnitCount(), Qt.point(-5, 5));
    };


    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadAnimation("loadStandingAnimation", unit);
        // mg
        var player = unit.getOwner();
        // get army name
        var armyName = player.getArmy().toLowerCase();
        if (armyName === "bg")
        {
            armyName = "bh"
        }
        var offset = Qt.point(35, 44);
        if (armyName === "yc")
        {
            offset = Qt.point(30, 40);
        }
        else if (armyName === "ge")
        {
            offset = Qt.point(39, 30);
        }
        else if (armyName === "bm")
        {
            offset = Qt.point(37, 40);
        }
        else if (armyName === "bh")
        {
            offset = Qt.point(39, 30);
        }
        sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), offset,
                          1, 1, 0, 0);
    };

    this.getFireDurationMS = function()
    {
        // the time will be scaled with animation speed inside the engine
        return 500;
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("mg_hit",  false, sprite.getMaxUnitCount(), Qt.point(0, 22),
                          1, 1.0, 0, 0);
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_RECON = new Constructor();
