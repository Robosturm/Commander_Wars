var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Flare");
    };
};

Constructor.prototype = WEAPON;
var WEAPON_FLARE = new Constructor();
