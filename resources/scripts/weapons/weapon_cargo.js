var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Cargo");
    };
    this.getEnviromentDamage = function(enviromentId)
    {
        return -1;
    };
    this.damageTable = [];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_CARGO.damageTable, "WEAPON_CARGO");
    };
};

Constructor.prototype = WEAPON;
var WEAPON_CARGO = new Constructor();
