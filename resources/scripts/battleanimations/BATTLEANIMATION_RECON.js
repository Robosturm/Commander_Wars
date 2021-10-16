var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };
    this.armyData = [["ac", "ac"],
                     ["bd", "bd"],
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

    this.animationData = [["ac", [Qt.point(-5, 5),  Qt.point(41, 41), Qt.point(41, 33)]],
                          ["bd", [Qt.point(-5, 5),  Qt.point(34, 42), Qt.point(34, 34)]],
                          ["bh", [Qt.point(-5, 5),  Qt.point(39, 37), Qt.point(39, 30)]],
                          ["bm", [Qt.point(-5, 5),  Qt.point(37, 47), Qt.point(37, 40)]],
                          ["dm", [Qt.point(-5, 5),  Qt.point(34, 49), Qt.point(34, 44)]],
                          ["ge", [Qt.point(-5, 5),  Qt.point(39, 37), Qt.point(39, 30)]],
                          ["ma", [Qt.point(-15, 5), Qt.point(39, 28), Qt.point(39, 20)]],
                          ["os", [Qt.point(-5, 5),  Qt.point(35, 50), Qt.point(35, 44)]],
                          ["pf", [Qt.point(-5, 5),  Qt.point(30, 48), Qt.point(31, 40)]],
                          ["ti", [Qt.point(-5, 5),  Qt.point(36, 37), Qt.point(36, 30)]],
                          ["yc", [Qt.point(-5, 5),  Qt.point(29, 47), Qt.point(30, 40)]],];

    this.getArmyName = function(unit)
    {
        var player = unit.getOwner();
        return Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_RECON.armyData);
    };

    this.loadMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_RECON.getMaxUnitCount());
        var armyName = BATTLEANIMATION_RECON.getArmyName(unit);
        var data = Global.getDataFromTable(armyName, BATTLEANIMATION_RECON.animationData);
        var movement = 65;
        sprite.loadMovingSpriteV2("recon+" + armyName + "+move+mask", GameEnums.Recoloring_Matrix, sprite.getMaxUnitCount(), Qt.point(data[0].x - movement, 5),
                                  Qt.point(movement, 0), 600, false, 1, 1);
        sprite.loadMovingSprite("vehicle_dust", false, sprite.getMaxUnitCount(), Qt.point(data[0].x - movement - 25, 7),
                                Qt.point(movement, 0), 600, false, 1, 1);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("recon_move.wav", 5, i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.loadStopAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_RECON.loadSprite(sprite, unit, defender, weapon, "+stop", 1);
        var armyName = BATTLEANIMATION_RECON.getArmyName(unit);
        var data = Global.getDataFromTable(armyName, BATTLEANIMATION_RECON.animationData);
        sprite.loadSprite("vehicle_dust_stop",  false,
                          BATTLEANIMATION_RECON.getMaxUnitCount(), Qt.point(data[0].x - 25, 7), 1);
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_RECON.loadSprite(sprite, unit, defender, weapon, "", 1);
    };

    this.loadSprite = function(sprite, unit, defender, weapon, ending, count)
    {
        var armyName = BATTLEANIMATION_RECON.getArmyName(unit);
        var data = Global.getDataFromTable(armyName, BATTLEANIMATION_RECON.animationData);
        var offset = data[0];
        sprite.loadSpriteV2("recon+" + armyName + ending + "+mask", GameEnums.Recoloring_Matrix,
                            BATTLEANIMATION_RECON.getMaxUnitCount(), offset, count);
    };


    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_RECON.getMaxUnitCount());
        var armyName = BATTLEANIMATION_RECON.getArmyName(unit);
        var offset = Qt.point(35, 44);
        var data = Global.getDataFromTable(armyName, BATTLEANIMATION_RECON.animationData);
        if (BATTLEANIMATION.getRelativePosition(unit, defender) > 0)
        {
            BATTLEANIMATION_RECON.loadSprite(sprite, unit, defender, weapon, "+fire+air", 2);
            offset = data[1];
            sprite.loadSprite("mg_shot_air",  false, sprite.getMaxUnitCount(), offset,
                              1, 1, 0, 0);
        }
        else
        {
            BATTLEANIMATION_RECON.loadSprite(sprite, unit, defender, weapon, "+fire", 2);
            offset = data[2];
            sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), offset,
                              1, 1, 0, 0);
        }
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("mg_weapon_fire.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
            sprite.loadSound("mg_weapon_fire.wav", 1, 200 + i * BATTLEANIMATION.defaultFrameDelay);
            sprite.loadSound("mg_weapon_fire.wav", 1, 400 + i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        return 600 + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_RECON.getMaxUnitCount());
    };

    this.hasMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        return true;
    };

    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadColorOverlayForLastLoadedFrame("#969696", 300, 3, 0);
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_RECON.getMaxUnitCount());
        var yOffset = 22;
        if (unit.getUnitType()  === GameEnums.UnitType_Air)
        {
            yOffset = 40
        }
        sprite.loadSprite("mg_hit",  false, sprite.getMaxUnitCount(), Qt.point(0, yOffset),
                          1, 1.0, 0, 0, true);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("mg_impact.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        return 400 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_RECON.getMaxUnitCount());
    };

    this.getMoveInDurationMS = function(sprite, unit, defender, weapon)
    {
        return 610;
    };

    this.getStopDurationMS = function(sprite, unit, defender, weapon)
    {
        return 500 + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_RECON.getMaxUnitCount());
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_RECON = new Constructor();
