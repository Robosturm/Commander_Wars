var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };

    this.armyData = [["os", "os"],
                     ["ma", "ma"],];

    this.animationData = [["os", [-80,  Qt.point(65, 0), 600, Qt.point(25, 23), Qt.point(26, 51), Qt.point(26, 46)]],
                          ["ma", [-100, Qt.point(65, 0), 600, Qt.point(17, 25), Qt.point(36, 31), Qt.point(36, 26)]],];

    this.loadMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_NEOTANK.getMaxUnitCount());
        var armyName = Global.getArmyNameFromPlayerTable(unit.getOwner(), BATTLEANIMATION_NEOTANK.armyData);
        var data = Global.getArmyDataFromTable(armyName, BATTLEANIMATION_NEOTANK.animationData);
        sprite.loadMovingSpriteV2("neotank+" + armyName + "+move+mask", GameEnums.Recoloring_Matrix, sprite.getMaxUnitCount(), Qt.point(data[0], 5),
                                  data[1], data[2], false,
                                  1, 1);
        sprite.loadMovingSprite("vehicle_dust", false, sprite.getMaxUnitCount(), Qt.point(data[0] - 20, 7),
                                data[1], data[2], false,
                                1, 1);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("neotank_move.wav", 5, i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.loadStopAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_NEOTANK.loadSprite(sprite, unit, defender, weapon, "+stop");
        sprite.loadSprite("vehicle_dust_stop",  false,
                          BATTLEANIMATION_NEOTANK.getMaxUnitCount(), Qt.point(data[0] + data[1].x - 20, 7), 1);
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_NEOTANK.loadSprite(sprite, unit, defender, weapon, "");
    };

    this.loadSprite = function(sprite, unit, defender, weapon, ending)
    {
        var armyName = Global.getArmyNameFromPlayerTable(unit.getOwner(), BATTLEANIMATION_NEOTANK.armyData);
        var data = Global.getArmyDataFromTable(armyName, BATTLEANIMATION_NEOTANK.animationData);
        var offset = Qt.point(data[0] + data[1].x, 5);
        sprite.loadSpriteV2("neotank+" + armyName + ending + "+mask", GameEnums.Recoloring_Matrix,
                            BATTLEANIMATION_NEOTANK.getMaxUnitCount(), offset, 1);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_NEOTANK.getMaxUnitCount());
        var armyName = Global.getArmyNameFromPlayerTable(unit.getOwner(), BATTLEANIMATION_NEOTANK.armyData);
        var data = Global.getArmyDataFromTable(armyName, BATTLEANIMATION_NEOTANK.animationData);
        if (weapon === 0)
        {
            BATTLEANIMATION_NEOTANK.loadSprite(sprite, unit, defender, weapon, "+cannon+fire");
            sprite.loadSprite("medium_shot",  false, sprite.getMaxUnitCount(), data[3],
                              1, 1.0, 0, 120);
            for (var i = 0; i < count; i++)
            {
                sprite.loadSound("neocannonweapon_fire.wav", 1, 120 + i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
        else
        {
            var offset = Qt.point(-15, 5);
            if (BATTLEANIMATION.getRelativePosition(unit, defender) > 0)
            {
                sprite.loadSpriteV2("neotank+" + armyName + "+mg+fire+air+mask", GameEnums.Recoloring_Matrix,
                                    BATTLEANIMATION_NEOTANK.getMaxUnitCount(), offset, 3);
                sprite.loadSprite("mg_shot_air",  false, sprite.getMaxUnitCount(), data[4],
                                  1, 1, 0, 0);
            }
            else
            {
                sprite.loadSpriteV2("neotank+" + armyName + "+mg+fire+mask", GameEnums.Recoloring_Matrix,
                                    BATTLEANIMATION_NEOTANK.getMaxUnitCount(), offset, 3);
                sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), data[5],
                                  1, 1, 0, 0);
            }
            for (var i = 0; i < count; i++)
            {
                sprite.loadSound("mg_weapon_fire.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
                sprite.loadSound("mg_weapon_fire.wav", 1, 200 + i * BATTLEANIMATION.defaultFrameDelay);
                sprite.loadSound("mg_weapon_fire.wav", 1, 400 + i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        return 600 + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_NEOTANK.getMaxUnitCount());
    };

    this.hasMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        return true;
    };

    this.getMoveInDurationMS = function(sprite, unit, defender, weapon)
    {
        return 610;
    };

    this.getStopDurationMS = function(sprite, unit, defender, weapon)
    {
        return 500 + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_NEOTANK.getMaxUnitCount());
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_NEOTANK = new Constructor();
