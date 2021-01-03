var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Flare");
    };
    this.getEnviromentDamage = function(enviromentId)
    {
        return -1;
    };
    this.damageTable = [];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_FLARE.damageTable, "WEAPON_FLARE");
    };
};

Constructor.prototype = WEAPON;
var WEAPON_FLARE = new Constructor();
