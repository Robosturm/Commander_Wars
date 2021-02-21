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
        BATTLEANIMATION_BATTLESHIP.baseStandingAnimation(sprite, unit, defender, weapon, 0, Qt.point(0, 0), 0);
    };

    this.baseStandingAnimation = function(sprite, unit, defender, weapon, fireFrames, movement, moveTime)
    {
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_BATTLESHIP.armyData);
        sprite.loadMovingSprite("battleship+" + armyName,  false,
                          BATTLEANIMATION_BATTLESHIP.getMaxUnitCount(), Qt.point(0, 20), movement, moveTime, false, -1);
        sprite.loadMovingSpriteV2("battleship+" + armyName + "+mask", GameEnums.Recoloring_Table,
                            BATTLEANIMATION_BATTLESHIP.getMaxUnitCount(), Qt.point(0, 20), movement, moveTime, false, -1);
        if (armyName !== "ma")
        {
            sprite.loadMovingSprite("battleship+" + armyName + "+fire",  false,
                              BATTLEANIMATION_BATTLESHIP.getMaxUnitCount(), Qt.point(47, 20 + 64),
                              movement, moveTime, false, 1, 1.0, 0, 0, false, map.getFrameTime() , fireFrames);
            sprite.loadMovingSpriteV2("battleship+" + armyName + "+fire+mask", GameEnums.Recoloring_Table,
                              BATTLEANIMATION_BATTLESHIP.getMaxUnitCount(), Qt.point(47, 20 + 64),
                              movement, moveTime, false, 1, 1.0, 0, 0, false, map.getFrameTime() , fireFrames);
        }
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(5);
        BATTLEANIMATION_BATTLESHIP.baseStandingAnimation(sprite, unit, defender, weapon, count, Qt.point(0, 0), 0);
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_BATTLESHIP.armyData);
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

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("unit_explosion",  false, 5, Qt.point(0, 20),
                          1, 1.0, 0, 0);
        sprite.loadSound("impact_explosion.wav", 1, "resources/sounds/", 0);
    };

    this.hasDyingAnimation = function()
    {
        // return true if the unit has an implementation for loadDyingAnimation
        return true;
    };

    this.loadDyingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_BATTLESHIP.baseStandingAnimation(sprite, unit, defender, weapon, 0, Qt.point(-140, 0), 600);
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 1500;
    };

    this.getDyingDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 1000;
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_BATTLESHIP = new Constructor();
