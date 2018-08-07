var Constructor = function()
{
    // called for loading the main sprite
    this.loadSprites = function(building)
    {
        if (building.getOwnerID() >= 0)
        {
            // none neutral player
            var player = building.getOwner();
            // get army name
            var armyName = player.getArmy();
            // bh and bg have the same sprites
            if (armyName === "BG")
            {
                armyName = "BH"
            }
            building.loadSprite("hq+" + armyName, false);
            building.loadSprite("hq+" + armyName + "+mask", true);
        }
        else
        {
            // neutral player
            building.loadSprite("hq+neutral", false);
        }
    };
}

Constructor.prototype = BUILDING;
var HQ = new Constructor();
