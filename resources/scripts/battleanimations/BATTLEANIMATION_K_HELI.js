var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };
    this.armyData = [["bd", "bd"],
                     ["bh", "bh"],
                     ["bg", "bh"],
                     ["bm", "bm"],
                     ["dm", "dm"],
                     ["ge", "ge"],
                     ["ma", "ma"],
                     ["os", "os"],
                     ["pf", "pf"],
                     ["ti", "ti"],
                     ["yc", "yc"],];


    this.animationData = [["bd", [Qt.point(-36, 31), Qt.point(30, 35)]],
                          ["bh", [Qt.point(-49, 39), Qt.point(33, 34)]],
                          ["bm", [Qt.point(-36, 31), Qt.point(30, 35)]],
                          ["dm", [Qt.point(-40, 41), Qt.point(29, 38)]],
                          ["ge", [Qt.point(-38, 37), Qt.point(33, 33)]],
                          ["ma", [Qt.point(-22, 39), Qt.point(41, 49)]],
                          ["os", [Qt.point(-40, 32), Qt.point(29, 34)]],
                          ["pf", [Qt.point(-29, 28), Qt.point(37, 38)]],
                          ["ti", [Qt.point(-38, 37), Qt.point(32, 34)]],
                          ["yc", [Qt.point(-37, 32), Qt.point(26, 38)]],];

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_K_HELI.loadSprite(sprite, unit, defender, weapon, 0, 0);
    };

    this.loadSprite = function(sprite, unit, defender, weapon, startFrame = 0, endFrame = 0, loops = 1)
    {
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_K_HELI.armyData);
        var offset = Qt.point(-30, 30);
        sprite.loadSpriteV2("k_heli+" + armyName + "+mask", GameEnums.Recoloring_Matrix,
                            BATTLEANIMATION_K_HELI.getMaxUnitCount(), offset, -1, 1, 0, 0, false,
                            false, 50);
        sprite.addMoveTweenToLastLoadedSprites(0, -3, 1200);
        sprite.loadSpriteV2("k_heli+" + armyName + "+mg+fire+mask", GameEnums.Recoloring_Matrix,
                            BATTLEANIMATION_K_HELI.getMaxUnitCount(), Qt.point(offset.x + 40, offset.y),
                            loops, 1, 0, 0, false,
                            false, 100, endFrame, startFrame);
        sprite.addMoveTweenToLastLoadedSprites(0, -3, 1200);
    }

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_K_HELI.armyData);
        var offset = Qt.point(0, 0);
        var count = sprite.getUnitCount(BATTLEANIMATION_K_HELI.getMaxUnitCount());
        var data = Global.getDataFromTable(armyName, BATTLEANIMATION_K_HELI.animationData);
        if (weapon === 0)
        {
            BATTLEANIMATION_K_HELI.loadSprite(sprite, unit, defender, weapon);
            offset = data[0];
            sprite.loadMovingSprite("rocket", false, sprite.getMaxUnitCount(), offset,
                                    Qt.point(130, -80), 600, true,
                                    -1, 1, -1);
            offset = Qt.point(data[0].x + 45, data[0].y + 5);
            sprite.loadSprite("rocket_trailing_smoke", false, BATTLEANIMATION_K_HELI.getMaxUnitCount(), offset);
            offset = Qt.point(data[0].x + 2, data[0].y);
            sprite.loadSprite("bazooka_launch",  false, BATTLEANIMATION_K_HELI.getMaxUnitCount(), offset);
            for (var i = 0; i < count; i++)
            {
                sprite.loadSound("missile_weapon_fire.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
        else
        {
            BATTLEANIMATION_K_HELI.loadSprite(sprite, unit, defender, weapon, 0, 1, 3);
            offset = data[1];
            sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), offset,
                              1, 1, 0, 0);
            sprite.addMoveTweenToLastLoadedSprites(0, -3, 1200);
            for (var i = 0; i < count; i++)
            {
                sprite.loadSound("mg_weapon_fire.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
                sprite.loadSound("mg_weapon_fire.wav", 1, 200 + i * BATTLEANIMATION.defaultFrameDelay, 1, true);
                sprite.loadSound("mg_weapon_fire.wav", 1, 400 + i * BATTLEANIMATION.defaultFrameDelay, 1, true);
            }
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        return 600 + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_K_HELI.getMaxUnitCount());
    };

    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        if (weapon === 0)
        {
            sprite.loadColorOverlayForLastLoadedFrame("#969696", 1000, 1, 300);
        }
        else
        {
            sprite.loadColorOverlayForLastLoadedFrame("#969696", 300, 2, 0);
        }
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_K_HELI.getMaxUnitCount());
        var i = 0;
        if (weapon === 0)
        {
            sprite.loadSprite("rocket_hit",  false, sprite.getMaxUnitCount(), Qt.point(0, 20),
                              1, 1.0, 0, 400, true);
            sprite.addSpriteScreenshake(8, 0.95, 800, 500);
            sprite.loadMovingSprite("rocket", false, sprite.getMaxUnitCount(), Qt.point(127, 90),
                                    Qt.point(-127, -60), 400, true,
                                    -1, 1, 0, 0, true);
            for (i = 0; i < count; i++)
            {
                sprite.loadSound("rocket_flying.wav", 1, 0);
                sprite.loadSound("rockets_explode.wav", 1, 200 + i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
        else
        {
            var yOffset = 22;
            if (unit.getUnitType()  === GameEnums.UnitType_Air)
            {
                yOffset = 40
            }
            sprite.loadSprite("mg_hit",  false, sprite.getMaxUnitCount(), Qt.point(0, yOffset),
                              1, 1.0, 0, 0);
            BATTLEANIMATION.playMgImpactSound(sprite, unit, defender, weapon, count);
        }
    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        if (weapon === 0)
        {
            return 400 + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_K_HELI.getMaxUnitCount());
        }
        else
        {
            return 400 - BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_K_HELI.getMaxUnitCount());
        }
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_K_HELI = new Constructor();
