var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };

    this.armyData = [["os", "os"],
                     ["ge", "ge"],
                     ["ma", "ma"],
                     ["ti", "ti"],
                     ["yc", "yc"],];

    this.animationData = [["os", [Qt.point(5, 23),]],
                          ["ge", [Qt.point(0, 19),]],
                          ["ma", [Qt.point(22, 32),]],
                          ["ti", [Qt.point(18, 28),]],
                          ["yc", [Qt.point(-2, 23),]],];

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_DUSTER.armyData);
        var data = Global.getDataFromTable(armyName, BATTLEANIMATION_DUSTER.animationData);
        sprite.loadSpriteV2("duster+" + armyName + "+mask", GameEnums.Recoloring_Matrix,
                            BATTLEANIMATION_DUSTER.getMaxUnitCount(), Qt.point(-40, 20), -1, 1.0, 0, 0,
                            false, false, 30);
        sprite.addMoveTweenToLastLoadedSprites(0, -3, 1200);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_DUSTER.loadStandingAnimation(sprite, unit, defender, weapon);
        // mg
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_DUSTER.armyData);
        var data = Global.getDataFromTable(armyName, BATTLEANIMATION_DUSTER.animationData);
        var count = sprite.getUnitCount(BATTLEANIMATION_DUSTER.getMaxUnitCount());
        var offset = data[0];

        sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), offset,
                          1, 1, 0, 0);
        sprite.addMoveTweenToLastLoadedSprites(0, -3, 1200);
        BATTLEANIMATION.showMgBullets(sprite, offset);
        sprite.addMoveTweenToLastLoadedSprites(0, -3, 1200);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("mg_weapon_fire.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
            sprite.loadSound("mg_weapon_fire.wav", 1, 200 + i * BATTLEANIMATION.defaultFrameDelay, 1, true);
            sprite.loadSound("mg_weapon_fire.wav", 1, 400 + i * BATTLEANIMATION.defaultFrameDelay, 1, true);
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        return 600 + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_DUSTER.getMaxUnitCount());
    };

    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadColorOverlayForLastLoadedFrame("#969696", 300, 3, 0);
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_DUSTER.getMaxUnitCount());
        var yOffset = 22;
        if (unit.getUnitType()  === GameEnums.UnitType_Air)
        {
            yOffset = 40
        }
        sprite.loadSprite("mg_hit",  false, sprite.getMaxUnitCount(), Qt.point(0, yOffset),
                          1, 1.0, 0, 0, true);
        BATTLEANIMATION.playMgImpactSound(sprite, unit, defender, weapon, count);
    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        return 400 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_DUSTER.getMaxUnitCount());
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_DUSTER = new Constructor();
