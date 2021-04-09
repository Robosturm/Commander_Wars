var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("neospider_tank",  false,
                          BATTLEANIMATION_ZCOUNIT_NEOSPIDER_TANK.getMaxUnitCount(), Qt.point(-35, 0));
        sprite.loadSpriteV2("neospider_tank+mask", GameEnums.Recoloring_Table,
                            BATTLEANIMATION_ZCOUNIT_NEOSPIDER_TANK.getMaxUnitCount(), Qt.point(-35, 0));
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadAnimation("loadStandingAnimation", unit);
        var offset = Qt.point(24, 17);
        var count = sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_NEOSPIDER_TANK.getMaxUnitCount());
        if (weapon === 0)
        {

            sprite.loadSprite("medium_shot",  false, sprite.getMaxUnitCount(), offset,
                              1, 1.0, 0, 0);
            for (var i = 0; i < count; i++)
            {
                sprite.loadSound("neocannonweapon_fire.wav", 1, "resources/sounds/", i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
        else
        {
            // mg
            offset = Qt.point(32, 17);
            sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), offset,
                              1, 1, 0, 0, false, true);
            for (var i = 0; i < count; i++)
            {
                sprite.loadSound("mg_weapon_fire.wav", 1, "resources/sounds/", i * BATTLEANIMATION.defaultFrameDelay);
                sprite.loadSound("mg_weapon_fire.wav", 1, "resources/sounds/", 200 + i * BATTLEANIMATION.defaultFrameDelay);
                sprite.loadSound("mg_weapon_fire.wav", 1, "resources/sounds/", 400 + i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_NEOSPIDER_TANK.getMaxUnitCount());
        var i = 0;
        if (weapon === 0)
        {
            sprite.loadSprite("unit_explosion",  false, 5, Qt.point(0, 20),
                              1, 1.0, 0, 0);
            sprite.addSpriteScreenshake(8, 0.95, 800, 200);
            for (i = 0; i < count; i++)
            {
                sprite.loadSound("impact_explosion.wav", 1, "resources/sounds/", i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
        else
        {
            sprite.loadSprite("mg_hit",  false, 5, Qt.point(0, 22),
                              1, 1.0, 0, 0);
            for (i = 0; i < count; i++)
            {
                sprite.loadSound("mg_impact.wav", 1, "resources/sounds/", i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 500 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_ZCOUNIT_NEOSPIDER_TANK.getMaxUnitCount();
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ZCOUNIT_NEOSPIDER_TANK = new Constructor();
