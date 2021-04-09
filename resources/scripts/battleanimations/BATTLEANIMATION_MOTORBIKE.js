var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        var player = unit.getOwner();
        // get army name
        var armyName = "+" + player.getArmy().toLowerCase();
        if (armyName === "bg")
        {
            armyName = "bh"
        }
        if ((armyName !== "+bm") &&
                (armyName !== "+os"))
        {
            armyName = "";
        }
        sprite.loadSprite("motorbike" + armyName,  false,
                          BATTLEANIMATION_MOTORBIKE.getMaxUnitCount(), Qt.point(-15, 5));
        sprite.loadSpriteV2("motorbike" + armyName + "+mask", GameEnums.Recoloring_Table,
                            BATTLEANIMATION_MOTORBIKE.getMaxUnitCount(), Qt.point(-15, 5));
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_MOTORBIKE.getMaxUnitCount());
        BATTLEANIMATION_MOTORBIKE.loadStandingAnimation(sprite, unit, defender, weapon);
        // mg
        var player = unit.getOwner();
        // get army name
        var armyName = player.getArmy().toLowerCase();
        if (armyName === "bg")
        {
            armyName = "bh"
        }
        var offset = Qt.point(26, 20);
        if (armyName === "os")
        {
            offset = Qt.point(27, 27);
        }
        else if (armyName === "bm")
        {
            offset = Qt.point(19, 28);
        }
        sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), offset,
                          1, 1, 0, 0);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("mg_weapon_fire.wav", 1, "resources/sounds/", i * BATTLEANIMATION.defaultFrameDelay);
            sprite.loadSound("mg_weapon_fire.wav", 1, "resources/sounds/", 200 + i * BATTLEANIMATION.defaultFrameDelay);
            sprite.loadSound("mg_weapon_fire.wav", 1, "resources/sounds/", 400 + i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadColorOverlayForLastLoadedFrame("#969696", 300, 3, 0);
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_MOTORBIKE.getMaxUnitCount());
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
        return 500 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_MOTORBIKE.getMaxUnitCount();
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_MOTORBIKE = new Constructor();
