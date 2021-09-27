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
                     ["bh", "ma"],
                     ["bg", "ma"],
                     ["ma", "ma"],
                     ["ac", "ma"],
                     ["pf", "ma"],
                     ["ti", "ma"],
                     ["dm", "ma"],];

    this.animationData = [["os", [Qt.point(-45, 5), Qt.point(28, 45)]],
                          ["bm", [Qt.point(-45, 5), Qt.point(9, 38)]],
                          ["ge", [Qt.point(-45, 5), Qt.point(14, 36)]],
                          ["yc", [Qt.point(-45, 5), Qt.point(24, 40)]],
                          ["ma", [Qt.point(-55, 5), Qt.point(-12, 42)]],];

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        var player = unit.getOwner();
        var army = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_ARTILLERYCRAFT.armyData);
        var data = Global.getArmyDataFromTable(army, BATTLEANIMATION_ARTILLERYCRAFT.animationData);
        sprite.loadSpriteV2("artillerycraft+" + army + "+mask", GameEnums.Recoloring_Matrix,
                            BATTLEANIMATION_ARTILLERYCRAFT.getMaxUnitCount(), data[0]);
        sprite.loadSpriteV2("artillerycraft+" + army + "+prop+mask", GameEnums.Recoloring_Matrix,
                            BATTLEANIMATION_ARTILLERYCRAFT.getMaxUnitCount(), data[0],
                            -1, 1, 0, 0, false, false, 100);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_ARTILLERYCRAFT.loadStandingAnimation(sprite, unit, defender, weapon);

        var player = unit.getOwner();
        var army = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_ARTILLERYCRAFT.armyData);
        var data = Global.getArmyDataFromTable(army, BATTLEANIMATION_ARTILLERYCRAFT.animationData);
        var offset = data[1];
        var count = sprite.getUnitCount(BATTLEANIMATION_ARTILLERYCRAFT.getMaxUnitCount());
        sprite.loadSprite("artillery_shot",  false, sprite.getMaxUnitCount(), offset,
                          1, 1.0, 0, 0);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("cannon_weapon_fire.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        return 500 + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_ARTILLERYCRAFT.getMaxUnitCount());
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION.getMaxUnitCount());
        var i = 0;
        sprite.loadSprite("artillery_hit",  false, sprite.getMaxUnitCount(), Qt.point(0, 20),
                          1, 1.0, 0, 0, true);
        sprite.addSpriteScreenshake(8, 0.98, 800, 200);
        for (i = 0; i < count; i++)
        {
            sprite.loadSound("artillery_explode.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
        }
    }

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        return 400 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_ARTILLERYCRAFT.getMaxUnitCount());
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ARTILLERYCRAFT = new Constructor();
