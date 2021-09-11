var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };
    this.armyData = [["os", "os"],
                     ["bm", "bm"],
                     ["ge", "ge"],
                     ["yc", "yc"],
                     ["bh", "bh"],
                     ["bg", "bh"],
                     ["ma", "ma"],];

    this.loadMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        var player = unit.getOwner();
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_CRUISER.armyData);
        if (armyName === "ma")
        {            
            BATTLEANIMATION_CRUISER.loadSprite(sprite, unit, defender, weapon, Qt.point(0, 0), 0, 0, 0, 0, 0, Qt.point(-65, 20));
        }
        else
        {
            BATTLEANIMATION_CRUISER.loadSprite(sprite, unit, defender, weapon, Qt.point(-85, 0), 850, 0, 0, 0, 0, Qt.point(20, 20));            
        }
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_CRUISER.loadSprite(sprite, unit, defender, weapon, Qt.point(0, 0), 0, 0, 0, 0, 0, Qt.point(-65, 20));
    };

    this.loadSprite = function(sprite, unit, defender, weapon, movement, moveTime, mgStartFrame, mgEndFrame, rocketStartFrame, rocketEndFrame, startPos,
                               rocketSprite = "+missile", mgSprite = "+cannon+fire+air")
    {
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_CRUISER.armyData);
        if (armyName === "ma")
        {
            startPos = Qt.point(0, 20);
        }
        sprite.loadMovingSprite("cruiser+" + armyName,  false,
                                BATTLEANIMATION_CRUISER.getMaxUnitCount(), startPos, movement, moveTime, false, -1);
        sprite.loadMovingSpriteV2("cruiser+" + armyName + "+mask", GameEnums.Recoloring_Matrix,
                                  BATTLEANIMATION_CRUISER.getMaxUnitCount(), startPos, movement, moveTime, false, -1);
        if (armyName !== "ma")
        {
            sprite.loadMovingSpriteV2("cruiser+" + armyName + rocketSprite + "+mask", GameEnums.Recoloring_Matrix,
                                      BATTLEANIMATION_CRUISER.getMaxUnitCount(), Qt.point(startPos.x + 80, startPos.y + 32),
                                      movement, moveTime, false, 1, 1, 0, 0,
                                      false, 100, rocketEndFrame, rocketStartFrame);
            sprite.loadMovingSpriteV2("cruiser+" + armyName + mgSprite + "+mask", GameEnums.Recoloring_Matrix,
                                      BATTLEANIMATION_CRUISER.getMaxUnitCount(), Qt.point(startPos.x + 80 + 64, startPos.y + 32),
                                      movement, moveTime, false, 3, 1, 0, 0,
                                      false, 100, mgEndFrame, mgStartFrame);
        }
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(5);
        var player = unit.getOwner();
        var rocketSprite = "+missile";
        var mgSprite = "+cannon+fire+air";
        var mgEndFrame = 0;
        var rocketEndFrame = 0;
        if (weapon === 0)
        {
            rocketSprite = "+missile+fire";
            rocketEndFrame = count - 1;
        }
        else
        {
            mgEndFrame = 1;
        }
        
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_CRUISER.armyData);
        var offset = Qt.point(0, 0);

        BATTLEANIMATION_CRUISER.loadSprite(sprite, unit, defender, weapon, Qt.point(0, 0), 0, 
                                           0, mgEndFrame, 0, rocketEndFrame, Qt.point(-65, 20),
                                           rocketSprite, mgSprite);
        if (weapon === 0)
        {
            offset = Qt.point(57, 79);
            if (armyName === "ma")
            {
                offset = Qt.point(70, 70);
            }
            for (var i = 0; i < count; i++)
            {
                var offset2 = Qt.point(0, 0);
                switch (i)
                {
                case 1:
                    offset2 = Qt.point(-5, -5);
                    break;
                case 2:
                    offset2 = Qt.point(-10, -10);
                    break;
                case 3:
                    offset2 = Qt.point(2, -7);
                    break;
                case 4:
                    offset2 = Qt.point(-3, -12);
                    break;
                }
                sprite.loadSingleMovingSprite("rocket_up", false,
                                              Qt.point(offset.x + offset2.x,
                                                       offset.y + offset2.y),
                                              Qt.point(80, 40), 400, false,
                                              -1, 1.0, 5, 100 * i, false);
                sprite.loadSound("rocket_launch.wav", 1, 100 * i);
            }
        }
        else
        {            
            offset = Qt.point(96, 76);
            if (armyName === "ma")
            {
                offset = Qt.point(62, 57);
            }
            sprite.loadSprite("mg_shot_air",  false, sprite.getMaxUnitCount(), offset,
                              1, 1, 0, 0, false, true);
            sprite.loadSound("anti_air_gun_fire.wav", 1, 0);
            sprite.loadSound("anti_air_gun_fire.wav", 1, 200);
            sprite.loadSound("anti_air_gun_fire.wav", 1, 400);
        }
    };

    this.loadStandingFiredAnimation = function(sprite, unit, defender, weapon)
    {
        var rocketEndFrame = 0;
        if (weapon === 0)
        {
            rocketEndFrame = sprite.getUnitCount(5);
        }
        BATTLEANIMATION_CRUISER.loadSprite(sprite, unit, defender, weapon, Qt.point(0, 0), 0, 0, 0, rocketEndFrame, rocketEndFrame, Qt.point(-65, 20));
    };

    this.loadDyingAnimation = function(sprite, unit, defender, weapon)
    {
        var rocketEndFrame = 0;
        if (weapon === 0)
        {
            rocketEndFrame = sprite.getFireUnitCount(5);
        }
        BATTLEANIMATION_CRUISER.loadSprite(sprite, unit, defender, weapon, Qt.point(-140, 0), 2000, rocketEndFrame, rocketEndFrame, Qt.point(-65, 20));
        sprite.loadSound("ship_dying_move.wav", -2);
    };
    
    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        if (weapon === 0)
        {
            return sprite.getUnitCount(5) * 100 + 10;
        }
        else
        {
            return 500;
        }
    };


    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadColorOverlayForLastLoadedFrame("#969696", 1000, 1, 300);
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(5);
        var i = 0;
        if (weapon === 0)
        {
            sprite.loadSprite("rocket_hit",  false, 5, Qt.point(0, 20),
                              1, 1.0, 0, 300);
            sprite.addSpriteScreenshake(8, 0.95, 800, 500);
            sprite.loadMovingSprite("rocket_down", false, 5, Qt.point(127, 90),
                                    Qt.point(-127, -60), 400, true,
                                    -1, 1, 0, 0, true);
            for (i = 0; i < count; i++)
            {
                sprite.loadSound("rocket_flying.wav", 1, 0);
                sprite.loadSound("impact_explosion.wav", 1, 300 + i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
        else
        {
            sprite.loadSprite("mg_hit",  false, 5, Qt.point(0, 22),
                              1, 1.0, 0, 0);
            for (i = 0; i < count; i++)
            {
                sprite.loadSound("mg_impact.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        if (weapon === 0)
        {
            return 1000;
        }
        else
        {
            return 1000;
        }
    };

    this.hasMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        return true;
    };
    this.getMoveInDurationMS = function(sprite, unit, defender, weapon)
    {
        return 860;
    };

    this.getDyingDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 2200;
    };

    this.hasDyingAnimation = function()
    {
        // return true if the unit has an implementation for loadDyingAnimation
        return true;
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_CRUISER = new Constructor();
