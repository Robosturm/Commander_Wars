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
    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_BOMBER.loadSprite(sprite, unit, defender, weapon, Qt.point(0, 0), 0);
    };

    this.loadSprite = function(sprite, unit, defender, weapon, movement, moveTime)
    {
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_BOMBER.armyData);
        sprite.loadMovingSprite("bomber+" + armyName,  false,
                                BATTLEANIMATION_BOMBER.getMaxUnitCount(), Qt.point(0, 40), movement, moveTime);
        sprite.loadMovingSpriteV2("bomber+" + armyName + "+mask", GameEnums.Recoloring_Table,
                                  BATTLEANIMATION_BOMBER.getMaxUnitCount(), Qt.point(0, 40), movement, moveTime);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_BOMBER.loadStandingAnimation(sprite, unit, defender, weapon);
        var count = sprite.getUnitCount(5);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSingleMovingSprite("bombs_projectile", false, Qt.point(70, 50),
                                          Qt.point(0, -70), 400, false,
                                          1, 1, -1, i * 150);
        }
        sprite.loadSound("falling_bomb.wav", 1, "resources/sounds/", 0);
    };

    this.getFireDurationMS = function()
    {
        // the time will be scaled with animation speed inside the engine
        return 1250;
    };

    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadColorOverlayForLastLoadedFrame("#969696", 1000, 1, 300);
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(5);
        sprite.loadSprite("unit_explosion",  false, 5, Qt.point(0, 60),
                          1, 1.0, 0, 300);
        sprite.loadMovingSprite("bomb_falling", false, 5, Qt.point(0, 150),
                                Qt.point(0, -130), 400, true,
                                1, 1, 0, 0, true);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("impact_explosion.wav", 1, "resources/sounds/", i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        // should be a second or longer.
        // the time will be scaled with animation speed inside the engine
        return 1500;
    };

    this.getDyingDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 1000;
    };

    this.hasDyingAnimation = function()
    {
        // return true if the unit has an implementation for loadDyingAnimation
        return true;
    };

    this.loadDyingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_BOMBER.loadSprite(sprite, unit, defender, weapon, Qt.point(-140, -140), 600);
    };

};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_BOMBER = new Constructor();
