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
				return WEAPON.mediumDamage;
			case "FIGHTER":
				return WEAPON.mediumDamage;
			case "BOMBER":
				return WEAPON.mediumDamage;
			case "STEALTHBOMBER":
				return WEAPON.mediumDamage;
			case "TRANSPORTPLANE":
				return 75;
			case "WATERPLANE":
				return WEAPON.mediumDamage;
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
