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

    this.loadMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        var player = unit.getOwner();
        var movement = Qt.point(-120, 0);
        var moveTime = 1200;
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_BATTLESHIP.armyData);
        var offset = Qt.point(20, 20);
        if (armyName === "ma")
        {
            offset.x = 0;
            movement = Qt.point(0, 0);
            moveTime = 0;
        }
        sprite.loadMovingSprite("battleship+" + armyName,  false,
                                BATTLEANIMATION_BATTLESHIP.getMaxUnitCount(), offset, movement, moveTime, false, -1);
        sprite.loadMovingSpriteV2("battleship+" + armyName + "+mask", GameEnums.Recoloring_Matrix,
                                  BATTLEANIMATION_BATTLESHIP.getMaxUnitCount(), offset, movement, moveTime, false, -1);
        if (armyName !== "ma")
        {
            var frameTime = 200;
            sprite.loadMovingSpriteV2("battleship+" + armyName + "+fire+mask", GameEnums.Recoloring_Matrix,
                                      BATTLEANIMATION_BATTLESHIP.getMaxUnitCount(), Qt.point(offset.x + 160, offset.y + 64),
                                      movement, moveTime, false, 1, 1.0, 0, 0, false, frameTime, 0);
        }
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_BATTLESHIP.baseStandingAnimation(sprite, unit, defender, weapon, 0, Qt.point(0, 0), 0);
    };

    this.baseStandingAnimation = function(sprite, unit, defender, weapon, fireFrames, movement, moveTime)
    {
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_BATTLESHIP.armyData);
        var offset = Qt.point(-100, 20);
        if (armyName === "ma")
        {
            offset.x = 0;
        }
        sprite.loadMovingSprite("battleship+" + armyName,  false,
                                BATTLEANIMATION_BATTLESHIP.getMaxUnitCount(), offset, movement, moveTime, false, -1);
        sprite.loadMovingSpriteV2("battleship+" + armyName + "+mask", GameEnums.Recoloring_Matrix,
                                  BATTLEANIMATION_BATTLESHIP.getMaxUnitCount(), offset, movement, moveTime, false, -1);
        if (armyName !== "ma")
        {
            var frameTime = 200;
            sprite.loadMovingSpriteV2("battleship+" + armyName + "+fire+mask", GameEnums.Recoloring_Matrix,
                                      BATTLEANIMATION_BATTLESHIP.getMaxUnitCount(), Qt.point(offset.x + 160, offset.y + 64),
                                      movement, moveTime, false, 1, 1.0, 0, 0, false, frameTime, fireFrames);
        }
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(5);
        BATTLEANIMATION_BATTLESHIP.baseStandingAnimation(sprite, unit, defender, weapon, count, Qt.point(0, 0), 0);
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_BATTLESHIP.armyData);
        var offset = Qt.point(88, 115);
        // gun
        if (armyName === "ma")
        {
            offset = Qt.point(54, 71);
        }
        for (var i = 0; i < count; i++)
        {
            var offset2 = Qt.point(0, 0);
            switch (i)
            {
            case 1:
                offset2 = Qt.point(-8, -7);
                break;
            case 2:
                offset2 = Qt.point(20, -18);
                break;
            case 3:
                offset2 = Qt.point(18, -23);
                break;
            case 4:
                offset2 = Qt.point(13, -27);
                break;
            }

            sprite.loadSingleMovingSprite("battleship_shot", false,
                                          Qt.point(offset.x + offset2.x,
                                                   offset.y + offset2.y),
                                          Qt.point(0, 0), 0, true,
                                          1, 1.0, 5, 200 * i);
            sprite.loadSound("cannon_weapon_fire.wav", 1, i * 200);
        }
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(5);
        sprite.loadSprite("artillery_heavy_hit",  false, 5, Qt.point(-16, 20),
                          1, 1.0, 0, 0);
        sprite.addSpriteScreenshake(8, 0.95, 800, 200);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("impact_explosion.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.hasMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        return true;
    };
    this.getMoveInDurationMS = function(sprite, unit, defender, weapon)
    {
        return 1210;
    };

    this.hasDyingAnimation = function()
    {
        return true;
    };

    this.loadDyingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_BATTLESHIP.baseStandingAnimation(sprite, unit, defender, weapon, 0, Qt.point(-140, 0), 2000);
        sprite.loadSound("ship_dying_move.wav", -2);
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        return 1500;
    };

    this.getDyingDurationMS = function(sprite, unit, defender, weapon)
    {
        return 2200;
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_BATTLESHIP = new Constructor();
