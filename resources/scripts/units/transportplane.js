var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(0);
        unit.setMaxAmmo1(0);
        unit.setWeapon1ID("");
        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");
        unit.setFuel(100);
        unit.setMaxFuel(100);
        unit.setBaseMovementPoints(7);
        unit.setMinRange(1);
        unit.setMaxRange(1);
		unit.setVision(3);
    };
    this.getBaseCost = function()
    {
        return 20000;
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // load sprites
        unit.loadSprite("transportplane", false);
        unit.loadSprite("transportplane+mask", true);
    };
    this.getMovementType = function()
    {
        return "MOVE_AIR";
    };
    this.getName = function()
    {
        return qsTr("Transportplane");
    };
    this.createExplosionAnimation = function(x, y)
    {
        var animation = GameAnimationFactory.createAnimation(x, y);
        animation.addSprite("explosion+air", 0, -map.getImageSize(), 0, 1.5);
        return animation;
    };
    this.getLoadingPlace = function()
    {
        return 2;
    };
    this.getTransportUnits = function()
    {
        return ["ANTITANKCANNON", "APC", "ARTILLERY", "FLAK", "FLARE",
                "HEAVY_HOVERCRAFT", "HEAVY_TANK", "HOVERCRAFT", "HOVERFLAK",
                "INFANTRY", "LIGHT_TANK", "MECH", "MEGATANK", "MISSILE",
                "MOTORBIKE", "NEOTANK", "RECON", "ROCKETTRHOWER", "SNIPER"];
    };
}

Constructor.prototype = UNIT;
var TRANSPORTPLANE = new Constructor();
