var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };
    this.armyData = [["bm", "bm"],
                     ["bh", "bh"],
                     ["bg", "bg"],
                     ["ge", "ge"],
                     ["ma", "ma"],
                     ["os", "os"],
                     ["pf", "pf"],
                     ["ti", "ti"],
                     ["yc", "yc"],];
    this.animationData = [["bm", [Qt.point(-40, 20), Qt.point(-50, 30)]],
                          ["bh", [Qt.point(-40, 20), Qt.point(-50, 30)]],
                          ["bg", [Qt.point(-40, 20), Qt.point(-50, 30)]],
                          ["ge", [Qt.point(-40, 20), Qt.point(-50, 30)]],
                          ["ma", [Qt.point(-50, 20), Qt.point(-50, 30)]],
                          ["os", [Qt.point(-40, 20), Qt.point(-50, 30)]],
                          ["pf", [Qt.point(-40, 20), Qt.point(-50, 30)]],
                          ["ti", [Qt.point(-40, 20), Qt.point(-50, 30)]],
                          ["yc", [Qt.point(-40, 20), Qt.point(-50, 30)]],];
    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_FIGHTER.loadSprite(sprite, unit, defender, weapon, "");
    };

    this.loadStandingFiredAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_FIGHTER.loadSprite(sprite, unit, defender, weapon, "+fire");
    };

    this.loadSprite = function(sprite, unit, defender, weapon, ending)
    {
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_FIGHTER.armyData);
        var data = Global.getArmyDataFromTable(armyName, BATTLEANIMATION_FIGHTER.animationData);
        var offset = data[0];
        sprite.loadSpriteV2("fighter+" + armyName + ending + "+mask", GameEnums.Recoloring_Matrix,
                            BATTLEANIMATION_FIGHTER.getMaxUnitCount(), offset, -1);
    };


    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_FIGHTER.loadSprite(sprite, unit, defender, weapon, "+fire");
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_FIGHTER.armyData);
        var data = Global.getArmyDataFromTable(armyName, BATTLEANIMATION_FIGHTER.animationData);
        var offset = data[1];
        var count = sprite.getUnitCount(BATTLEANIMATION_FIGHTER.getMaxUnitCount());        
        sprite.loadMovingSprite("rocket", false, sprite.getMaxUnitCount(), offset,
                                Qt.point(227, 0), 800, false,
                                -1, 1, -1);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("missile_weapon_fire.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        return 800 + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_FIGHTER.getMaxUnitCount());
    };

    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadColorOverlayForLastLoadedFrame("#969696", 1000, 1, 300);
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_FIGHTER.getMaxUnitCount());
        sprite.loadMovingSprite("rocket_hit_air", false, sprite.getMaxUnitCount(), Qt.point(0, 30),
                                Qt.point(-50, 0), 300, false,
                                1, 1.0, 0, 400, true);
        sprite.addSpriteScreenshake(8, 0.95, 800, 500);
        sprite.loadMovingSprite("rocket", false, sprite.getMaxUnitCount(), Qt.point(127, 30),
                                Qt.point(-127, 0), 400, true,
                                -1, 1, 0, 0, true);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("rocket_flying.wav", 1, 0);
            sprite.loadSound("rockets_explode.wav", 1, 200 + i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        return 600 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_FIGHTER.getMaxUnitCount());
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_FIGHTER = new Constructor();
