var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("antitankcannon",  false,
                          BATTLEANIMATION_ANTITANKCANNON.getMaxUnitCount(), Qt.point(-15, 5));
        sprite.loadSpriteV2("antitankcannon+mask", GameEnums.Recoloring_Table,
                          BATTLEANIMATION_ANTITANKCANNON.getMaxUnitCount(), Qt.point(-15, 5));
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        // gun
        BATTLEANIMATION_ANTITANKCANNON.loadStandingAnimation(sprite, unit, defender, weapon);
        var offset = Qt.point(33, 25);
        // 49 27
        // -15 5
        // -7 15
        var count = sprite.getUnitCount(BATTLEANIMATION_ANTITANKCANNON.getMaxUnitCount());
        sprite.loadSprite("medium_shot",  false, sprite.getMaxUnitCount(), offset,
                          1, 0.5, 0, 0);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("cannon_weapon_fire.wav", 1, "resources/sounds/", i * BATTLEANIMATION.defaultFrameDelay);
		}
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 500 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_ANTITANKCANNON.getMaxUnitCount();
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ANTITANKCANNON = new Constructor();
