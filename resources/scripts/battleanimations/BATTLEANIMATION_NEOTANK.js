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
        sprite.loadMovingSprite("vehicle_dust", false, sprite.getMaxUnitCount(), Qt.point(-100, 7),
                                Qt.point(65, 0), 400, false,
                                1, 1);
    };

    this.loadStopAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_NEOTANK.loadSprite(sprite, unit, defender, weapon, "+stop");
        sprite.loadSprite("vehicle_dust_stop",  false,
                          BATTLEANIMATION_NEOTANK.getMaxUnitCount(), Qt.point(-35, 7), 1);
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
            var offset = Qt.point(-15, 5);
            if (BATTLEANIMATION.getRelativePosition(unit, defender) > 0)
            {
                sprite.loadSprite("neotank+mg_fire_air",  false,
                                  BATTLEANIMATION_NEOTANK.getMaxUnitCount(), offset, 3);
                sprite.loadSpriteV2("neotank+mg_fire_air+mask", GameEnums.Recoloring_Table,
                                    BATTLEANIMATION_NEOTANK.getMaxUnitCount(), offset, 3);
                // mg
                sprite.loadSprite("mg_shot_air",  false, sprite.getMaxUnitCount(), Qt.point(26, 51),
                                  1, 1, 0, 0);
            }
            else
            {
                sprite.loadSprite("neotank+mg_fire",  false,
                                  BATTLEANIMATION_NEOTANK.getMaxUnitCount(), offset, 3);
                sprite.loadSpriteV2("neotank+mg_fire+mask", GameEnums.Recoloring_Table,
                                    BATTLEANIMATION_NEOTANK.getMaxUnitCount(), offset, 3);
                // mg
                sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), Qt.point(26, 46),
                                  1, 1, 0, 0);
            }
        }
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
        return 300 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_NEOTANK.getMaxUnitCount();
    };

    this.getFireDurationMS = function()
    {
        // the time will be scaled with animation speed inside the engine
        return 820 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_NEOTANK.getMaxUnitCount();
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_NEOTANK = new Constructor();
