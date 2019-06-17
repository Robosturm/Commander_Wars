var Constructor = function()
{
    this.getDescription = function()
    {
        return qsTr("This is the Commander Wars Tutorial which will teach you basics of how to play Commander Wars.");
    };

    this.getAuthor = function()
    {
        return "Robosturm";
    };

    this.getCampaignName = function()
    {
        return qsTr("Tutorial");
    };

    this.getCurrentCampaignMaps = function(campaign)
    {
        // set campaign folder
        var ret = ["maps/tutorial.camp/"]
        // get campaign variables
        var variables = campaign.getVariables();
        // read all  map variables
        var tutorial1Won = variables.createVariable("Tutorial 1");
        var tutorial2Won = variables.createVariable("Tutorial 2");
        var tutorial3Won = variables.createVariable("Tutorial 3");
        // check for each tutorial map if we have won it if not enable the map for play
        if (tutorial1Won.readDataBool() === false)
        {
            ret.push("tutorial1.map");
        }
        if (tutorial2Won.readDataBool() === false)
        {
            ret.push("tutorial2.map");
        }
        if (tutorial3Won.readDataBool() === false)
        {
            // ret.push("tutorial3.map");
        }
        return ret;
    };
	
	this.mapFinished = function(campaign, map, result)
    {
        // get campaign variables
		var variables = campaign.getVariables();
        // create a campaign variable if it doesn't exist else we get the existing one
        var mapVar = variables.createVariable(map.getMapName());
        // write the map result won or loose to it.
        mapVar.writeDataBool(result);
    };

    this.getCampaignFinished = function(campaign)
    {
        // get campaign variables
        var variables = campaign.getVariables();
        // read all  map variables
        var tutorial1Won = variables.createVariable("Tutorial 1");
        var tutorial2Won = variables.createVariable("Tutorial 2");
        var tutorial3Won = variables.createVariable("Tutorial 3");
        if (tutorial1Won.readDataBool() === true &&
            tutorial2Won.readDataBool() === true &&
            tutorial3Won.readDataBool() === true)
        {
            return true;
        }
        // not all maps are won so you didn't won the campaign
        return false;
    };
};

Constructor.prototype = BASECAMPAIGN;
var campaignScript = new Constructor();
