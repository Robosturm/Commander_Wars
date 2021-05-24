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

        unit.setFuel(80);
        unit.setMaxFuel(80);
        unit.setBaseMovementPoints(12);
        unit.setMinRange(1);
        unit.setMaxRange(1);
        unit.setVision(4);
    };
    this.getBaseCost = function()
    {
        return 5000;
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // load sprites
        unit.loadSprite("hot_tank", false);
        unit.loadSpriteV2("hot_tank+mask", GameEnums.Recoloring_Table);
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
        animation.loadSpriteV2("hot_tank+walk+mask", GameEnums.Recoloring_Table, 1.5);
        animation.loadSprite("hot_tank+walk", false, 1.5);
        animation.setSound("movetire.wav", -2);
        return animation;
    };
    this.getName = function()
    {
        return qsTr("Hot Tank");
    };
    this.canMoveAndFire = function()
    {
        return true;
    };

    this.getDescription = function()
    {
        return qsTr("High mobility. Gains firepower based on the distance moved this turn.");
    };
    this.getUnitType = function()
    {
        return GameEnums.UnitType_Ground;
    };    
    this.getCOSpecificUnit = function(building)
    {
        return true;
    };
    this.getBonusOffensive = function(attacker, atkX, atkY, defender, defX, defY, isDefender, action)
    {
        var bonus = 0;
        var tileBonus = 7;
        if (action !== null &&
            !isDefender)
        {
            var distance =  globals.getDistance(Qt.point(atkX, atkY), attacker.getPosition());
            var pathLength = action.getMovePathLength() - 1;
            if (pathLength > distance)
            {
                // actual calculation
                bonus = tileBonus * pathLength;
            }
            else
            {
                // for ai simulation
                bonus = tileBonus * distance;
            }
        }
        return bonus;
    };
}

Constructor.prototype = UNIT;
var ZCOUNIT_HOT_TANK = new Constructor();
