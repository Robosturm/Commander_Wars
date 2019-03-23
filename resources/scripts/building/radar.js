var Constructor = function()
{
    // called for loading the main sprite
    this.loadSprites = function(building)
    {
        if (building.getOwnerID() >= 0)
        {
            // none neutral player
            building.loadSprite("radar", false);
            building.loadSprite("radar+mask", true);
        }
        else
        {
            // neutral player
            building.loadSprite("radar+neutral", false);
        }
    };
    this.getBaseIncome = function()
    {
        return 0;
    };

    this.getVisionBonus = function()
    {
        return 1;
    };
}

Constructor.prototype = BUILDING;
var RADAR = new Constructor();
