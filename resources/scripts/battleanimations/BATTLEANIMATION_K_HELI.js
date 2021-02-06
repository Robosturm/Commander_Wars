var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };
    this.armyData = [["os", "os"],
                     ["bm", "bm"],
                     ["ge", "ge"],
                     ["yc", "yc"],
                     ["bh", "bh"],
                     ["bg", "bh"],
                     ["ma", "ma"],];
    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_K_HELI.armyData);
        var offset = Qt.point(-40, 30);
        if (armyName === "ma")
        {
            offset = Qt.point(-30, 20);
        }
        sprite.loadSpriteV2("k_heli+" + armyName + "+mask", GameEnums.Recoloring_Table,
                          BATTLEANIMATION_K_HELI.getMaxUnitCount(), offset);
        sprite.loadSprite("k_heli+" + armyName,  false,
                          BATTLEANIMATION_K_HELI.getMaxUnitCount(), offset, -1, 1.0, 0, 0,
                          false, false, 30);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_K_HELI.loadStandingAnimation(sprite, unit, defender, weapon);
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_K_HELI.armyData);
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
            sprite.loadSound("rocket_launch.wav", 1, "resources/sounds/", 0);
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

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        if (weapon === 0)
        {
            sprite.loadSprite("unit_explosion",  false, sprite.getMaxUnitCount(), Qt.point(0, 20),
                              1, 1.0, 0, 300);
            sprite.loadMovingSprite("rocket", false, sprite.getMaxUnitCount(), Qt.point(127, 90),
                                    Qt.point(-127, -60), 400, true,
                                    1, 1, 0, 0, true);
            sprite.loadSound("rocket_flying.wav", 1, "resources/sounds/", 0);
            sprite.loadSound("impact_explosion.wav", 1, "resources/sounds/", 200);
        }
        else
        {
            sprite.loadSprite("mg_hit",  false, sprite.getMaxUnitCount(), Qt.point(0, 22),
                              1, 1.0, 0, 0);
            sprite.loadSound("mg_impact.wav", 1, "resources/sounds/", 0);
        }
    };

    this.getImpactDurationMS = function()
    {
        // should be a second or longer.
        // the time will be scaled with animation speed inside the engine
        return 1500 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_K_HELI.getMaxUnitCount();
    };

    this.getFireDurationMS = function()
    {
        // the time will be scaled with animation speed inside the engine
        return 500 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_K_HELI.getMaxUnitCount();
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_K_HELI = new Constructor();
