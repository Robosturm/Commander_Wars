var Constructor = function()
{
    this.getDescription = function()
    {
        return qsTr("This is the Commander Wars campaign. Explore the new story of alpha land");
    };

    this.getAuthor = function()
    {
        return "Robosturm";
    };

    this.getCampaignName = function()
    {
        return qsTr("Commander Wars");
    };

    this.mapFinished = function(campaign, map, result)
    {
        // get campaign variables
        var variables = campaign.getVariables();
        // create a campaign variable if it doesn't exist else we get the existing one
        var mapVar = variables.createVariable(map.getMapName());
        // write the map result won or loose to it.
        mapVar.writeDataBool(result);
        var lastWon = variables.createVariable("LastWonMap");
        lastWon.writeDataString(map.getMapName());
    };

    this.getSelectableCOs = function(campaign, map, player, coIndex)
    {
        var ret = [];        
        return ret;
    };

    this.getCampaignFinished = function(campaign)
    {
        return false;
    };
    this.getAllowArmyCustomization = function(campaign, map)
    {
        return false;
    };
    this.getAutoSelectPlayerColors = function(campaign, map)
    {
        return true;
    };
    this.getUsesCampaignMap = function(campaign)
    {
        return true;
    };
    this.getCampaignMapData = function(campaign, data)
    {
        data.setFolder("maps/commander_wars.camp");
        data.setMapBackground("maps/commander_wars.camp/alphaLand.png");
        data.setMapWidth(3072);
        data.setMapHeight(1444);
        var variables = campaign.getVariables();
        var lastWon = variables.createVariable("LastWonMap");
        var lastWonMapName = lastWon.readDataString();
        var wonMaps = [];
        var openMaps = [];
        var openMapNames = [];
        var newMapPositions = [];

        // --------------------------------------------------------------
        var oilInTheDesertName = "Oil in the desert";
        var oilInTheDesert = variables.createVariable(oilInTheDesertName);
        var oilInTheDesertPos = Qt.point(0.24, 0.518);
        if (oilInTheDesert.readDataBool() === false)
        {
            openMaps.push(oilInTheDesertPos);
            openMapNames.push("oilInTheDesert.map");
            var newOilInTheDesert = variables.createVariable("new" + oilInTheDesertName);
            if (newOilInTheDesert.readDataBool() === false)
            {
                newMapPositions.push(newMapPositions.length);
                newOilInTheDesert.writeDataBool(true);
            }
        }
        else
        {
            if (lastWonMapName === oilInTheDesertName)
            {
                data.setNewlyWonMap(wonMaps.length);
            }
            wonMaps.push(oilInTheDesertPos);
        }
        // --------------------------------------------------------------

        // test code
        // var wonMaps = [Qt.point(0.5, 0.5), Qt.point(0.25, 0.25), Qt.point(0.75, 0.75)];
        // var openMaps = [Qt.point(0.25, 0.5), Qt.point(0.5, 0.25), Qt.point(0.25, 0.75)];
        // var newMapPositions = [0, 1, 2];

        data.setOpenMapPositions(openMaps);
        data.setNewMapPosition(newMapPositions);
        data.setWonMapPositions(wonMaps);
        data.setMapFilenames(openMapNames);
    };
};

Constructor.prototype = BASECAMPAIGN;
var campaignScript = new Constructor();
