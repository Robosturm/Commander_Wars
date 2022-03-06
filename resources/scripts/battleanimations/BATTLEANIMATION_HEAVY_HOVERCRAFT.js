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

    this.animationData = [["os", [Qt.point(-35, 5), Qt.point(25, 24), Qt.point(19, 48)]],
                          ["bm", [Qt.point(-35, 5), Qt.point(14, 18), Qt.point(15, 42)]],
                          ["ge", [Qt.point(-35, 5), Qt.point(14, 15), Qt.point(17, 34)]],
                          ["yc", [Qt.point(-35, 5), Qt.point(21, 27), Qt.point(24, 48)]],
                          ["ma", [Qt.point(-35, 5), Qt.point(3,  16), Qt.point(1, 38)]],];

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        var player = unit.getOwner();
        var army = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_HEAVY_HOVERCRAFT.armyData);
        var data = Global.getDataFromTable(army, BATTLEANIMATION_HEAVY_HOVERCRAFT.animationData);
        var weaponId = "";
        if (weapon === 1)
        {
            weaponId = "+mg";
        }
        sprite.loadSpriteV2("heavy_hovercraft+" + army + weaponId + "+mask", GameEnums.Recoloring_Matrix,
                            BATTLEANIMATION_HEAVY_HOVERCRAFT.getMaxUnitCount(), data[0]);
        var spriteId = "heavy_hovercraft+" + army + "+prop+mask";
        if (sprite.existResAnim(spriteId))
        {
            sprite.loadSpriteV2(spriteId, GameEnums.Recoloring_Matrix,
                                BATTLEANIMATION_HEAVY_HOVERCRAFT.getMaxUnitCount(), data[0],
                                -1, 1, 0, 0, false, false, 100);
        }
        BATTLEANIMATION.loadSpotterOrCoMini(sprite, unit, false);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_HEAVY_HOVERCRAFT.loadStandingAnimation(sprite, unit, defender, weapon);
        var offset = Qt.point(0, 0);
        var count = sprite.getUnitCount(BATTLEANIMATION_HEAVY_HOVERCRAFT.getMaxUnitCount());
        var player = unit.getOwner();
        var army = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_HEAVY_HOVERCRAFT.armyData);
        var data = Global.getDataFromTable(army, BATTLEANIMATION_HEAVY_HOVERCRAFT.animationData);
        if (weapon === 0)
        {
            offset = data[1];
            sprite.loadSprite("medium_shot",  false, sprite.getMaxUnitCount(), offset,
                              1, 1, 0, 0);
            for (var i = 0; i < count; i++)
            {
                sprite.loadSound("tank_shot.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
        else
        {            
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
            BATTLEANIMATION.showMgBullets(sprite, offset);
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
        return 600 + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_HEAVY_HOVERCRAFT.getMaxUnitCount());
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_HEAVY_HOVERCRAFT = new Constructor();
