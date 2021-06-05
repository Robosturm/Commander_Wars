var UNIT =
{
    getUnitDamageID : function(unit)
    {
        // empty string will be replaced by the actual unit id to find the damage value in the table
        // else the given string is used to get the entry
        // return something like BOMBER to give your custom unit the same weakness as a normal bomber
        return "";
    },

    getUnitDamage : function(weaponID, defender)
    {
        // weaponID string of the used weaponID
        // defender pointer to the which is attacked or used to check how much damage we will deal to it
        // gets called if no normal damage entry was found.
        // use this function to impl the damage this unit takes from certain weapons
        return -1;
    },

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
    initForMods : function(unit)
    {
        // you can use this init function to extend
    },

    loadSprites : function(unit)
    {
    },
    getMovementType : function(unit)
    {
        return "";
    },
    getActions : function(unit)
    {
        // returns a string id list of the actions this unit can perform
        return ["ACTION_FIRE", "ACTION_JOIN", "ACTION_LOAD", "ACTION_UNLOAD", "ACTION_WAIT", "ACTION_CO_UNIT_0", "ACTION_CO_UNIT_1"];
    },

    getName : function(unit)
    {
        return "";
    },

    getBaseCost : function()
    {
        return -1;
    },

    createExplosionAnimation : function(x, y, unit)
    {
        var animation = GameAnimationFactory.createAnimation(x, y);
        animation.addSprite("explosion+land", -map.getImageSize() / 2, -map.getImageSize(), 0, 1.5);
        animation.setSound("explosion+land.wav");
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

    canMoveAndFire : function(unit)
    {
        return false;
    },
    // number of units that can be loaded by this unit
    getLoadingPlace : function(unit)
    {
        return 0;
    },
    useTerrainDefense : function(unit)
    {
        return true;
    },
    getTransportUnits : function(unit)
    {
        return [];
    },

    transporterRefilling : function (unit)
    {
        // carrier refilling and unmoving is done here
        for (var i = 0; i < unit.getLoadedUnitCount(); i++)
        {
            var transportUnit = unit.getLoadedUnit(i);
            var refillMaterial = (typeof refillRule === 'undefined' || refillRule === null); // an existing rule equals it's set
            transportUnit.refill(refillMaterial);
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
        var funds = unit.getOwner().getFunds();
        // check if we can pay for all healing
        for (var i = healingDone; i >= 0; i--)
        {
            if (i * costs / 10  <= funds)
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
        unit.getOwner().addFunds(-healingDone / 10 * costs);
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

    getTerrainAnimationMoveSpeed : function(unit)
    {
        return 0;
    },

    getDescription : function(unit)
    {
        return "";
    },
    getUnitType : function(unit)
    {
        return GameEnums.UnitType_Ground;
    },

    // unit boosts
    postBattleActions : function(unit, damage, otherUnit, gotAttacked, weapon)
    {
        // unit the unit getting postBattleAction applied
        // damage taken (if gotAttacked = true) or dealt (if gotAttacked = false)
        // otherUnit the other unit involved in the battle
        // gotAttacked if true we're defending else we're attacking
        // this function gets called twice for a unit for one attack.
    },
    postAction : function(unit, action)
    {
    },
    getBonusOffensive : function(attacker, atkX, atkY, defender, defX, defY, isDefender, action)
    {
        return 0;
    },
    getBonusDefensive : function(defender, defX, defY, attacker, atkX, atkY, isDefender, action)
    {
        return 0;
    },

    canAttackStealthedUnit : function(attacker, defender)
    {
        var attackerType = attacker.getUnitType();
        attackerType = UNIT.unitTypeToGround(attackerType);
        var defenderType = defender.getUnitType();
        defenderType = UNIT.unitTypeToGround(defenderType);
        if (attacker.getBaseMaxRange() === 1)
        {
            if (attackerType === defenderType)
            {
                return true;
            }
        }
        if (defender.getCloaked() && !defender.getHidden())
        {
            return true;
        }
        return false;
    },

    unitTypeToGround : function(unitType)
    {
        if (unitType === GameEnums.UnitType_Hovercraft ||
            unitType === GameEnums.UnitType_Ground)
        {
            return GameEnums.UnitType_Ground;
        }
        return unitType;
    },

    getCOSpecificUnit : function(building)
    {
        return false;
    },

    getUnitTypeText : function(unitType)
    {
        switch (unitType)
        {
        case GameEnums.UnitType_Air:
            return qsTr("Air");
        case GameEnums.UnitType_Naval:
            return qsTr("Naval");
        case GameEnums.UnitType_Ground:
            return qsTr("Ground");
        case GameEnums.UnitType_Infantry:
            return qsTr("Infantry");
        case GameEnums.UnitType_Hovercraft:
            return qsTr("Hovercraft");
        }
        return tr("Ground");
    },

    unitTypeSortList : [GameEnums.UnitType_Infantry,
                        GameEnums.UnitType_Ground,
                        GameEnums.UnitType_Hovercraft,
                        GameEnums.UnitType_Air,
                        GameEnums.UnitType_Naval],

    getUnitTypeSort : function()
    {
        return UNIT.unitTypeSortList;
    },

    unitInfantrySortList : ["INFANTRY", "MECH", "MOTORBIKE", "SNIPER"],
    unitHovercraftSortList : ["ARTILLERYCRAFT", "HOVERCRAFT", "HOVERFLAK", "HEAVY_HOVERCRAFT"],
    unitAirSortList : ["T_HELI", "K_HELI", "DUSTER", "TRANSPORTPLANE", "BOMBER", "FIGHTER", "STEALTHBOMBER", "BLACK_BOMB"],
    unitGroundSortList : ["RECON", "APC", "FLARE", "ARTILLERY", "LIGHT_TANK", "FLAK", "ANTITANKCANNON", "HEAVY_TANK", "NEOTANK", "MISSILE", "ROCKETTHROWER", "MEGATANK", "PIPERUNNER"],
    unitNavalSortList : ["CANNONBOAT", "BLACK_BOAT", "LANDER", "CRUISER", "SUBMARINE", "DESTROYER", "BATTLESHIP", "AIRCRAFTCARRIER"],
    getUnitTypeSort1 : function()
    {
        return UNIT.unitGroundSortList;
    },
    getUnitTypeSort2 : function()
    {
        return UNIT.unitHovercraftSortList;
    },
    getUnitTypeSort4 : function()
    {
        return UNIT.unitInfantrySortList;
    },
    getUnitTypeSort8 : function()
    {
        return UNIT.unitAirSortList;
    },
    getUnitTypeSort16 : function()
    {
        return UNIT.unitNavalSortList;
    },
    getTypeOfWeapon1 : function(unit)
    {
        // changes for which ranges the ammo can be used
        // WeaponType_Direct
        // WeaponType_Indirect
        return GameEnums.WeaponType_Both;
    },

    getTypeOfWeapon2 : function(unit)
    {
        // changes for which ranges the ammo can be used.
        // WeaponType_Direct
        // WeaponType_Indirect
        return GameEnums.WeaponType_Both;
    },

    onKilled : function(animation)
    {
        animation.seekBuffer();
        var x = animation.readDataInt32();
        var y = animation.readDataInt32();
        var unit = map.getTerrain(x, y).getUnit();
        if (unit !== null)
        {
            unit.removeUnit();
        }
    },
};
