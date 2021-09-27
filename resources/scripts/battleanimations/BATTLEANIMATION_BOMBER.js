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
    this.animationData = [["os", [20, Qt.point(-85, -1), 850, Qt.point(70, 60)]],
                          ["bm", [20, Qt.point(-85, -1), 850, Qt.point(70, 60)]],
                          ["ge", [20, Qt.point(-85, -1), 850, Qt.point(70, 60)]],
                          ["yc", [20, Qt.point(-85, -1), 850, Qt.point(70, 60)]],
                          ["bh", [20, Qt.point(-85, -1), 850, Qt.point(70, 60)]],
                          ["ma", [3,  Qt.point(-85, -1), 850, Qt.point(75, 115)]],];

    this.loadMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.setBackgroundSpeed(sprite.getBackgroundSpeed() + 1);
        // get army name
        var player = unit.getOwner();
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_BOMBER.armyData);
        var data = Global.getArmyDataFromTable(armyName, BATTLEANIMATION_BOMBER.animationData);
        var offset = Qt.point(data[0], 40);
        var movement = data[1];
        var moveTime = data[2];
        sprite.loadMovingSpriteV2("bomber+" + armyName + "+mask", GameEnums.Recoloring_Matrix,
                                  BATTLEANIMATION_BOMBER.getMaxUnitCount(), offset, movement, moveTime);
    };

    this.getStopDurationMS = function(sprite, unit, defender, weapon)
    {
        return 0;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon, alive = true)
    {
        BATTLEANIMATION_BOMBER.loadSprite(sprite, unit, defender, weapon, Qt.point(0, 0), 0);
        if (alive)
        {
            sprite.addMoveTweenToLastLoadedSprites(0, -3, 1200);
        }
    };

    this.loadSprite = function(sprite, unit, defender, weapon, movement, moveTime)
    {
        // get army name
        var player = unit.getOwner();        
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_BOMBER.armyData);
        var data = Global.getArmyDataFromTable(armyName, BATTLEANIMATION_BOMBER.animationData);
        var offset = Qt.point(data[0] + data[1].x, 40);
        sprite.loadMovingSpriteV2("bomber+" + armyName + "+mask", GameEnums.Recoloring_Matrix,
                                  BATTLEANIMATION_BOMBER.getMaxUnitCount(), offset, movement, moveTime);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.restoreBackgroundSpeed();
        // get army name
        var player = unit.getOwner();
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_BOMBER.armyData);
        BATTLEANIMATION_BOMBER.loadStandingAnimation(sprite, unit, defender, weapon);
        var data = Global.getArmyDataFromTable(armyName, BATTLEANIMATION_BOMBER.animationData);
        var count = sprite.getUnitCount(5);
        var startPoint = data[3];
        for (var i = 0; i < count; i++)
        {
            sprite.loadSingleMovingSprite("bombs_projectile", false, startPoint,
                                          Qt.point(0, -160), 800, false,
                                          1, 1, -1, i * 150);
            sprite.loadSound("falling_bomb.wav", 1, i * 150);
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        return 800 + 150 * sprite.getUnitCount(5);
    };

    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadColorOverlayForLastLoadedFrame("#969696", 1000, 1, 300);
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(5);
        sprite.loadSprite("cannon_heavy_hit",  false, 5, Qt.point(0, 20),
                          1, 1.0, 0, 300, true);
        sprite.addSpriteScreenshake(8, 0.95, 800, 400);
        sprite.loadMovingSprite("bomb_falling", false, 5, Qt.point(0, 150),
                                Qt.point(0, -130), 400, true,
                                1, 1, 0, 0, true);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("impact_explosion.wav", 1, 300 + i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        return 400 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(5);
    };

    this.hasMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        return true;
    };
    this.getMoveInDurationMS = function(sprite, unit, defender, weapon)
    {
        return 860;
    };

    this.getDyingDurationMS = function(sprite, unit, defender, weapon)
    {
        return 2000;
    };

    this.hasDyingAnimation = function()
    {
        return true;
    };

    this.loadDyingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_BOMBER.loadSprite(sprite, unit, defender, weapon, Qt.point(-140, -140), 1800, false);
        sprite.loadSound("airunit_dying.wav", 1);
    };

};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_BOMBER = new Constructor();
