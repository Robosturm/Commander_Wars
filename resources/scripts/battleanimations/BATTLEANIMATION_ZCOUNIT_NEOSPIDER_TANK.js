var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSpriteV2("neospider_tank+mask", GameEnums.Recoloring_Matrix,
                            BATTLEANIMATION_ZCOUNIT_NEOSPIDER_TANK.getMaxUnitCount(), Qt.point(-35, 0));
        BATTLEANIMATION.loadSpotterOrCoMini(sprite, unit, false);
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
                sprite.loadSound("neocannonweapon_fire.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
        else
        {
            // mg
            offset = Qt.point(32, 17);
            sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), offset,
                              1, 1, 0, 0, false, true);
            BATTLEANIMATION.showMgBullets(sprite, offset);
            for (var i = 0; i < count; i++)
            {
                sprite.loadSound("mg_weapon_fire.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
                sprite.loadSound("mg_weapon_fire.wav", 1, 200 + i * BATTLEANIMATION.defaultFrameDelay, 1, true);
                sprite.loadSound("mg_weapon_fire.wav", 1, 400 + i * BATTLEANIMATION.defaultFrameDelay, 1, true);
            }
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        return 600 + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_NEOSPIDER_TANK.getMaxUnitCount());
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_NEOSPIDER_TANK.getMaxUnitCount());
        var i = 0;
        if (weapon === 0)
        {
            sprite.loadSprite("bullet_megatank",  false, sprite.getMaxUnitCount(), Qt.point(10, 30),
                              1, 1.0, 0, 0, true, true, 50);
            sprite.loadSprite("cannon_heavy_hit",  false, 5, Qt.point(0, 20),
                              1, 1.0, 0, 100, true);
            sprite.addSpriteScreenshake(8, 0.95, 800, 300);
            for (i = 0; i < count; i++)
            {
                sprite.loadSound("tank_hit.wav", 1, 50 + i * BATTLEANIMATION.defaultFrameDelay);
                sprite.loadSound("impact_explosion.wav", 1, 100 + i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
        else
        {
            var yOffset = 22;
            if (unit.getUnitType()  === GameEnums.UnitType_Air)
            {
                yOffset = 40
            }
            sprite.loadSprite("mg_hit",  false, 5, Qt.point(0, yOffset),
                              1, 1.0, 0, 0, true);
            BATTLEANIMATION.playMgImpactSound(sprite, unit, defender, weapon, count);
        }
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ZCOUNIT_NEOSPIDER_TANK = new Constructor();
