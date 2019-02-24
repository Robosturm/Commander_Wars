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
				return WEAPON.HighDamage;
			case "FIGHTER":
				return WEAPON.mediumDamage;
			case "BOMBER":
				return WEAPON.softDamage;
			case "STEALTHBOMBER":
				return WEAPON.softDamage;
			case "TRANSPORTPLANE":
				return 120;
			case "WATERPLANE":
				return WEAPON.mediumDamage;
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
var WEAPON_AIR_TO_AIR_MISSILE = new Constructor();
