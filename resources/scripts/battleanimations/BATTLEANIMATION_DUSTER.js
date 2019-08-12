var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("duster",  false,
                          BATTLEANIMATION_DUSTER.getMaxUnitCount(), Qt.point(-40, 20));
        sprite.loadSprite("duster+mask",  true,
                          BATTLEANIMATION_DUSTER.getMaxUnitCount(), Qt.point(-40, 20));
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_DUSTER.loadStandingAnimation(sprite, unit, defender, weapon);
        // mg
        var player = unit.getOwner();
        // get army name
        var armyName = player.getArmy().toLowerCase();
        if (armyName === "bg")
        {
            armyName = "bh"
        }
        var offset = Qt.point(45, 50);
        sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), offset,
                          1, 1, 0, 0);
    };

    this.getFireDurationMS = function()
    {
        // the time will be scaled with animation speed inside the engine
        return 500;
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("mg_hit",  false, sprite.getMaxUnitCount(), Qt.point(0, 22),
                          1, 1.0, 0, 0);
        sprite.loadSound("mg_impact.wav", 1, "resources/sounds/", 0);
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_DUSTER = new Constructor();
