var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_STEALTHBOMBER.loadSprite(sprite, unit, defender, weapon, Qt.point(0, 0), 0);
    };

    this.loadSprite = function(sprite, unit, defender, weapon, movement, moveTime, alive = true)
    {
        sprite.loadMovingSprite("stealthbomber",  false,
                          BATTLEANIMATION_STEALTHBOMBER.getMaxUnitCount(), Qt.point(0, 60), movement, moveTime);
        if (alive)
        {
            sprite.addMoveTweenToLastLoadedSprites(0, -3, 800);
        }
        sprite.loadMovingSpriteV2("stealthbomber+mask", GameEnums.Recoloring_Table,
                            BATTLEANIMATION_STEALTHBOMBER.getMaxUnitCount(), Qt.point(0, 60), movement, moveTime);
        if (alive)
        {
            sprite.addMoveTweenToLastLoadedSprites(0, -3, 800);
        }
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_STEALTHBOMBER.loadStandingAnimation(sprite, unit, defender, weapon);
        var count = sprite.getUnitCount(5);
        for (var i = 0; i < count; i++)
        {
            if (defender.getUnitType() === GameEnums.UnitType_Air)
            {
                sprite.loadSingleMovingSprite("rocket", false, Qt.point(45, 62),
                                              Qt.point(128, 0), 400, false,
                                              1, 1, -1, i * 150);
            }
            else
            {
                sprite.loadSingleMovingSprite("rocket_down", false, Qt.point(45, 82),
                                              Qt.point(128, -64), 400, false,
                                              1, 1, -1, i * 150);
            }
            sprite.loadSound("rocket_launch.wav", 1, "resources/sounds/", i * 150);
        }
    };

    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadColorOverlayForLastLoadedFrame("#969696", 1000, 1, 300);
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("unit_explosion",  false, 5, Qt.point(0, 60),
                          1, 1.0, 0, 300);
        if (defender.getUnitType() === GameEnums.UnitType_Air)
        {
            sprite.loadMovingSprite("rocket", false, 5, Qt.point(127, 60),
                                    Qt.point(-128, 0), 400, true,
                                    1, 1, 0, 0, true);
        }
        else
        {
            sprite.loadMovingSprite("rocket_down", false, 5, Qt.point(127, 80),
                                    Qt.point(-128, -64), 400, true,
                                    1, 1, 0, 0, true);
        }
        sprite.loadSound("rocket_flying.wav", 1, "resources/sounds/", 0);
        sprite.loadSound("impact_explosion.wav", 1, "resources/sounds/", 200);
    };

    this.hasDyingAnimation = function()
    {
        // return true if the unit has an implementation for loadDyingAnimation
        return true;
    };

    this.loadDyingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_STEALTHBOMBER.loadSprite(sprite, unit, defender, weapon, Qt.point(-140, -140), 600, false);
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 1250;
    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        // should be a second or longer.
        // the time will be scaled with animation speed inside the engine
        return 1500;
    };

    this.getDyingDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 1000;
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_STEALTHBOMBER = new Constructor();
