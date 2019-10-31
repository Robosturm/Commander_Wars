var Constructor = function()
{
    // called for loading the main sprite
    this.loadSprites = function(building)
    {
        // neutral player
        building.loadSprite("silo+rocket", false);
    };
    this.getBaseIncome = function()
    {
        return 0;
    };
    this.getDefense = function()
    {
        return 2;
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_silo";
    };

    this.getName = function()
    {
        return qsTr("Silo");
    };

    this.getDescription = function()
    {
        return "<r>" + qsTr("Missile can be launched once, by Infantry unit. Damage across ") + "</r>" +
               "<div c='#00ff00'>" + qsTr("13 square ") + "</div>" +
               "<r>" + qsTr("area.") + "</r>";
    };
}

Constructor.prototype = BUILDING;
var SILO_ROCKET = new Constructor();
