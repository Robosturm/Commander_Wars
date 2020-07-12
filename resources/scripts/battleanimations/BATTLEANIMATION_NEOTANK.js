var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };

    this.loadMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadMovingSprite("neotank+move", false, sprite.getMaxUnitCount(), Qt.point(-80, 5),
                                Qt.point(65, 0), 400, false,
                                1, 1);
        sprite.loadMovingSpriteV2("neotank+move+mask", GameEnums.Recoloring_Table, sprite.getMaxUnitCount(), Qt.point(-80, 5),
                                Qt.point(65, 0), 400, false,
                                1, 1);
    };

    this.loadStopAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_NEOTANK.loadSprite(sprite, unit, defender, weapon, "+stop");
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_NEOTANK.loadSprite(sprite, unit, defender, weapon, "");
    };


    this.loadSprite = function(sprite, unit, defender, weapon, ending)
    {
        var offset = Qt.point(-15, 5);
        sprite.loadSprite("neotank" + ending,  false,
                          BATTLEANIMATION_NEOTANK.getMaxUnitCount(), offset, 1);
        sprite.loadSpriteV2("neotank" + ending + "+mask", GameEnums.Recoloring_Table,
                          BATTLEANIMATION_NEOTANK.getMaxUnitCount(), offset, 1);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {

        if (weapon === 0)
        {
            BATTLEANIMATION_NEOTANK.loadSprite(sprite, unit, defender, weapon, "+cannon_fire");
            // gun
            sprite.loadSprite("medium_shot",  false, sprite.getMaxUnitCount(), Qt.point(22, 23),
                              1, 1.0, 0, 120);
            sprite.loadSound("tank_shot.wav", 1, "resources/sounds/", 120);
        }
        else
        {
            sprite.loadAnimation("loadStandingAnimation", unit);
            // mg
            sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), Qt.point(35, 47),
                              1, 1, 0, 0);
        }
    };

    this.getFireDurationMS = function()
    {
        // the time will be scaled with animation speed inside the engine
        return 620;
    };

    this.hasMoveInAnimation = function()
    {
        // return true if the unit has an implementation for loadMoveInAnimation
        return true;
    };
    this.getMoveInDurationMS = function()
    {
        // the time will be scaled with animation speed inside the engine
        return 410;
    };

    this.getStopDurationMS = function()
    {
        // the time will be scaled with animation speed inside the engine
        return 100;
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_NEOTANK = new Constructor();
