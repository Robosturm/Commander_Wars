var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };

    this.armyData = [["os", "os"],
                     ["bh", "bh"],
                     ["bg", "bh"],
                     ["ma", "bh"],];

    this.animationData = [["os", [Qt.point(34, 26), Qt.point(-10, 5)]],
                          ["bh", [Qt.point(16, 18), Qt.point(-20, 5)]],];

    // -10 5
    // 44  26

    // 36  18

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        var player = unit.getOwner();
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_FLARE.armyData);
        var data = Global.getDataFromTable(armyName, BATTLEANIMATION_FLARE.animationData);
        var offset = data[1];
        sprite.loadSpriteV2("flare+" + armyName + "+mask", GameEnums.Recoloring_Matrix,
                            BATTLEANIMATION_FLARE.getMaxUnitCount(), offset);
        BATTLEANIMATION.loadSpotterOrCoMini(sprite, unit, false);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_FLARE.getMaxUnitCount());
        BATTLEANIMATION_FLARE.loadStandingAnimation(sprite, unit, defender, weapon);
        // mg
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_FLARE.armyData);
        var data = Global.getDataFromTable(armyName, BATTLEANIMATION_FLARE.animationData);
        var offset = data[0];
        sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), offset,
                          1, 1, 0, 0);
        BATTLEANIMATION.showMgBullets(sprite, offset);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("mg_weapon_fire.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
            sprite.loadSound("mg_weapon_fire.wav", 1, 200 + i * BATTLEANIMATION.defaultFrameDelay, 1, true);
            sprite.loadSound("mg_weapon_fire.wav", 1, 400 + i * BATTLEANIMATION.defaultFrameDelay, 1, true);
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        return 600 + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_FLARE.getMaxUnitCount());
    };

    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadColorOverlayForLastLoadedFrame("#969696", 300, 3, 0);
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_FLARE.getMaxUnitCount());
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
        return 400 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_FLAK.getMaxUnitCount());
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_FLARE = new Constructor();
