var UNIT =
{
    init : function(unit)
    {
        unit.setAmmo1(0);
        unit.setMaxAmmo1(0);
        unit.setWeapon1ID("");
        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");
        unit.setFuel(99);
        unit.setMaxFuel(99);
        unit.setMinRange(1);
        unit.setMaxRange(1);
        unit.setVision(1);
    },
    loadSprites : function(unit)
    {
    },
    getMovementType : function()
    {
        return "";
    },
    getActions : function()
    {
        // returns a string id list of the actions this unit can perform
        return "ACTION_FIRE,ACTION_JOIN,ACTION_LOAD,ACTION_UNLOAD,ACTION_WAIT,ACTION_CO_UNIT_0,ACTION_CO_UNIT_1";
    },

    getName : function()
    {
        return "";
    },

    getBaseCost : function()
    {
        return -1;
    },

    createExplosionAnimation : function(x, y)
    {
        var animation = GameAnimationFactory.createAnimation(x, y);
        animation.addSprite("explosion+land", -map.getImageSize() / 2, -map.getImageSize(), 0, 1.5);
        audio.playSound("explosion+land.wav");
        return animation;
    },

    doWalkingAnimation : function(action)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
        var unitID = unit.getUnitID().toLowerCase();
        animation.loadSprite(unitID + "+walk+mask", true);
        animation.loadSprite(unitID + "+walk", false);
        return animation;
    },

    startOfTurn : function(unit)
    {
        // gets called at the start of a turn
    },

    canMoveAndFire : function()
    {
        return false;
    },
    // number of units that can be loaded by this unit
    getLoadingPlace : function()
    {
        return 0;
    },
    useTerrainDefense : function()
    {
        return true;
    },
    getTransportUnits : function()
    {
        return [];
    },

    transporterRefilling : function (unit)
    {
        // carrier refilling and unmoving is done here
        for (var i = 0; i < unit.getLoadedUnitCount(); i++)
        {
            var transportUnit = unit.getLoadedUnit(i);
            transportUnit.refill();
            transportUnit.setHasMoved(false);
        }
    },

    repairUnit : function (unit, repairAmount)
    {
        // repair it
        var costs = unit.getUnitCosts();
        var hp = unit.getHpRounded();
        var healingDone = 0;
        if (hp + repairAmount <= 10)
        {
            healingDone = repairAmount;
        }
        else
        {
            // we could heal more than we need
            healingDone = 10 - hp;
        }
        var fonds = unit.getOwner().getFonds();
        // check if we can pay for all healing
        for (var i = healingDone; i >= 0; i--)
        {
            if (i * costs / 10  <= fonds)
            {
                healingDone = i;
                break;
            }
            else if (i === 0)
            {
                healingDone = 0;
                break;
            }
        }
        // heal unit
        unit.setHp(hp + healingDone);
        // pay for healing
        unit.getOwner().addFonds(-healingDone / 10 * costs);
    },

    getTerrainAnimationBase : function(unit, terrain)
    {
        return Global[terrain.getID()].getTerrainAnimationBase(unit, terrain);
    },

    getTerrainAnimationForeground : function(unit, terrain)
    {
        return Global[terrain.getID()].getTerrainAnimationForeground(unit, terrain);
    },

    getTerrainAnimationBackground : function(unit, terrain)
    {
        return Global[terrain.getID()].getTerrainAnimationBackground(unit, terrain);
    },

    getDescription : function()
    {
        return "";
    },
};
