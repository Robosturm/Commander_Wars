var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("hoverflak",  false,
                          BATTLEANIMATION_HOVERFLAK.getMaxUnitCount(), Qt.point(-45, 5));
        sprite.loadSpriteV2("hoverflak+mask", GameEnums.Recoloring_Table,
                            BATTLEANIMATION_HOVERFLAK.getMaxUnitCount(), Qt.point(-45, 5));
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_HOVERFLAK.getMaxUnitCount());
        BATTLEANIMATION_HOVERFLAK.loadStandingAnimation(sprite, unit, defender, weapon);
        var offset = Qt.point(7, 38);
        if (defender.getUnitType() === GameEnums.UnitType_Air)
        {
            sprite.loadSprite("mg_shot_air",  false, sprite.getMaxUnitCount(), offset,
                              1, 1, 0, 0);
        }
        else
        {
            offset = Qt.point(7, 34);
            sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), offset,
                              1, 1, 0, 0);
        }
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("vulcan_weapon_fire.wav", 1, "resources/sounds/", i * BATTLEANIMATION.defaultFrameDelay);
            sprite.loadSound("vulcan_weapon_fire.wav", 1, "resources/sounds/", 200 + i * BATTLEANIMATION.defaultFrameDelay);
            sprite.loadSound("vulcan_weapon_fire.wav", 1, "resources/sounds/", 400 + i * BATTLEANIMATION.defaultFrameDelay);
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
            sprite.loadSound("mg_impact.wav", 1, "resources/sounds/", i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 500 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_HOVERFLAK.getMaxUnitCount();
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_HOVERFLAK = new Constructor();
