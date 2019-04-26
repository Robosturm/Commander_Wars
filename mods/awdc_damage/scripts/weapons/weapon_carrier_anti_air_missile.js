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
				return 55;
			case "FIGHTER":
				return 45;
			case "BOMBER":
				return 55;
			case "STEALTHBOMBER":
				return 55;
			case "TRANSPORTPLANE":
				return 75;
            case "BLACK_BOMB":
                return 75;
			case "WATERPLANE":
				return 55;
			case "K_HELI":
				return 75;
			case "T_HELI":
				return 85;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_CARRIER_ANTI_AIR_MISSILE = new Constructor();
