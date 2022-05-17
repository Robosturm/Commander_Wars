var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Unknown");
    };
    this.getGroupName = function()
    {
        return qsTr("Unknown");
    };
    this.getDescription = function()
    {
        return qsTr("Unknown");
    };
};

Constructor.prototype = MOVEMENTPLANNERADDIN;
var DUMMY = new Constructor();
