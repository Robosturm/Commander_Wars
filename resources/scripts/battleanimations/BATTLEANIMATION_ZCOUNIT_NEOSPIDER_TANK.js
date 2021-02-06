var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("neospider_tank",  false,
                          BATTLEANIMATION_ZCOUNIT_NEOSPIDER_TANK.getMaxUnitCount(), Qt.point(25, 25));
        sprite.loadSpriteV2("neospider_tank+mask", GameEnums.Recoloring_Table,
                          BATTLEANIMATION_ZCOUNIT_NEOSPIDER_TANK.getMaxUnitCount(), Qt.point(25, 25));
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadAnimation("loadStandingAnimation", unit);
        var offset = Qt.point(84, 42);
        if (weapon === 0)
		{
                
            sprite.loadSprite("medium_shot",  false, sprite.getMaxUnitCount(), offset,
                              1, 1.0, 0, 0);
            sprite.loadSound("tank_shot.wav", 1, "resources/sounds/", 0);
            
        }
        else
        {
            // mg
            offset = Qt.point(92, 42);
            sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), offset,
                              1, 1, 0, 0, false, true);
        }
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        if (weapon === 0)
        {
            sprite.loadSprite("unit_explosion",  false, 5, Qt.point(0, 20),
                              1, 1.0, 0, 0);
            sprite.loadSound("impact_explosion.wav", 1, "resources/sounds/", 0);
        }
        else
        {
            sprite.loadSprite("mg_hit",  false, 5, Qt.point(0, 22),
                              1, 1.0, 0, 0);
            sprite.loadSound("mg_impact.wav", 1, "resources/sounds/", 0);
        }
    };

    this.getFireDurationMS = function()
    {
        // the time will be scaled with animation speed inside the engine
        return 500 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_ZCOUNIT_NEOSPIDER_TANK.getMaxUnitCount();
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ZCOUNIT_NEOSPIDER_TANK = new Constructor();
