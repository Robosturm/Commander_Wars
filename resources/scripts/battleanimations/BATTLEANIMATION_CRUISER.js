var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
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
        BATTLEANIMATION_CRUISER.loadSprite(sprite, unit, defender, weapon, Qt.point(0, 0), 0);
    };

    this.loadSprite = function(sprite, unit, defender, weapon, movement, moveTime)
    {
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_CRUISER.armyData);
        sprite.loadMovingSprite("cruiser+" + armyName,  false,
                                BATTLEANIMATION_CRUISER.getMaxUnitCount(), Qt.point(0, 20), movement, moveTime, false, -1);
        sprite.loadMovingSpriteV2("cruiser+" + armyName + "+mask", GameEnums.Recoloring_Table,
                                  BATTLEANIMATION_CRUISER.getMaxUnitCount(), Qt.point(0, 20), movement, moveTime, false, -1);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_CRUISER.loadStandingAnimation(sprite, unit, defender, weapon);
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_CRUISER.armyData);
        var offset = Qt.point(0, 0);
        if (weapon === 0)
        {
            // gun
            offset = Qt.point(58, 85);
            if (armyName === "yc")
            {
                offset = Qt.point(57, 87);
            }
            else if (armyName === "ge")
            {
                offset = Qt.point(58, 87);
            }
            else if (armyName === "ge")
            {
                offset = Qt.point(58, 50);
            }
            else if (armyName === "bm")
            {
                offset = Qt.point(58, 86);
            }
            else if (armyName === "bh")
            {
                offset = Qt.point(57, 85);
            }
            else if (armyName === "ma")
            {
                offset = Qt.point(70, 70);
            }

            var count = sprite.getUnitCount(5);
            for (var i = 0; i < count; i++)
            {
                var offset2 = Qt.point(0, 0);
                switch (i)
                {
                case 1:
                    offset2 = Qt.point(-5, -5);
                    break;
                case 2:
                    offset2 = Qt.point(-10, -10);
                    break;
                case 3:
                    offset2 = Qt.point(2, -7);
                    break;
                case 4:
                    offset2 = Qt.point(-3, -12);
                    break;
                }

                sprite.loadSingleMovingSprite("rocket_up", false,
                                              Qt.point(offset.x + offset2.x,
                                                       offset.y + offset2.y),
                                              Qt.point(80, 40), 400, false,
                                              1, 1.0, 5, 200 * i, false);
                sprite.loadSound("rocket_launch.wav", 1, "resources/sounds/", 200 * i);
            }
        }
        else
        {
            // mg
            offset = Qt.point(81, 75);
            if (armyName === "yc")
            {
                offset = Qt.point(82, 74);
            }
            else if (armyName === "ge")
            {
                offset = Qt.point(82, 74);
            }
            else if (armyName === "bm")
            {
                offset = Qt.point(81, 74);
            }
            else if (armyName === "bh")
            {
                offset = Qt.point(81, 74);
            }
            else if (armyName === "ma")
            {
                offset = Qt.point(93, 56);
            }
            sprite.loadSprite("mg_shot_air",  false, sprite.getMaxUnitCount(), offset,
                              1, 1, 0, 0, false, true);
        }
    };

    this.getFireDurationMS = function()
    {
        // the time will be scaled with animation speed inside the engine
        return 1500;
    };

    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadColorOverlayForLastLoadedFrame("#969696", 1000, 1, 300);
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(5);
        var i = 0;
        if (weapon === 0)
        {
            sprite.loadSprite("unit_explosion",  false, 5, Qt.point(0, 20),
                              1, 1.0, 0, 300);
            sprite.loadMovingSprite("rocket_down", false, 5, Qt.point(127, 90),
                                    Qt.point(-127, -60), 400, true,
                                    1, 1, 0, 0, true);
            for (i = 0; i < count; i++)
            {
                sprite.loadSound("rocket_flying.wav", 1, "resources/sounds/", 0);
                sprite.loadSound("impact_explosion.wav", 1, "resources/sounds/", 200 + i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
        else
        {
            sprite.loadSprite("mg_hit",  false, 5, Qt.point(0, 22),
                              1, 1.0, 0, 0);
            for (i = 0; i < count; i++)
            {
                sprite.loadSound("mg_impact.wav", 1, "resources/sounds/", i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        if (weapon === 0)
        {
            return 1500;
        }
        else
        {
            return 1000;
        }
    };

    this.getDyingDurationMS = function(sprite, unit, defender, weapon)
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
        BATTLEANIMATION_CRUISER.loadSprite(sprite, unit, defender, weapon, Qt.point(-140, 0), 1000);
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_CRUISER = new Constructor();
