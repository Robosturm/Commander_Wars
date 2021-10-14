var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };

    this.armyData = [["ac", ""],
                     ["bd", ""],
                     ["bh", ""],
                     ["bg", ""],
                     ["bm", "bm"],
                     ["ge", ""],
                     ["ma", ""],
                     ["os", "os"],
                     ["pf", ""],
                     ["ti", ""],
                     ["yc", ""],];

    this.animationData = [["os", [Qt.point(10, 21)]],
                          ["bm", [Qt.point(11, 22)]],
                          ["",   [Qt.point(24, 19)]],];

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_MOTORBIKE.armyData);
        sprite.loadSpriteV2("motorbike+" + armyName + "+mask", GameEnums.Recoloring_Matrix,
                            BATTLEANIMATION_MOTORBIKE.getMaxUnitCount(), Qt.point(-15, 5));
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_MOTORBIKE.getMaxUnitCount());
        BATTLEANIMATION_MOTORBIKE.loadStandingAnimation(sprite, unit, defender, weapon);
        var player = unit.getOwner();
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_MOTORBIKE.armyData);
        var data = Global.getDataFromTable(armyName, BATTLEANIMATION_MOTORBIKE.animationData);
        var offset = data[0];
        if (BATTLEANIMATION.getRelativePosition(unit, defender) > 0)
        {
            sprite.loadSprite("mg_shot_air", false, sprite.getMaxUnitCount(), Qt.point(offset.x, offset.y + 5),
                              1, 1, 0, 0);
        }
        else
        {

            sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), offset,
                              1, 1, 0, 0);
        }

        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("mg_weapon_fire.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
            sprite.loadSound("mg_weapon_fire.wav", 1, 200 + i * BATTLEANIMATION.defaultFrameDelay);
            sprite.loadSound("mg_weapon_fire.wav", 1, 400 + i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        return 600 + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_MOTORBIKE.getMaxUnitCount());
    };

    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadColorOverlayForLastLoadedFrame("#969696", 300, 3, 0);
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_MOTORBIKE.getMaxUnitCount());
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
        return 400 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_MOTORBIKE.getMaxUnitCount());
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_MOTORBIKE = new Constructor();
