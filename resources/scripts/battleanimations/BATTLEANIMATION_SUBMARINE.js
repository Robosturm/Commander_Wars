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

    // todo load ship move sound
    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_SUBMARINE.loadSprite(sprite, unit, defender, weapon, Qt.point(-60, 20), Qt.point(0, 0), 0);
    };

    this.loadMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_SUBMARINE.loadSprite(sprite, unit, defender, weapon, Qt.point(0, 20), Qt.point(-60, 0), 1200);
    };

    this.loadSprite = function(sprite, unit, defender, weapon, startPos, movement, moveTime)
    {
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_SUBMARINE.armyData);
        if(unit.getHidden() === true &&
           armyName !== "ma")
        {
            sprite.loadMovingSpriteV2("submarine+hidden+" + armyName + "+mask", GameEnums.Recoloring_Matrix,
                                      BATTLEANIMATION_SUBMARINE.getMaxUnitCount(), startPos, movement, moveTime, false, -1);
            sprite.loadMovingSprite("submarine+hidden+" + armyName,  false,
                                    BATTLEANIMATION_SUBMARINE.getMaxUnitCount(), startPos, movement, moveTime, false, -1);
        }
        else
        {
            sprite.loadMovingSpriteV2("submarine+" + armyName + "+mask", GameEnums.Recoloring_Matrix,
                                      BATTLEANIMATION_SUBMARINE.getMaxUnitCount(), startPos, movement, moveTime, false, -1);
            sprite.loadMovingSprite("submarine+" + armyName,  false,
                                    BATTLEANIMATION_SUBMARINE.getMaxUnitCount(), startPos, movement, moveTime, false, -1);
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
                                              1, 1, 2, i * 150);
            }
            else
            {
                sprite.loadSingleMovingSprite("torpedo", false, Qt.point(90, 50),
                                              Qt.point(60, 0), 400, false,
                                              1, 0.5, -1, i * 150);
            }
            sprite.loadSound("torpedo_fire.wav", 1, i * 150);
        }
    };

    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(5);
        sprite.loadColorOverlayForLastLoadedFrame("#969696", 300, count, 300);
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(5);
        sprite.addSpriteScreenshake(8, 0.95, 800, 500);
        for (var i = 0; i < count; i++)
        {
            if (globals.isEven(i))
            {
                sprite.loadSingleMovingSprite("torpedo", false, Qt.point(127, 30),
                                              Qt.point(-70, 0), 400, true,
                                              1, 1, 2, i * 150, true);

                sprite.loadSingleMovingSprite("water_hit",  false, Qt.point(45, 30),
                                              Qt.point(0, 0), 0, false,
                                              1, 1.0, 2, 300 + i * 150);
            }
            else
            {
                sprite.loadSingleMovingSprite("torpedo", false, Qt.point(127, 50),
                                              Qt.point(-60, 0), 400, true,
                                              1, 0.5, -1, i * 150);
                sprite.loadSingleMovingSprite("water_hit",  false, Qt.point(57, 50),
                                              Qt.point(0, 0), 0, false,
                                              1, 0.5, -1, 300 + i * 150, true);
            }
            sprite.loadSound("torpedo_move.wav", 1);
            sprite.loadSound("impact_explosion.wav", 1, 300 + i * 150);
        }
    };

    this.hasMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        // return true if the unit has an implementation for loadMoveInAnimation
        return true;
    };
    this.getMoveInDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 1210;
    };

    this.hasDyingAnimation = function()
    {
        // return true if the unit has an implementation for loadDyingAnimation
        return true;
    };

    this.loadDyingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_SUBMARINE.loadSprite(sprite, unit, defender, weapon, Qt.point(-140, 0), 2000);
        sprite.loadSound("ship_dying_move.wav", -2);
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 1250;
    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        // should be a second or longer.
        // the time will be scaled with animation speed inside the engine
        return 1500;
    };

    this.getDyingDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 2200;
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_SUBMARINE = new Constructor();
