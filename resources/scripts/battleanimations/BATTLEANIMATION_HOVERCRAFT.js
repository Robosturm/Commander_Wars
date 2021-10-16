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
                     ["bh", "ma"],
                     ["bg", "ma"],
                     ["ma", "ma"],
                     ["ac", "ma"],
                     ["pf", "ma"],
                     ["ti", "ma"],
                     ["dm", "ma"],];

    this.animationData = [["os", [Qt.point(-35, 5), Qt.point(2, 24),  Qt.point(4, 39)]],
                          ["bm", [Qt.point(-35, 5), Qt.point(10, 20), Qt.point(13, 36)]],
                          ["ge", [Qt.point(-35, 5), Qt.point(10, 19), Qt.point(14, 33)]],
                          ["yc", [Qt.point(-35, 5), Qt.point(8, 19),  Qt.point(14, 37)]],
                          ["ma", [Qt.point(-45, 5), Qt.point(0, 28),  Qt.point(13, 33)]],];

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_HOVERCRAFT.loadSprite(sprite, unit, defender, weapon);
    };

    this.loadSprite = function(sprite, unit, defender, weapon, ending = "", loops = 1)
    {
        var player = unit.getOwner();
        var army = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_HOVERCRAFT.armyData);
        var data = Global.getDataFromTable(army, BATTLEANIMATION_HOVERCRAFT.animationData);
        var weaponId = "";
        if (weapon === 1)
        {
            weaponId = "+mg";
        }
        sprite.loadSpriteV2("hovercraft+" + army + weaponId + ending + "+mask", GameEnums.Recoloring_Matrix,
                            BATTLEANIMATION_HOVERCRAFT.getMaxUnitCount(), data[0], loops);
        var spriteId = "hovercraft+" + army + "+prop+mask";
        if (sprite.existResAnim(spriteId))
        {
            sprite.loadSpriteV2(spriteId, GameEnums.Recoloring_Matrix,
                                BATTLEANIMATION_HOVERCRAFT.getMaxUnitCount(), data[0],
                                -1, 1, 0, 0, false, false, 100);
        }
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        var offset = Qt.point(0, 0);
        var count = sprite.getUnitCount(BATTLEANIMATION_HOVERCRAFT.getMaxUnitCount());
        var player = unit.getOwner();
        var army = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_HOVERCRAFT.armyData);
        var data = Global.getDataFromTable(army, BATTLEANIMATION_HOVERCRAFT.animationData);
        if (weapon === 0)
        {
            BATTLEANIMATION_HOVERCRAFT.loadSprite(sprite, unit, defender, weapon, "+fire");
            offset = data[1];
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
            offset = data[2];
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
        return 600 + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_HOVERCRAFT.getMaxUnitCount());
    };

};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_HOVERCRAFT = new Constructor();
