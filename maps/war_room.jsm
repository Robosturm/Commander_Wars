var Constructor = function()
{
    this.getDescription = function()
    {
        return qsTr("This is the Commander Wars war room. Here you can fight predefined battles against the ai and challenge yourself.");
    };

    this.getAuthor = function()
    {
        return "Robosturm";
    };

    this.getCampaignName = function()
    {
        return qsTr("Warroom");
    };

    this.getCurrentCampaignMaps = function(campaign)
    {
        // set campaign folder
        var ret = ["maps/war_room.camp/"]
        var maps = globals.getFiles("maps/war_room.camp/", "*.map");
        ret = ret.concat(maps);
        return ret;
    };
	
	this.mapFinished = function(campaign, map, result)
    {
        // do nothing here
    };

    this.getSelectableCOs = function(campaign, map, player, coIndex)
    {
        return [];
    };

    this.getCampaignFinished = function(campaign)
    {
        // you can't win this campaign :P
        return false;
    };
};

Constructor.prototype = BASECAMPAIGN;
var campaignScript = new Constructor();
