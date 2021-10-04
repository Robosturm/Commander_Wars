var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };
    this.armyData = [["ac", "ac"],
                     ["bd", "bd"],
                     ["bh", "bh"],
                     ["bg", "bh"],
                     ["bm", "bm"],
                     ["dm", "dm"],
                     ["ge", "ge"],
                     ["ma", "ma"],
                     ["os", "os"],
                     ["ti", "ti"],
                     ["pf", "pf"],
                     ["yc", "yc"],];

    this.animationData = [["ac", [Qt.point(-10, 5), Qt.point(5, 38),   "rocket_launch",   Qt.point(35, 22)]],
                          ["bd", [Qt.point(-10, 5), Qt.point(-2, 30),  "rocket_launch+2", Qt.point(20, 10)]],
                          ["bh", [Qt.point(-10, 5), Qt.point(0, 33),   "rocket_launch",   Qt.point(35, 22)]],
                          ["bm", [Qt.point(-10, 5), Qt.point(0, 32),   "rocket_launch",   Qt.point(35, 22)]],
                          ["dm", [Qt.point(-10, 5), Qt.point(5, 37),   "rocket_launch",   Qt.point(35, 22)]],
                          ["ge", [Qt.point(-10, 5), Qt.point(0, 24),   "rocket_launch",   Qt.point(35, 22)]],
                          ["ma", [Qt.point(-25, 5), Qt.point(-2, 36),  "rocket_launch",   Qt.point(35, 22)]],
                          ["os", [Qt.point(-10, 5), Qt.point(0, 31),   "rocket_launch",   Qt.point(35, 22)]],
                          ["ti", [Qt.point(-20, 5), Qt.point(-15, 31), "rocket_launch+1", Qt.point(20, 10)]],
                          ["pf", [Qt.point(-10, 5), Qt.point(7, 45),   "rocket_launch+1", Qt.point(20, 10)]],
                          ["yc", [Qt.point(-10, 5), Qt.point(0, 32),   "rocket_launch",   Qt.point(35, 22)]],];

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_ROCKETTHROWER.loadSprite(sprite, unit, defender, weapon, "");
    };

    this.loadSprite = function(sprite, unit, defender, weapon, ending, startFrame = 0)
    {
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_ROCKETTHROWER.armyData);
        var data = Global.getArmyDataFromTable(armyName, BATTLEANIMATION_ROCKETTHROWER.animationData);
        var offset = data[0];
        sprite.loadSpriteV2("rocketthrower+" + armyName + ending + "+mask", GameEnums.Recoloring_Matrix,
                            BATTLEANIMATION_ROCKETTHROWER.getMaxUnitCount(), offset, 1, 1, 0, 0, false, false, 100, -1, startFrame);
        BATTLEANIMATION.loadSpotter(sprite, unit);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        var player = unit.getOwner();
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_ROCKETTHROWER.armyData);
        BATTLEANIMATION_ROCKETTHROWER.loadSprite(sprite, unit, defender, weapon, "+fire");
        var data = Global.getArmyDataFromTable(armyName, BATTLEANIMATION_ROCKETTHROWER.animationData);
        var count = sprite.getUnitCount(BATTLEANIMATION_ROCKETTHROWER.getMaxUnitCount());
        sprite.loadMovingSprite("rocket_up", false, sprite.getMaxUnitCount(), data[1],
                                Qt.point(128, 64), 400, true,
                                -1, 1, 0);
        sprite.loadMovingSprite(data[2], false, sprite.getMaxUnitCount(), Qt.point(data[1].x - data[3].x, data[1].y - data[3].y),
                                Qt.point(0, 0), 0, false, 1, 1, 0);
        sprite.addSpriteShakeY(4, 0.5, 0.0, 75, 0, count);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("rocket_launch.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        return 500 + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_ROCKETTHROWER.getMaxUnitCount());
    };

    this.loadStandingFiredAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_ROCKETTHROWER.loadSprite(sprite, unit, defender, weapon, "+fire", 1);
    };

    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadColorOverlayForLastLoadedFrame("#969696", 1000, 1, 300);
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_ROCKETTHROWER.getMaxUnitCount());
        sprite.loadSprite("rocket_hit",  false, sprite.getMaxUnitCount(), Qt.point(0, 20),
                          1, 1.0, 0, 400, true);
        sprite.addSpriteScreenshake(8, 0.95, 800, 500);
        sprite.loadMovingSprite("rocket_down", false, sprite.getMaxUnitCount(), Qt.point(127, 80),
                                Qt.point(-128, -64), 400, true,
                                -1, 1, 0, 0, true);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("rocket_flying.wav", 1, 0);
            sprite.loadSound("rockets_explode.wav", 1, 200 + i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        return 400 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_ROCKETTHROWER.getMaxUnitCount());
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ROCKETTHROWER = new Constructor();
