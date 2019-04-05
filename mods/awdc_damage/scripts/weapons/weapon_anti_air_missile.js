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
                return 100;
			case "FIGHTER":
                return 100;
			case "BOMBER":
                return 100;
			case "STEALTHBOMBER":
                return 100;
			case "TRANSPORTPLANE":
                return 100;
			case "WATERPLANE":
                return 100;
			case "K_HELI":
                return 120;
			case "T_HELI":
                return 120;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_ANTI_AIR_MISSILE = new Constructor();
