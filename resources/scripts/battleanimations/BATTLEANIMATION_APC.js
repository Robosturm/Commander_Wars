var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };
    this.armyData = [["os", "os"],
                     ["bm", "bm"],
                     ["ge", "ge"],
                     ["yc", "yc"],
                     ["bh", "bh"],
                     ["bg", "bh"],
                     ["ma", "ma"],
                     ["ac", "ac"],
                     ["dm", "dm"],];
    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_APC.armyData);
        sprite.loadSprite("apc+" + armyName,  false,
                          BATTLEANIMATION_APC.getMaxUnitCount(), Qt.point(10, 10));
        sprite.loadSpriteV2("apc+" + armyName + "+mask",  GameEnums.Recoloring_Table,
                          BATTLEANIMATION_APC.getMaxUnitCount(), Qt.point(10, 10));
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_APC = new Constructor();
