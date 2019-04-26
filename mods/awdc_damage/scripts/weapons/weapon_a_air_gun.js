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
				return 105;
			case "FIGHTER":
				return 105;
			case "BOMBER":
				return 105;
			case "STEALTHBOMBER":
				return 105;
			case "TRANSPORTPLANE":
				return 105;
            case "BLACK_BOMB":
                return 105;
			case "WATERPLANE":
				return 105;
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
var WEAPON_A_AIR_GUN = new Constructor();
