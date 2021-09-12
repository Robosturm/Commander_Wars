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
                     ["bh", ""],
                     ["bg", ""],
                     ["ma", ""],
                     ["ac", ""],
                     ["pf", ""],
                     ["ti", ""],
                     ["dm", ""],];

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        var player = unit.getOwner();
        var army = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_ARTILLERYCRAFT.armyData);
        if (army !== "")
        {
            sprite.loadSpriteV2("artillerycraft+" + army + "+mask", GameEnums.Recoloring_Matrix,
                                BATTLEANIMATION_ARTILLERYCRAFT.getMaxUnitCount(), Qt.point(-45, 5));
            sprite.loadSpriteV2("artillerycraft+" + army + "+prop+mask", GameEnums.Recoloring_Matrix,
                                BATTLEANIMATION_ARTILLERYCRAFT.getMaxUnitCount(), Qt.point(-45, 5),
                                -1, 1, 0, 0, false, false, 100);
        }
        else
        {
            sprite.loadSpriteV2("artillerycraft+mask", GameEnums.Recoloring_Matrix,
                                BATTLEANIMATION_ARTILLERYCRAFT.getMaxUnitCount(), Qt.point(-55, 5));
        }
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_ARTILLERYCRAFT.loadStandingAnimation(sprite, unit, defender, weapon);
        var offset = Qt.point(-12, 42);
        var player = unit.getOwner();
        var army = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_ARTILLERYCRAFT.armyData);
        if (army === "os")
        {
            offset = Qt.point(28, 45);
        }
        else if (army === "ge")
        {
            offset = Qt.point(14, 36);
        }
        else if (army === "bm")
        {
            offset = Qt.point(9, 38);
        }
        else if (army === "yc")
        {
            offset = Qt.point(24, 40);
        }
        var count = sprite.getUnitCount(BATTLEANIMATION_ARTILLERYCRAFT.getMaxUnitCount());
        sprite.loadSprite("artillery_shot",  false, sprite.getMaxUnitCount(), offset,
                          1, 1.0, 0, 0);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("cannon_weapon_fire.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION.getMaxUnitCount());
        var i = 0;
        sprite.loadSprite("artillery_hit",  false, sprite.getMaxUnitCount(), Qt.point(0, 20),
                          1, 1.0, 0, 0);
        sprite.addSpriteScreenshake(8, 0.98, 800, 200);
        for (i = 0; i < count; i++)
        {
            sprite.loadSound("artillery_explode.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
        }
    }

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 500 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_ARTILLERYCRAFT.getMaxUnitCount();
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ARTILLERYCRAFT = new Constructor();
