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
                     ["ma", "ma"],];
    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_ARTILLERY.armyData);
        var offset = Qt.point(-5, 5);
        if (armyName === "ma")
        {
            offset = Qt.point(-35, 5);
        }
        sprite.loadSprite("artillery+" + armyName,  false,
                          BATTLEANIMATION_ARTILLERY.getMaxUnitCount(), offset);
        sprite.loadSpriteV2("artillery+" + armyName + "+mask", GameEnums.Recoloring_Table,
                            BATTLEANIMATION_ARTILLERY.getMaxUnitCount(), offset);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        // get army name
        var player = unit.getOwner();
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_ARTILLERY.armyData);
        var count = sprite.getUnitCount(BATTLEANIMATION_ARTILLERY.getMaxUnitCount());
        var offset = Qt.point(-5, 5);
        if (armyName === "ma")
        {
            offset = Qt.point(-35, 5);
        }
        sprite.loadSprite("artillery+" + armyName + "+fire",  false,
                          BATTLEANIMATION_ARTILLERY.getMaxUnitCount(), offset, 1);
        sprite.loadSpriteV2("artillery+" + armyName + "+fire+mask",  GameEnums.Recoloring_Table,
                            BATTLEANIMATION_ARTILLERY.getMaxUnitCount(), offset, 1);
        offset = Qt.point(30, 37);
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
        sprite.loadSprite("artillery_shot",  false, sprite.getMaxUnitCount(), offset,
                          1, 1.0, 0, 500);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("artillery_weapon_load.wav", 1, "resources/sounds/", i * BATTLEANIMATION.defaultFrameDelay);
            sprite.loadSound("cannon_weapon_fire.wav", 1, "resources/sounds/", 500 + i * BATTLEANIMATION.defaultFrameDelay);
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
        sprite.loadSprite("unit_explosion",  false, sprite.getMaxUnitCount(), Qt.point(0, 20),
                          1, 1.0, 0, 0);
        sprite.addSpriteScreenshake(8, 0.98, 800, 200);
        for (i = 0; i < count; i++)
        {
            sprite.loadSound("artillery_explode.wav", 1, "resources/sounds/", i * BATTLEANIMATION.defaultFrameDelay);
        }
    }
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ARTILLERY = new Constructor();
