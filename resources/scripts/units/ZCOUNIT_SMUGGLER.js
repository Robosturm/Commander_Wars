var Constructor = function()
{
    this.getUnitDamageID = function(unit)
    {
        return "RECON";
    };

    this.init = function(unit)
    {
        unit.setAmmo1(10);
        unit.setMaxAmmo1(10);
        unit.setWeapon1ID("WEAPON_RECON_MG");

        unit.setAmmo2(1);
        unit.setMaxAmmo2(1);
        unit.setWeapon2ID("WEAPON_CARGO");

        unit.setFuel(80);
        unit.setMaxFuel(80);
        unit.setBaseMovementPoints(8);
        unit.setMinRange(1);
        unit.setMaxRange(1);
        unit.setVision(2);
    };
    this.getBaseCost = function()
    {
        return 4000;
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // load sprites
        unit.loadSprite("smuggler", false);
        unit.loadSpriteV2("smuggler+mask", GameEnums.Recoloring_Table);
    };
    this.getMovementType = function()
    {
        return "MOVE_TIRE_A";
    };
    this.doWalkingAnimation = function(action)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
        var unitID = unit.getUnitID().toLowerCase();
        animation.loadSpriteV2("smuggler+walk+mask", GameEnums.Recoloring_Table, 1.5);
        animation.loadSprite("smuggler+walk", false, 1.5);
        animation.setSound("movetire.wav", -2);
        return animation;
    };
    this.getName = function()
    {
        return qsTr("Smuggler");
    };
    this.canMoveAndFire = function()
    {
        return true;
    };

    this.getDescription = function()
    {
        return qsTr("High mobility. Can resupply another unit. Gets a free repair and resupply on any building.");
    };
    this.getUnitType = function()
    {
        return GameEnums.UnitType_Ground;
    };
    this.startOfTurn = function(unit)
    {
        var terrain = unit.getTerrain();
        if (terrain !== null)
        {
            var building = terrain.getBuilding();
            if (building !== null)
            {
                var unitX = unit.getX();
                var unitY = unit.getY();
                var repairAmount = 2 + unit.getRepairBonus(Qt.point(unitX, unitY));
                unit.setHp(unit.getHpRounded() + repairAmount);
                unit.refill(false);
                if (!unit.isStealthed(map.getCurrentViewPlayer()))
                {
                    var animationCount = GameAnimationFactory.getAnimationCount();
                    var animation = GameAnimationFactory.createAnimation(unitX, unitY);
                    var width = animation.addText(qsTr("REPAIR"), map.getImageSize() / 2 + 25, 2, 1);
                    animation.addBox("info", map.getImageSize() / 2, 0, width + 32, map.getImageSize(), 400);
                    animation.addSprite("repair", map.getImageSize() / 2 + 8, 1, 400, 1.7);
                    animation.addSound("repair_2.wav");
                    if (animationCount > 0)
                    {
                        GameAnimationFactory.getAnimation(animationCount - 1).queueAnimation(animation);
                    }
                }
            }
        }
    };
    this.getActions = function(unit)
    {
        // returns a string id list of the actions this unit can perform
        return ["ACTION_FIRE", "ACTION_SUPPORTSINGLE_SUPPLY", "ACTION_JOIN", "ACTION_LOAD",
                "ACTION_UNLOAD", "ACTION_WAIT", "ACTION_CO_UNIT_0", "ACTION_CO_UNIT_1"];
    };
    this.getCOSpecificUnit = function(building)
    {
        return true;
    };
}

Constructor.prototype = UNIT;
var ZCOUNIT_SMUGGLER = new Constructor();
