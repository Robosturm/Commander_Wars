Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Designator");
    };
    this.getEnviromentDamage = function(enviromentId)
    {
        return WEAPON_CANNON.getEnviromentDamage(enviromentId);
    };

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_CANNON.damageTable, "WEAPON_CANNON");
    };
};

Constructor.prototype = WEAPON;
var WEAPON_DESIGNATOR = new Constructor();
