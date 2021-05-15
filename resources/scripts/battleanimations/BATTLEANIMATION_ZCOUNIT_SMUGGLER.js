var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };

    this.loadMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_SMUGGLER.getMaxUnitCount());
        sprite.loadMovingSprite("smuggler", false, sprite.getMaxUnitCount(), Qt.point(-80, 5),
                                Qt.point(65, 0), 600, false, 1, 1);
        sprite.loadMovingSpriteV2("smuggler+mask", GameEnums.Recoloring_Table, sprite.getMaxUnitCount(), Qt.point(-80, 5),
                                  Qt.point(65, 0), 600, false, 1, 1);
        sprite.loadMovingSprite("vehicle_dust", false, sprite.getMaxUnitCount(), Qt.point(-100, 7),
                                Qt.point(65, 0), 600, false, 1, 1);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("recon_move.wav", 5, "resources/sounds/", i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.loadStopAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_ZCOUNIT_SMUGGLER.loadSprite(sprite, unit, defender, 1);
        sprite.loadSprite("vehicle_dust_stop",  false,
                          BATTLEANIMATION_ZCOUNIT_SMUGGLER.getMaxUnitCount(), Qt.point(-50, 7), 1);
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_ZCOUNIT_SMUGGLER.loadSprite(sprite, unit, defender, 1);
    };

    this.loadSprite = function(sprite, unit, defender, count)
    {
        var offset = Qt.point(-15, 5);
        sprite.loadSprite("smuggler",  false,
                          BATTLEANIMATION_ZCOUNIT_SMUGGLER.getMaxUnitCount(), offset, count);
        sprite.loadSpriteV2("smuggler+mask", GameEnums.Recoloring_Table,
                            BATTLEANIMATION_ZCOUNIT_SMUGGLER.getMaxUnitCount(), offset, count);
    };


    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_SMUGGLER.getMaxUnitCount());
        BATTLEANIMATION_ZCOUNIT_SMUGGLER.loadSprite(sprite, unit, defender, 1);

        var offset = Qt.point(22, 37);
        // -15 5
        //  0 -5
        //  37 37
        sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), offset,
                          1, 1, 0, 0);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("mg_weapon_fire.wav", 1, "resources/sounds/", i * BATTLEANIMATION.defaultFrameDelay);
            sprite.loadSound("mg_weapon_fire.wav", 1, "resources/sounds/", 200 + i * BATTLEANIMATION.defaultFrameDelay);
            sprite.loadSound("mg_weapon_fire.wav", 1, "resources/sounds/", 400 + i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.hasMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        // return true if the unit has an implementation for loadMoveInAnimation
        return true;
    };

    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadColorOverlayForLastLoadedFrame("#969696", 300, 3, 0);
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_SMUGGLER.getMaxUnitCount());
        sprite.loadSprite("mg_hit",  false, sprite.getMaxUnitCount(), Qt.point(0, 22),
                          1, 1.0, 0, 0);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("mg_impact.wav", 1, "resources/sounds/", i * BATTLEANIMATION.defaultFrameDelay);
        }
    };
    this.getMoveInDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 610;
    };

    this.getStopDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 300 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_ZCOUNIT_SMUGGLER.getMaxUnitCount();
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 820 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_ZCOUNIT_SMUGGLER.getMaxUnitCount();
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ZCOUNIT_SMUGGLER = new Constructor();
