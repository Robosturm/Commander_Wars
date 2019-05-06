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
                return 100;
			case "FIGHTER":
                return 85;
			case "BOMBER":
                return 100;
			case "STEALTHBOMBER":
				return 105;
			case "TRANSPORTPLANE":
                return 120;
			case "WATERPLANE":
                return 100;
			case "K_HELI":
                return 105;
			case "T_HELI":
                return 105;
			default:
				return -1;
        }
    };
};

Constructor.prototype = WEAPON;
var WEAPON_A_AIR_GUN = new Constructor();
