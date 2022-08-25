var Constructor = function()
{
    this.armyData = [["ac", "ac"],
                     ["bd", ""],
                     ["bh", "bh"],
                     ["bg", "bg"],
                     ["bm", "bm"],
                     ["dm", ""],
                     ["ge", "ge"],
                     ["gs", "gs"],
                     ["ma", "ma"],
                     ["os", "os"],
                     ["pf", "pf"],
                     ["ti", "ti"],
                     ["yc", "yc"],];

    this.animationData = [["ac", [Qt.point(-50, 20),  Qt.point(-84, 30), Qt.point(-50, 30)]],
                          ["bm", [Qt.point(-50, 20),  Qt.point(-84, 30), Qt.point(-50, 30)]],
                          ["ge", [Qt.point(-50, 20),  Qt.point(-84, 30), Qt.point(-50, 30)]],
                          ["os", [Qt.point(-50, 20),  Qt.point(-84, 30), Qt.point(-50, 30)]],
                          ["yc", [Qt.point(-50, 20),  Qt.point(-84, 30), Qt.point(-50, 30)]],
                          ["bh", [Qt.point(-50, 20),  Qt.point(-84, 30), Qt.point(-50, 30)]],
                          ["bg", [Qt.point(-50, 20),  Qt.point(-84, 30), Qt.point(-50, 30)]],
                          ["gs", [Qt.point(-50, 20),  Qt.point(-84, 30), Qt.point(-50, 30)]],
                          ["ma", [Qt.point(-50, 20),  Qt.point(-84, 30), Qt.point(-50, 30)]],
                          ["pf", [Qt.point(-50, 20),  Qt.point(-84, 30), Qt.point(-50, 30)]],
                          ["ti", [Qt.point(-50, 20),  Qt.point(-84, 30), Qt.point(-50, 30)]],
                          ["",   [Qt.point(-50, 20),  Qt.point(-84, 30), Qt.point(-50, 30)]],];

    this.getMaxUnitCount = function()
    {
        return 5;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_WATERPLANE.loadSprite(sprite, unit, defender, weapon, "");
    };
    this.loadSprite = function(sprite, unit, defender, weapon, ending)
    {
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_WATERPLANE.armyData);
        var data = Global.getDataFromTable(armyName, BATTLEANIMATION_WATERPLANE.animationData);
        var offset = data[0];

        var spriteId = "waterplane+mask";
        if (armyName !== "")
        {
            spriteId = "waterplane+" + armyName + ending + "+mask";
        }
        sprite.loadSpriteV2(spriteId, GameEnums.Recoloring_Matrix,
                            BATTLEANIMATION_WATERPLANE.getMaxUnitCount(), offset, -1, 1.0, 0, 0,
                            false, false, 30);
        sprite.addMoveTweenToLastLoadedSprites(0, -5, 1200);
    };
    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_WATERPLANE.loadSprite(sprite, unit, defender, weapon, "+fire");
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_WATERPLANE.armyData);
        var data = Global.getDataFromTable(armyName, BATTLEANIMATION_WATERPLANE.animationData);
        var count = sprite.getUnitCount(BATTLEANIMATION_WATERPLANE.getMaxUnitCount());
        if (defender.getUnitType() === GameEnums.UnitType_Air)
        {
            var offset = data[1];
            sprite.loadMovingSprite("rocket", false, sprite.getMaxUnitCount(), offset,
                                    Qt.point(227, 0), 800, false,
                                    -1, 1, -1);
        }
        else
        {
            var offset = data[2];
            sprite.loadMovingSprite("rocket_down", false, sprite.getMaxUnitCount(), offset,
                                    Qt.point(150, -75), 800, false,
                                    -1, 1, -1);
        }
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("missile_weapon_fire.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        return 600 + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_WATERPLANE.getMaxUnitCount());
    };

    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadColorOverlayForLastLoadedFrame("#969696", 1000, 1, 300);
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_WATERPLANE.getMaxUnitCount());

        if (defender.getUnitType() === GameEnums.UnitType_Air)
        {
            sprite.loadMovingSprite("rocket_hit_air", false, sprite.getMaxUnitCount(), Qt.point(0, 20),
                                    Qt.point(-50, 0), 400, false,
                                    1, 1.0, 0, 300, true);
            sprite.loadMovingSprite("rocket", false, sprite.getMaxUnitCount(), Qt.point(127, 20),
                                    Qt.point(-127, 0), 400, true,
                                    -1, 1, 0, 0, true);
        }
        else
        {
            sprite.loadSprite("rocket_hit",  false, sprite.getMaxUnitCount(), Qt.point(0, 20),
                              1, 1.0, 0, 400, true);
            sprite.loadMovingSprite("rocket_down", false, sprite.getMaxUnitCount(), Qt.point(127, 80),
                                    Qt.point(-128, -64), 400, true,
                                    -1, 1, 0, 0, true);
        }
        sprite.addSpriteScreenshake(8, 0.95, 800, 500);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("rocket_flying.wav", 1, 0);
            sprite.loadSound("rockets_explode.wav", 1, 200 + i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        if (defender.getUnitType() === GameEnums.UnitType_Air)
        {
            return 400 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_WATERPLANE.getMaxUnitCount());
        }
        else
        {
            return 400 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_WATERPLANE.getMaxUnitCount());
        }
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_WATERPLANE = new Constructor();
