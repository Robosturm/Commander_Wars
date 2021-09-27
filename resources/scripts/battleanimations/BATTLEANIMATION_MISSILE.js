var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };
    this.armyData = [["bd", "bd"],
                     ["bm", "bm"],
                     ["bh", "bh"],
                     ["bg", "bh"],
                     ["ge", "ge"],
                     ["ma", "ma"],
                     ["os", "os"],
                     ["pf", "pf"],
                     ["ti", "ti"],
                     ["yc", "yc"],];

    this.animationData = [["bd", [Qt.point(-10, 5), Qt.point(-1, 26), "rocket_launch+2", Qt.point(20, 10)]],
                          ["bm", [Qt.point(-10, 5), Qt.point(11, 35), "rocket_launch",   Qt.point(35, 22)]],
                          ["bh", [Qt.point(-10, 5), Qt.point(11, 35), "rocket_launch",   Qt.point(35, 22)]],
                          ["ge", [Qt.point(-10, 5), Qt.point(11, 35), "rocket_launch",   Qt.point(35, 22)]],
                          ["ma", [Qt.point(-35, 5), Qt.point(9, 36),  "rocket_launch",   Qt.point(35, 22)]],
                          ["os", [Qt.point(-10, 5), Qt.point(11, 35), "rocket_launch",   Qt.point(35, 22)]],
                          ["pf", [Qt.point(-10, 5), Qt.point(-1, 35), "rocket_launch+1", Qt.point(20, 10)]],
                          ["ti", [Qt.point(-10, 5), Qt.point(-4, 38), "rocket_launch+1", Qt.point(20, 10)]],
                          ["yc", [Qt.point(-10, 5), Qt.point(11, 35), "rocket_launch",   Qt.point(35, 22)]],];

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_MISSILE.loadSprite(sprite, unit, defender, weapon, "");
    };

    this.loadSprite = function(sprite, unit, defender, weapon, ending, startFrame = 0, endFrame = 0)
    {
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_MISSILE.armyData);
        var data = Global.getArmyDataFromTable(armyName, BATTLEANIMATION_MISSILE.animationData);
        var offset = data[0];
        sprite.loadSpriteV2("missile+" + armyName + ending + "+mask", GameEnums.Recoloring_Matrix,
                            BATTLEANIMATION_MISSILE.getMaxUnitCount(), offset,
                            1, 1, 0, 0, false, false, 200, endFrame, startFrame);
        BATTLEANIMATION.loadSpotter(sprite, unit);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_MISSILE.loadSprite(sprite, unit, defender, weapon, "+fire", 0, 1);
        var player = unit.getOwner();
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_MISSILE.armyData);
        var count = sprite.getUnitCount(BATTLEANIMATION_MISSILE.getMaxUnitCount());
        var data = Global.getArmyDataFromTable(armyName, BATTLEANIMATION_MISSILE.animationData);
        var offset = data[1];
        sprite.loadMovingSprite("rocket_up", false, sprite.getMaxUnitCount(), offset,
                                Qt.point(128, 64), 400, false,
                                -1, 1, 0);
        offset.x -= data[3].x;
        offset.y -= data[3].y;
        sprite.loadMovingSprite(data[2], false, sprite.getMaxUnitCount(), offset,
                                Qt.point(0, 0), 0, false, 1, 1, 0);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("rocket_launch.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        return 500 + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_MISSILE.getMaxUnitCount());
    };

    this.loadStandingFiredAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_MISSILE.loadSprite(sprite, unit, defender, weapon, "+fire", 1, 1);
    };

    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadColorOverlayForLastLoadedFrame("#969696", 1000, 1, 300);
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_MISSILE.getMaxUnitCount());
        sprite.loadMovingSprite("rocket_hit_air", false, sprite.getMaxUnitCount(), Qt.point(0, 20),
                                Qt.point(-30, 15), 400, false,
                                1, 1.0, 0, 300, true);
        sprite.addSpriteScreenshake(8, 0.95, 800, 500);
        sprite.loadMovingSprite("rocket_up", false, sprite.getMaxUnitCount(), Qt.point(127, 0),
                                Qt.point(-128, 64), 400, true,
                                -1, 1, 0, 0, true);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("rocket_flying.wav", 1, 0);
            sprite.loadSound("rockets_explode.wav", 1, 200 + i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        return 400 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_MISSILE.getMaxUnitCount());
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_MISSILE = new Constructor();
