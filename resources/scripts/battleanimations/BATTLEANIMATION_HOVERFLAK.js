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
        BATTLEANIMATION_HOVERFLAK.loadSprite(sprite, unit, defender, weapon);
    };

    this.loadSprite = function(sprite, unit, defender, weapon, ending = "", loops = 1)
    {
        var player = unit.getOwner();
        var army = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_HOVERFLAK.armyData);
        if (army !== "")
        {
            var airName = "";
            if (BATTLEANIMATION.getRelativePosition(unit, defender) > 0)
            {
                airName = "+air";
            }
            sprite.loadSpriteV2("hoverflak+" + army + ending + airName + "+mask", GameEnums.Recoloring_Matrix,
                                BATTLEANIMATION_HOVERFLAK.getMaxUnitCount(), Qt.point(-35, 5), loops);
            sprite.loadSpriteV2("hoverflak+" + army + "+prop+mask", GameEnums.Recoloring_Matrix,
                                BATTLEANIMATION_HOVERFLAK.getMaxUnitCount(), Qt.point(-35, 5),
                                -1, 1, 0, 0, false, false, 100);
        }
        else
        {
            sprite.loadSpriteV2("hoverflak+mask", GameEnums.Recoloring_Matrix,
                                BATTLEANIMATION_HOVERFLAK.getMaxUnitCount(), Qt.point(-45, 5));
        }
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_HOVERFLAK.getMaxUnitCount());
        BATTLEANIMATION_HOVERFLAK.loadSprite(sprite, unit, defender, weapon, "+fire", 3);
        var player = unit.getOwner();
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_HOVERFLAK.armyData);
        var offset = Qt.point(7, 38);
        if (armyName !== "")
        {
            if (BATTLEANIMATION.getRelativePosition(unit, defender) > 0)
            {
                var mgCount = 2;
                if (armyName === "os")
                {
                    offset = Qt.point(15, 45);
                }
                else if (armyName === "bm")
                {
                    offset = Qt.point(13, 39);
                }
                else if (armyName === "ge")
                {
                    offset = Qt.point(14, 41);
                }
                else if (armyName === "yc")
                {
                    offset = Qt.point(19, 43);
                    mgCount = 1;
                }
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
                // -35,
                //  54 29
                var mgCount = 2;
                if (armyName === "os")
                {
                    offset = Qt.point(15, 29);
                }
                else if (armyName === "bm")
                {
                    offset = Qt.point(14, 25);
                }
                else if (armyName === "ge")
                {
                    offset = Qt.point(16, 26);
                }
                else if (armyName === "yc")
                {
                    offset = Qt.point(19, 27);
                    mgCount = 1;
                }
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
        }
        else
        {
            if (BATTLEANIMATION.getRelativePosition(unit, defender) > 0)
            {
                sprite.loadSprite("mg_shot_air",  false, sprite.getMaxUnitCount(), offset,
                                  1, 1, 0, 0);
            }
            else
            {
                offset = Qt.point(7, 33);
                sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), offset,
                                  1, 1, 0, 0);
            }
        }
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("vulcan_weapon_fire.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
            sprite.loadSound("vulcan_weapon_fire.wav", 1, 200 + i * BATTLEANIMATION.defaultFrameDelay);
            sprite.loadSound("vulcan_weapon_fire.wav", 1, 400 + i * BATTLEANIMATION.defaultFrameDelay);
            if (armyName !== "")
            {
                sprite.loadSound("vulcan_weapon_fire.wav", 1, 600 + i * BATTLEANIMATION.defaultFrameDelay);
                sprite.loadSound("vulcan_weapon_fire.wav", 1, 800 + i * BATTLEANIMATION.defaultFrameDelay);
                sprite.loadSound("vulcan_weapon_fire.wav", 1, 1000 + i * BATTLEANIMATION.defaultFrameDelay);
            }
            if (armyName !== "yc" &&
                armyName !== "")
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
        sprite.loadSprite("mg_hit",  false, sprite.getMaxUnitCount(), Qt.point(0, 22),
                          1, 1.0, 0, 0);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("mg_impact.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_FLAK.getMaxUnitCount());
        var player = unit.getOwner();
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_FLAK.armyData);
        if (armyName === "yc")
        {
            return 1200 + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_HOVERFLAK.getMaxUnitCount());
        }
        else if (armyName === "os" ||
                 armyName === "ge" ||
                 armyName === "bm")
        {
            return 1300 + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_HOVERFLAK.getMaxUnitCount());
        }
        else
        {
            return 600 + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_HOVERFLAK.getMaxUnitCount());
        }
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_HOVERFLAK = new Constructor();
