var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSpriteV2("crystal_tank+mask", GameEnums.Recoloring_Matrix,
                          BATTLEANIMATION_ZCOUNIT_CRYSTAL_TANK.getMaxUnitCount(), Qt.point(-5, 10));
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_ZCOUNIT_CRYSTAL_TANK.loadStandingAnimation(sprite, unit, defender, weapon);
        var offset = Qt.point(0, 0);
        if (weapon === 0)
        {
            sprite.loadSpriteV2("crystal_tank+fire", GameEnums.Recoloring_None,
                              BATTLEANIMATION_ZCOUNIT_CRYSTAL_TANK.getMaxUnitCount(), Qt.point(-5, 10));
            offset = Qt.point(83, 50);
            sprite.loadSingleMovingSprite("crystalcannon", false,
                                          Qt.point(offset.x, offset.y),
                                          Qt.point(0, 0), 0, true,
                                          1, 1.0, 1, 0, false);
            var enemySprite = sprite.getEnemySprite();
            enemySprite.setInvertStartPosition(true);
            enemySprite.loadSingleMovingSprite("crystalcannon_trail", false,
                                               Qt.point(0, offset.y),
                                               Qt.point(0, 0), 0, true,
                                               1, 1.0, 5, 0, false);
            enemySprite.loadSingleMovingSprite("crystalcannon_trail", false,
                                               Qt.point(64, offset.y),
                                               Qt.point(0, 0), 0, true,
                                               1, 1.0, 5, 0, false);
            enemySprite.setInvertStartPosition(false);
        }
        else
        {
            // mg
            offset = Qt.point(83, 68);
            sprite.loadSprite("flak_shot",  false, sprite.getMaxUnitCount(), offset,
                              1, 1, 0, 0, false, true);
            sprite.loadSound("mg_weapon_fire.wav", 1);
        }
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
		var count = sprite.getUnitCount(5);
        var i = 0;
        if (weapon === 0)
        {
            sprite.loadSprite("cannon_hit",  false, 5, Qt.point(0, 20),
                            1, 1.0, 0, 0, true);
            sprite.addSpriteScreenshake(8, 0.95, 800, 200);
            for (i = 0; i < count; i++)
            {
                sprite.loadSound("impact_explosion.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
        else
        {
            var xOffset = 0;
            var yOffset = 22;
            if (defender.getUnitType()  === GameEnums.UnitType_Air)
            {
                yOffset = 45;
            }
            sprite.loadSprite("flak_hit",  false, 5, Qt.point(xOffset, yOffset),
                            1, 1.0, 0, 0, true);
            for (i = 0; i < count; i++)
            {
                sprite.loadSound("mg_impact.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
			}
        }
    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(5);
        if (weapon === 0)
        {
            return 300 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * count;
        }
        else
        {
            return 400 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * count;
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        if (weapon === 0)
        {
            return 1900;
        }
        else
        {
            return 800;
        }
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ZCOUNIT_CRYSTAL_TANK = new Constructor();
