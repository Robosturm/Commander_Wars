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

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        var armyName = Global.getArmyNameFromPlayerTable(unit.getOwner(), BATTLEANIMATION_ANTITANKCANNON.armyData);
        sprite.loadSpriteV2("antitankcannon+" + armyName + "+mask", GameEnums.Recoloring_Matrix,
                          BATTLEANIMATION_ANTITANKCANNON.getMaxUnitCount(), Qt.point(-15, 5));
        sprite.loadSprite("antitankcannon+" + armyName,  false,
                          BATTLEANIMATION_ANTITANKCANNON.getMaxUnitCount(), Qt.point(-15, 5));
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        // gun
        BATTLEANIMATION_ANTITANKCANNON.loadStandingAnimation(sprite, unit, defender, weapon);
        var armyName = Global.getArmyNameFromPlayerTable(unit.getOwner(), BATTLEANIMATION_ANTITANKCANNON.armyData);
        var offset = Qt.point(31, 30);
        var anim = "light_shot";
        if (armyName === "ac")
        {
            //  49  27
            // -15  5
            //  0   8
            offset = Qt.point(34, 24);
            anim = "light_shot";
        }
        else if (armyName === "os" ||
                 armyName === "bm" ||
                 armyName === "ge" ||
                 armyName === "yc")
        {
            //  52  32
            // -15  5
            // -7   9
            offset = Qt.point(30, 28);
            anim = "artillery_shot";
        }

        var count = sprite.getUnitCount(BATTLEANIMATION_ANTITANKCANNON.getMaxUnitCount());
        sprite.loadSprite(anim,  false, sprite.getMaxUnitCount(), offset,
                          1, 1, 0, 0);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("cannon_weapon_fire.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
		}
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION.getMaxUnitCount());
        var i = 0;
        sprite.loadSprite("artillery_hit",  false, sprite.getMaxUnitCount(), Qt.point(0, 20),
                          1, 1.0, 0, 0);
        sprite.addSpriteScreenshake(8, 0.98, 800, 200);
        for (i = 0; i < count; i++)
        {
            sprite.loadSound("artillery_explode.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 500 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_ANTITANKCANNON.getMaxUnitCount();
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ANTITANKCANNON = new Constructor();
