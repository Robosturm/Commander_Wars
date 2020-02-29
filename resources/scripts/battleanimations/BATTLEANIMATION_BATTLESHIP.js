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
        sprite.loadSprite("battleship+" + armyName,  false,
                          BATTLEANIMATION_BATTLESHIP.getMaxUnitCount(), Qt.point(0, 20));
        sprite.loadSprite("battleship+" + armyName + "+mask",  true,
                          BATTLEANIMATION_BATTLESHIP.getMaxUnitCount(), Qt.point(0, 20));
    };
    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_BATTLESHIP.loadStandingAnimation(sprite, unit, defender, weapon);
        var player = unit.getOwner();
        // get army name
        var armyName = player.getArmy().toLowerCase();
        if (armyName === "bg")
        {
            armyName = "bh"
        }
        var offset = Qt.point(70, 117);
        // gun
        if (armyName === "yc")
        {
            offset = Qt.point(65, 113);
        }
        else if (armyName === "ge")
        {
            offset = Qt.point(70, 117);
        }
        else if (armyName === "bm")
        {
            offset = Qt.point(70, 115);
        }
        else if (armyName === "bh")
        {
            offset = Qt.point(70, 115);
        }
        else if (armyName === "ma")
        {
            offset = Qt.point(54, 71);
        }
        var count = sprite.getUnitCount(5);
        for (var i = 0; i < count; i++)
        {
            var offset2 = Qt.point(0, 0);
            switch (i)
            {
            case 1:
                offset2 = Qt.point(-7, -6);
                break;
            case 2:
                offset2 = Qt.point(20, -18);
                break;
            case 3:
                offset2 = Qt.point(16, -22);
                break;
            case 4:
                offset2 = Qt.point(12, -26);
                break;
            }

            sprite.loadSingleMovingSprite("artillery_shot", false,
                                          Qt.point(offset.x + offset2.x,
                                                   offset.y + offset2.y),
                                          Qt.point(0, 0), 0, true,
                                          1, 1.0, 5, 200 * i, false);
            sprite.loadSound("tank_shot.wav", 1, "resources/sounds/", i * 200);
        }
    };

    this.getFireDurationMS = function()
    {
        // the time will be scaled with animation speed inside the engine
        return 1500;
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("unit_explosion",  false, 5, Qt.point(0, 20),
                          1, 1.0, 0, 0);
        sprite.loadSound("impact_explosion.wav", 1, "resources/sounds/", 0);
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_BATTLESHIP = new Constructor();
