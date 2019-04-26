var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Anti Air Gun");
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
                return 65;
			case "STEALTHBOMBER":
                return 65;
			case "TRANSPORTPLANE":
				return 75;
            case "BLACK_BOMB":
                return 75;
			case "WATERPLANE":
                return 45;
			case "K_HELI":
				return 75;
			case "T_HELI":
				return 75;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_DESTROYER_A_AIR_GUN = new Constructor();
