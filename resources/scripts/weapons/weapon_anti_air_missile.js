var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Anti Air Missile");
    };
    this.getBaseDamage = function(unit)
    {
        switch(unit.getUnitID())
        {
			case "DUSTER":
				return 150;
			case "FIGHTER":
				return 150;
			case "BOMBER":
				return 150;
			case "STEALTHBOMBER":
				return 150;
			case "TRANSPORTPLANE":
				return 150;
			case "WATERPLANE":
				return 150;
			case "K_HELI":
				return 170;
			case "T_HELI":
				return 170;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_ANTI_AIR_MISSILE = new Constructor();