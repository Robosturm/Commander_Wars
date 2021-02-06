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
        var offset = Qt.point(30, 28);
        sprite.loadSprite("medium_shot",  false, sprite.getMaxUnitCount(), offset,
                          1, 0.5, 0, 0);
        sprite.loadSound("tank_shot.wav", 1, "resources/sounds/", 0);
    };

    this.getFireDurationMS = function()
    {
        // the time will be scaled with animation speed inside the engine
        return 500 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_ANTITANKCANNON.getMaxUnitCount();
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ANTITANKCANNON = new Constructor();
