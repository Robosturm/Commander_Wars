var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        var player = unit.getOwner();
        var armyName = "+" + player.getArmy().toLowerCase();
        if (armyName === "+bg")
        {
            armyName = "+bh"
        }
        if ((armyName !== "+bh"))
        {
            armyName = "";
        }
        sprite.loadSprite("flare" + armyName,  false,
                          BATTLEANIMATION_FLARE.getMaxUnitCount(), Qt.point(-38, 5));
        sprite.loadSpriteV2("flare" + armyName + "+mask", GameEnums.Recoloring_Table,
                            BATTLEANIMATION_FLARE.getMaxUnitCount(), Qt.point(-38, 5));
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_FLARE.loadStandingAnimation(sprite, unit, defender, weapon);
        // mg
        var offset = Qt.point(31, 29);
        var player = unit.getOwner();
        // get army name
        var armyName = player.getArmy().toLowerCase();
        if (armyName === "bg")
        {
            armyName = "bh"
        }
        if (armyName === "bh")
        {
            offset = Qt.point(17, 27);
        }
        sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), offset,
                          1, 1, 0, 0);
    };

    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadColorOverlayForLastLoadedFrame("#969696", 300, 3, 0);
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_FLARE.getMaxUnitCount());
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
        return 500 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_FLARE.getMaxUnitCount();
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_FLARE = new Constructor();
