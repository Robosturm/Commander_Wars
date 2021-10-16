var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };

    this.armyData = [["os", "os"],
                     ["bm", "bm"],
                     ["ge", "ge"],
                     ["yc", "yc"],
                     ["ac", "ac"],];
    this.animationData = [["os", [Qt.point(37, 28), "artillery_shot"]],
                          ["bm", [Qt.point(37, 28), "artillery_shot"]],
                          ["ge", [Qt.point(37, 28), "artillery_shot"]],
                          ["yc", [Qt.point(39, 28), "artillery_shot"]],
                          ["ac", [Qt.point(34, 24), "light_shot"]],];

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_ANTITANKCANNON.loadSprite(sprite, unit, defender, weapon, "");
    };

    this.loadSprite = function(sprite, unit, defender, weapon, ending)
    {
        var armyName = Global.getArmyNameFromPlayerTable(unit.getOwner(), BATTLEANIMATION_ANTITANKCANNON.armyData);
        sprite.loadSpriteV2("antitankcannon+" + armyName + ending + "+mask", GameEnums.Recoloring_Matrix,
                          BATTLEANIMATION_ANTITANKCANNON.getMaxUnitCount(), Qt.point(-15, 5));
        var spriteId = "antitankcannon+" + armyName;
        if (sprite.existResAnim(spriteId))
        {
            sprite.loadSprite(spriteId,  false,
                              BATTLEANIMATION_ANTITANKCANNON.getMaxUnitCount(), Qt.point(-15, 5));
        }
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        // gun
        BATTLEANIMATION_ANTITANKCANNON.loadSprite(sprite, unit, defender, weapon, "+fire");
        var armyName = Global.getArmyNameFromPlayerTable(unit.getOwner(), BATTLEANIMATION_ANTITANKCANNON.armyData);
        var data = Global.getDataFromTable(armyName, BATTLEANIMATION_ANTITANKCANNON.animationData);
        var offset = data[0];
        var anim = data[1];
        var count = sprite.getUnitCount(BATTLEANIMATION_ANTITANKCANNON.getMaxUnitCount());
        sprite.loadSprite(anim,  false, sprite.getMaxUnitCount(), offset,
                          1, 1, 0, 0);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("cannon_weapon_fire.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
		}
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        return 500 + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_ANTITANKCANNON.getMaxUnitCount());
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION.getMaxUnitCount());
        var i = 0;
        sprite.loadSprite("artillery_hit",  false, sprite.getMaxUnitCount(), Qt.point(0, 20),
                          1, 1.0, 0, 0, true);
        sprite.addSpriteScreenshake(8, 0.98, 800, 200);
        for (i = 0; i < count; i++)
        {
            sprite.loadSound("artillery_explode.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        return 400 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_ANTITANKCANNON.getMaxUnitCount());
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ANTITANKCANNON = new Constructor();
