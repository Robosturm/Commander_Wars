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
                     ["ma", "ma"],
                     ["ac", "ac"],];
    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_ARTILLERY.loadSprite(sprite, unit, defender, weapon, "", 0, 0);
    };

    this.loadStandingFiredAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_ARTILLERY.loadSprite(sprite, unit, defender, weapon, "", 1, 1);
    };

    this.loadSprite = function(sprite, unit, defender, weapon, ending, startFrame = 0, endFrame = -1)
    {
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_ARTILLERY.armyData);
        var offset = Qt.point(-5, 5);
        if (armyName === "ma")
        {
            offset = Qt.point(-35, 5);
        }
        sprite.loadSpriteV2("artillery+" + armyName + ending + "+mask", GameEnums.Recoloring_Matrix,
                            BATTLEANIMATION_ARTILLERY.getMaxUnitCount(), offset, 1, 1, 0, 0, false, false, 100, endFrame, startFrame);
        BATTLEANIMATION.loadSpotter(sprite, unit);
    };


    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        // get army name
        var player = unit.getOwner();
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_ARTILLERY.armyData);
        var count = sprite.getUnitCount(BATTLEANIMATION_ARTILLERY.getMaxUnitCount());
        BATTLEANIMATION_ARTILLERY.loadSprite(sprite, unit, defender, weapon, "+fire");
        var offset = Qt.point(30, 37);
        // gun
        if (armyName === "yc")
        {
            offset = Qt.point(30, 37);
        }
        else if (armyName === "ma")
        {
            offset = Qt.point(30, 57);
        }
        else if (armyName === "ge")
        {
            offset = Qt.point(30, 37);
        }
        else if (armyName === "bm")
        {
            offset = Qt.point(30, 37);
        }
        else if (armyName === "bh")
        {
            offset = Qt.point(30, 37);
        }
        else if (armyName === "ac")
        {
            offset = Qt.point(44, 35);
        }
        sprite.loadSprite("artillery_shot",  false, sprite.getMaxUnitCount(), offset,
                          1, 1.0, 0, 500);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("artillery_weapon_load.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
            sprite.loadSound("cannon_weapon_fire.wav", 1, 500 + i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 1000 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_ARTILLERY.getMaxUnitCount();
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
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ARTILLERY = new Constructor();
