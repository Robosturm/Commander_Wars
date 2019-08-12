var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("neotank",  false,
                          BATTLEANIMATION_NEOTANK.getMaxUnitCount(), Qt.point(-5, 5));
        sprite.loadSprite("neotank+mask",  true,
                          BATTLEANIMATION_NEOTANK.getMaxUnitCount(), Qt.point(-5, 5));
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadAnimation("loadStandingAnimation", unit);
        if (weapon === 0)
        {
            // gun
            sprite.loadSprite("medium_shot",  false, sprite.getMaxUnitCount(), Qt.point(27, 23),
                              1, 1.0, 0, 0);
            sprite.loadSound("tank_shot.wav", 1, "resources/sounds/", 0);
        }
        else
        {
            // mg
            sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), Qt.point(35, 47),
                              1, 1, 0, 0);
        }
    };

    this.getFireDurationMS = function()
    {
        // the time will be scaled with animation speed inside the engine
        return 500;
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_NEOTANK = new Constructor();
