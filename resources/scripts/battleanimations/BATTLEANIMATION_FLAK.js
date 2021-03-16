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
                     ["bh", "bh"],
                     ["bg", "bh"],
                     ["ma", "ma"],];

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_FLAK.armyData);
        var offset = Qt.point(-5, 5);
        if (armyName === "ma")
        {
            offset = Qt.point(-15, 5);
        }
        sprite.loadSprite("flak+" + armyName,  false,
                          BATTLEANIMATION_FLAK.getMaxUnitCount(), offset);
        sprite.loadSpriteV2("flak+" + armyName + "+mask",  GameEnums.Recoloring_Table,
                            BATTLEANIMATION_FLAK.getMaxUnitCount(), offset);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        // mg
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_FLAK.armyData);

        var offset = Qt.point(-5, 5);

        if (armyName !== "ma")
        {
            if (BATTLEANIMATION.getRelativePosition(unit, defender) > 0)
            {
                sprite.loadSprite("flak+" + armyName + "+fire+air",  false,
                                  BATTLEANIMATION_FLAK.getMaxUnitCount(), offset, 2);
                sprite.loadSpriteV2("flak+" + armyName + "+fire+air+mask", GameEnums.Recoloring_Table,
                                    BATTLEANIMATION_FLAK.getMaxUnitCount(), offset, 2);
                if (armyName === "bh")
                {
                    offset = Qt.point(29, 40);
                }
                else if (armyName === "os")
                {
                    offset = Qt.point(35, 40);
                }
                else if (armyName === "bm")
                {
                    offset = Qt.point(34, 37);
                }
                else if (armyName === "ge")
                {
                    offset = Qt.point(38, 40);
                }
                else if (armyName === "yc")
                {
                    offset = Qt.point(35, 43);
                }
                sprite.loadSprite("mg_shot_air",  false, sprite.getMaxUnitCount(), offset,
                                  2, 1, 0, 0);
            }
            else
            {
                sprite.loadSprite("flak+" + armyName + "+fire+ground",  false,
                                  BATTLEANIMATION_FLAK.getMaxUnitCount(), offset, 2);
                sprite.loadSpriteV2("flak+" + armyName + "+fire+ground+mask",  GameEnums.Recoloring_Table,
                                    BATTLEANIMATION_FLAK.getMaxUnitCount(), offset, 2);
                if (armyName === "bh")
                {
                    offset = Qt.point(31, 31);
                }
                else if (armyName === "os")
                {
                    offset = Qt.point(34, 30);
                }
                else if (armyName === "bm")
                {
                    offset = Qt.point(34, 28);
                }
                else if (armyName === "ge")
                {
                    offset = Qt.point(41, 32);
                }
                else if (armyName === "yc")
                {
                    offset = Qt.point(33, 31);
                }
                sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), offset,
                                  2, 1, 0, 0);
            }
        }
        else
        {
            if (armyName === "ma")
            {
                offset = Qt.point(24, 48);
            }
            BATTLEANIMATION_FLAK.loadStandingAnimation(sprite, unit, defender, weapon);
            sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), offset,
                              2, 1, 0, 0);
        }
    };

    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadColorOverlayForLastLoadedFrame("#969696", 300, 3, 0);
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(5);
        sprite.loadSprite("mg_hit",  false, sprite.getMaxUnitCount(), Qt.point(0, 22),
                          1, 1.0, 0, 0);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("mg_impact.wav", 1, "resources/sounds/", i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 850 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_FLAK.getMaxUnitCount();
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_FLAK = new Constructor();
