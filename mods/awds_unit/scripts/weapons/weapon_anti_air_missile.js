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
                return 120;
			case "WATERPLANE":
                return 100;
            case "BLACK_BOMB":
                return 120;
			case "K_HELI":
                return 115;
			case "T_HELI":
                return 115;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_ANTI_AIR_MISSILE = new Constructor();
