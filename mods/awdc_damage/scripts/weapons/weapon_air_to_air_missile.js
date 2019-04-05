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
                return 80;
			case "FIGHTER":
                return 55;
			case "BOMBER":
                return 65;
			case "STEALTHBOMBER":
                return 65;
			case "TRANSPORTPLANE":
                return 75;
			case "WATERPLANE":
                return 65;
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
