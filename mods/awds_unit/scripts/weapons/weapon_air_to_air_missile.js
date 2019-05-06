var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Missile");
    };
    this.getBaseDamage = function(unit)
    {
        switch(unit.getUnitID())
        {
			// air
			case "DUSTER":
                return 85;
			case "FIGHTER":
                return 55;
			case "BOMBER":
                return 100;
			case "STEALTHBOMBER":
                return 85;
			case "TRANSPORTPLANE":
				return 120;
            case "BLACK_BOMB":
                return 120;
			case "WATERPLANE":
                return 85;
			case "K_HELI":
                return 100;
			case "T_HELI":
                return 100;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_AIR_TO_AIR_MISSILE = new Constructor();
