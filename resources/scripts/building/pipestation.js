var Constructor = function()
{
    // called for loading the main sprite
    this.loadSprites = function(building, neutral)
    {
        if (building.getOwnerID() >= 0 && !neutral)
        {
            // none neutral player
            building.loadSprite("pipestation", false);
            building.loadSprite("pipestation+mask", true);
        }
        else
        {
            // neutral player
            building.loadSprite("pipestation+neutral", false);
        }
    };
    this.getActions = function()
    {
        // returns a string id list of the actions this building can perform
        return "ACTION_BUILD_UNITS";
    };

    this.getConstructionList = function(building)
    {
        return ["PIPERUNNER"];
    };
    this.getBaseIncome = function()
    {
        return 2000;
    };
    this.getName = function()
    {
        return qsTr("Pipestation");
    };
    this.getDescription = function()
    {
        return "<r>" + qsTr("Once captured can be used for ") + "</r>" +
               "<div c='#00ff00'>" + qsTr("production and resupplying ") + "</div>" +
               "<r>" + qsTr("of ") + "</r>" +
               "<div c='#00ff00'>" + qsTr("pipe ") + "</div>" +
               "<r>" + qsTr("units. Produces also more funds.") + "</r>";
    };

    this.getVisionHide = function(building)
    {
        return true;
    };
}

Constructor.prototype = BUILDING;
var PIPESTATION = new Constructor();
