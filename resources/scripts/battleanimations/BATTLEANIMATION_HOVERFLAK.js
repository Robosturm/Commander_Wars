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

    this.animationData = [["os", [Qt.point(-35, 5), 2, Qt.point(15, 45), Qt.point(15, 29), 1300]],
                          ["bm", [Qt.point(-35, 5), 2, Qt.point(13, 39), Qt.point(14, 25), 1300]],
                          ["ge", [Qt.point(-35, 5), 2, Qt.point(14, 41), Qt.point(16, 26), 1300]],
                          ["yc", [Qt.point(-35, 5), 1, Qt.point(19, 43), Qt.point(19, 27), 1200]],
                          ["ma", [Qt.point(-45, 5), 1, Qt.point(7, 38),  Qt.point(7, 33),  1200]],];

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_HOVERFLAK.loadSprite(sprite, unit, defender, weapon);
    };

    this.loadSprite = function(sprite, unit, defender, weapon, ending = "", loops = 1)
    {
        var player = unit.getOwner();
        var army = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_HOVERFLAK.armyData);
        var data = Global.getArmyDataFromTable(army, BATTLEANIMATION_HOVERFLAK.animationData);
        var airName = "";
        if (BATTLEANIMATION.getRelativePosition(unit, defender) > 0)
        {
            airName = "+air";
        }
        sprite.loadSpriteV2("hoverflak+" + army + ending + airName + "+mask", GameEnums.Recoloring_Matrix,
                            BATTLEANIMATION_HOVERFLAK.getMaxUnitCount(), data[0], loops);
        sprite.loadSpriteV2("hoverflak+" + army + "+prop+mask", GameEnums.Recoloring_Matrix,
                            BATTLEANIMATION_HOVERFLAK.getMaxUnitCount(), data[0],
                            -1, 1, 0, 0, false, false, 100);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_HOVERFLAK.getMaxUnitCount());
        BATTLEANIMATION_HOVERFLAK.loadSprite(sprite, unit, defender, weapon, "+fire", 3);
        var player = unit.getOwner();
        var army = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_HOVERFLAK.armyData);
        var data = Global.getArmyDataFromTable(army, BATTLEANIMATION_HOVERFLAK.animationData);
        var offset = data[2];
        var mgCount = data[1];
        if (BATTLEANIMATION.getRelativePosition(unit, defender) > 0)
        {
            for (var i2 = 0; i2 < mgCount; ++i2)
            {
                sprite.loadSprite("mg_shot_air",  false, sprite.getMaxUnitCount(),
                                  Qt.point(offset.x + i2 * 5, offset.y + i2 * 5),
                                  1, 1, 0, 100 * i2);
                sprite.loadSprite("mg_shot_air",  false, sprite.getMaxUnitCount(),
                                  Qt.point(offset.x + i2 * 5, offset.y + i2 * 5),
                                  1, 1, 0, 600 + 100 * i2);
            }
        }
        else
        {
            offset = data[3];
            for (var i2 = 0; i2 < mgCount; ++i2)
            {
                sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(),
                                  Qt.point(offset.x + i2 * 4, offset.y + i2 * 4),
                                  1, 1, 0, 100 * i2);
                sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(),
                                  Qt.point(offset.x + i2 * 4, offset.y + i2 * 4),
                                  1, 1, 0, 600 + 100 * i2);
            }
        }
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("vulcan_weapon_fire.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
            sprite.loadSound("vulcan_weapon_fire.wav", 1, 200 + i * BATTLEANIMATION.defaultFrameDelay);
            sprite.loadSound("vulcan_weapon_fire.wav", 1, 400 + i * BATTLEANIMATION.defaultFrameDelay);
            sprite.loadSound("vulcan_weapon_fire.wav", 1, 600 + i * BATTLEANIMATION.defaultFrameDelay);
            sprite.loadSound("vulcan_weapon_fire.wav", 1, 800 + i * BATTLEANIMATION.defaultFrameDelay);
            sprite.loadSound("vulcan_weapon_fire.wav", 1, 1000 + i * BATTLEANIMATION.defaultFrameDelay);
            if (mgCount === 2)
            {
                sprite.loadSound("vulcan_weapon_fire.wav", 1, 100 + i * BATTLEANIMATION.defaultFrameDelay);
                sprite.loadSound("vulcan_weapon_fire.wav", 1, 300 + i * BATTLEANIMATION.defaultFrameDelay);
                sprite.loadSound("vulcan_weapon_fire.wav", 1, 500 + i * BATTLEANIMATION.defaultFrameDelay);
                sprite.loadSound("vulcan_weapon_fire.wav", 1, 700 + i * BATTLEANIMATION.defaultFrameDelay);
                sprite.loadSound("vulcan_weapon_fire.wav", 1, 900 + i * BATTLEANIMATION.defaultFrameDelay);
                sprite.loadSound("vulcan_weapon_fire.wav", 1, 1100 + i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
    };

    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadColorOverlayForLastLoadedFrame("#969696", 300, 3, 0);
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_HOVERFLAK.getMaxUnitCount());
        var yOffset = 22;
        if (unit.getUnitType()  === GameEnums.UnitType_Air)
        {
            yOffset = 40
        }
        sprite.loadSprite("mg_hit",  false, sprite.getMaxUnitCount(), Qt.point(0, yOffset),
                          1, 1.0, 0, 0, true);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("mg_impact.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        return 400 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_HOVERFLAK.getMaxUnitCount());
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_FLAK.getMaxUnitCount());
        var player = unit.getOwner();
        var army = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_HOVERFLAK.armyData);
        var data = Global.getArmyDataFromTable(army, BATTLEANIMATION_HOVERFLAK.animationData);
        return data[4] + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_HOVERFLAK.getMaxUnitCount());
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_HOVERFLAK = new Constructor();
