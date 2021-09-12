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
                     ["bh", ""],
                     ["bg", ""],
                     ["ma", ""],
                     ["ac", ""],
                     ["pf", ""],
                     ["ti", ""],
                     ["dm", ""],];

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_HOVERCRAFT.loadSprite(sprite, unit, defender, weapon);
    };

    this.loadSprite = function(sprite, unit, defender, weapon, ending = "", loops = 1)
    {
        var player = unit.getOwner();
        var army = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_HOVERCRAFT.armyData);
        if (army !== "")
        {
            var weaponId = "";
            if (weapon === 1)
            {
                weaponId = "+mg";
            }
            sprite.loadSpriteV2("hovercraft+" + army + weaponId + ending + "+mask", GameEnums.Recoloring_Matrix,
                                BATTLEANIMATION_HOVERCRAFT.getMaxUnitCount(), Qt.point(-35, 5), loops);
            sprite.loadSpriteV2("hovercraft+" + army + "+prop+mask", GameEnums.Recoloring_Matrix,
                                BATTLEANIMATION_HOVERCRAFT.getMaxUnitCount(), Qt.point(-35, 5),
                                -1, 1, 0, 0, false, false, 100);
        }
        else
        {
            sprite.loadSpriteV2("hovercraft+mask", GameEnums.Recoloring_Matrix,
                                BATTLEANIMATION_HOVERCRAFT.getMaxUnitCount(), Qt.point(-45, 5));
        }
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        var offset = Qt.point(0, 0);
        var count = sprite.getUnitCount(BATTLEANIMATION_HOVERCRAFT.getMaxUnitCount());
        var player = unit.getOwner();
        var army = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_HOVERCRAFT.armyData);
        if (weapon === 0)
        {
            BATTLEANIMATION_HOVERCRAFT.loadSprite(sprite, unit, defender, weapon, "+fire");
            offset = Qt.point(0, 28);
            if (army === "os")
            {
                offset = Qt.point(2, 24);
            }
            else if (army === "ge")
            {
                offset = Qt.point(10, 19);
            }
            else if (army === "bm")
            {
                offset = Qt.point(10, 20);
            }
            else if (army === "yc")
            {
                offset = Qt.point(8, 19);
            }
            sprite.loadSprite("light_shot",  false, sprite.getMaxUnitCount(), offset,
                              1, 1, 0, 0);
            for (var i = 0; i < count; i++)
            {
                sprite.loadSound("tank_shot.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
        else
        {
            BATTLEANIMATION_HOVERCRAFT.loadSprite(sprite, unit, defender, weapon, "+fire", 3);
            offset = Qt.point(13, 33);
            if (army === "os")
            {
                offset = Qt.point(4, 39);
            }
            else if (army === "ge")
            {
                offset = Qt.point(14, 33);
            }
            else if (army === "bm")
            {
                offset = Qt.point(13, 36);
            }
            else if (army === "yc")
            {
                offset = Qt.point(14, 37);
            }
            if (BATTLEANIMATION.getRelativePosition(unit, defender) > 0)
            {
                offset.y += 5;
                sprite.loadSprite("mg_shot_air",  false, sprite.getMaxUnitCount(), offset,
                                  1, 1, 0, 0);
            }
            else
            {
                sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), offset,
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
        // the time will be scaled with animation speed inside the engine
        return 500 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_HOVERCRAFT.getMaxUnitCount();
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_HOVERCRAFT = new Constructor();
