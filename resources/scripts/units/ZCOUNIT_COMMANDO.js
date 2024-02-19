var Constructor = function()
{
    this.getUnitDamageID = function(unit)
    {
        return "MECH";
    };
    this.init = function(unit)
    {
        unit.setAmmo1(3);
        unit.setMaxAmmo1(3);
        unit.setWeapon1ID("WEAPON_BAZOOKA");

        unit.setAmmo2(10);
        unit.setMaxAmmo2(10);
        unit.setWeapon2ID("WEAPON_RECON_MG");

        unit.setFuel(100);
        unit.setMaxFuel(100);
        unit.setBaseMovementPoints(4);
        unit.setMinRange(1);
        unit.setMaxRange(1);
        unit.setVision(3);
    };
    this.getBaseCost = function()
    {
        return 5000;
    };

    this.loadSprites = function(unit)
    {
        unit.loadSpriteV2("commando+mask", GameEnums.Recoloring_Matrix);
    };
    this.getMovementType = function()
    {
        return "MOVE_MECH";
    };
    this.actionList = ["ACTION_FIRE", "ACTION_MISSILE", "ACTION_CAPTURE", "ACTION_JOIN", "ACTION_LOAD", "ACTION_WAIT", "ACTION_CO_UNIT_0", "ACTION_CO_UNIT_1"];
    this.doWalkingAnimation = function(action, map)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(map, unit, action);
        animation.loadSpriteV2("commando+walk+mask", GameEnums.Recoloring_Matrix, 2);
        animation.setSound("moveboots.wav", -2);
        return animation;
    };
    this.getName = function()
    {
        return qsTr("Commando");
    };
    this.canMoveAndFire = function()
    {
        return true;
    };

    this.getDescription = function()
    {
        return qsTr("<r>Attack power high. Can </r><div c='#00ff00'>capture </div><r> bases. </r><div c='#00ff00'>Vision +3 </div><r> when on mountains and gets cloaked in addition.</r> Moving out of a mountain removes any cloaks from the Commando Unit.");
    };
    this.getUnitType = function()
    {
        return GameEnums.UnitType_Infantry;
    };
    this.startOfTurn = function(unit, map)
    {
        ZCOUNIT_COMMANDO.cloak(unit, map);
    };

    this.cloak = function(unit, map)
    {
        var terrain = unit.getTerrain();
        if (terrain !== null)
        {
            var terrainId = terrain.getID();
            if (terrainId === "MOUNTAIN" ||
                terrainId === "SNOW_MOUNTAIN" ||
                terrainId === "DESERT_ROCK" ||
                terrainId === "WASTE_MOUNTAIN")
            {
                var cloaked = unit.getCloaked();
                unit.setCloaked(1);
                var cloakedNow = unit.getCloaked();
                if (cloaked !== cloakedNow)
                {
                    var animationCount = GameAnimationFactory.getAnimationCount();
                    var queueAnimation = null;
                    if (animationCount > 0)
                    {
                        queueAnimation = GameAnimationFactory.getAnimation(animationCount - 1);
                    }
                    var animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
                    animation.addSprite("stealth", -map.getImageSize() / 2, -map.getImageSize() / 2, 0, 2);
                    animation.setSound("stealth.wav", 1);
                    if (queueAnimation !== null)
                    {
                        queueAnimation.queueAnimation(animation);
                    }
                }
            }
            else
            {
                var cloaked = unit.getCloaked();
                unit.removeCloaked();
                var cloakedNow = unit.getCloaked();
                if (cloaked !== cloakedNow)
                {
                    var animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
                    animation.addSprite("stealth", -map.getImageSize() / 2, -map.getImageSize() / 2, 0, 2);
                    animation.setSound("unstealth.wav", 1);
                }
            }
        }
    };
    this.postAction = function(unit, action, map)
    {
        ZCOUNIT_COMMANDO.cloak(unit, map);
    };
    this.getCOSpecificUnit = function(building)
    {
        return true;
    };
    this.getEditorPlacementSound = function()
    {
        return "moveboots.wav";
    };
}

Constructor.prototype = UNIT;
var ZCOUNIT_COMMANDO = new Constructor();
