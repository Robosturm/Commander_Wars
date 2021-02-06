var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("crystal_tank",  false,
                          BATTLEANIMATION_ZCOUNIT_CRYSTAL_TANK.getMaxUnitCount(), Qt.point(5, 10));
        sprite.loadSpriteV2("crystal_tank+mask", GameEnums.Recoloring_Table,
                          BATTLEANIMATION_ZCOUNIT_CRYSTAL_TANK.getMaxUnitCount(), Qt.point(5, 10));
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_ZCOUNIT_CRYSTAL_TANK.loadStandingAnimation(sprite, unit, defender, weapon);
        var offset = Qt.point(0, 0);
        if (weapon === 0)
        {
            offset = Qt.point(85, 55);
            sprite.loadSingleMovingSprite("medium_shot", false,
                                          Qt.point(offset.x, offset.y),
                                          Qt.point(0, 0), 0, true,
                                          1, 1.0, 5, 200, false);
            sprite.loadSound("tank_shot.wav", 1, "resources/sounds/", 200);
        }
        else
        {
            // mg
            offset = Qt.point(93, 73);
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
        return 500;
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ZCOUNIT_CRYSTAL_TANK = new Constructor();
