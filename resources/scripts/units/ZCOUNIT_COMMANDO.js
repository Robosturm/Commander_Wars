var Constructor = function()
{
    this.getUnitDamageID = function(unit)
    {
        return "MECH";
    };
    this.init = function(unit)
    {
        unit.setAmmo1(10);
        unit.setMaxAmmo1(10);
        unit.setWeapon1ID("WEAPON_RECON_MG");

        unit.setAmmo2(3);
        unit.setMaxAmmo2(3);
        unit.setWeapon2ID("WEAPON_BAZOOKA");

        unit.setFuel(99);
        unit.setMaxFuel(99);
        unit.setBaseMovementPoints(4);
        unit.setMinRange(1);
        unit.setMaxRange(1);
        unit.setVision(3);
    };
    this.getBaseCost = function()
    {
        return 4000;
    };

    this.loadSprites = function(unit)
    {
        // load sprites
        unit.loadSprite("commando", false, 1.5);
        unit.loadSpriteV2("commando+mask", GameEnums.Recoloring_Table, 1.5);
    };
    this.getMovementType = function()
    {
        return "MOVE_MECH";
    };
    this.getActions = function()
    {
        // returns a string id list of the actions this unit can perform
        return "ACTION_FIRE,ACTION_MISSILE,ACTION_CAPTURE,ACTION_JOIN,ACTION_LOAD,ACTION_WAIT,ACTION_CO_UNIT_0,ACTION_CO_UNIT_1";
    };
    this.doWalkingAnimation = function(action)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
        // none neutral player
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, MECH.armyData);
        var zoom = 1.5;
        animation.loadSpriteV2("commando+walk+mask", GameEnums.Recoloring_Table, zoom);
        animation.loadSprite("commando+walk", false, zoom);
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
    this.startOfTurn = function(unit)
    {
        ZCOUNIT_COMMANDO.cloak(unit);
    };

    this.cloak = function(unit)
    {
        var terrain = unit.getTerrain();
        if (terrain !== null)
        {
            var terrainId = terrain.getID();
            if (terrainId === "MOUNTAIN" ||
                terrainId === "SNOW_MOUNTAIN" ||
                terrainId === "DESERT_ROCK")
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
                    var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
                    animation.addSprite("stealth", -map.getImageSize() / 2, -map.getImageSize() / 2, 0, 1.5);
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
                    var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
                    animation.addSprite("stealth", -map.getImageSize() / 2, -map.getImageSize() / 2, 0, 1.5);
                    animation.setSound("unstealth.wav", 1);
                }
            }
        }
    };
    this.postAction = function(unit, action)
    {
        ZCOUNIT_COMMANDO.cloak(unit);
    };
    this.getCOSpecificUnit = function(building)
    {
        return true;
    };
}

Constructor.prototype = UNIT;
var ZCOUNIT_COMMANDO = new Constructor();
