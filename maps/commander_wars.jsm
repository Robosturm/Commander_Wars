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
        // sprite flag offset 16 64
        // 3072 1444
        var variables = campaign.getVariables();
        var lastWon = variables.createVariable("LastWonMap");
        var lastWonMapName = lastWon.readDataString();
        var wonMaps = [];
        var openMaps = [];
        var openMapNames = [];
        var newMapPositions = [];
        var mapCount = 0;
        var mapWinCount = 0;

        // --------------------------------------------------------------
        // Oil in the deset
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
                newMapPositions.push(mapCount);
                newOilInTheDesert.writeDataBool(true);
            }
            ++mapCount;
        }
        else
        {
            if (lastWonMapName === oilInTheDesertName)
            {
                data.setNewlyWonMap(mapWinCount);
            }
            wonMaps.push(oilInTheDesertPos);
            ++mapWinCount;
        }
        // --------------------------------------------------------------
        // --------------------------------------------------------------
        // Take and Hold
        // --------------------------------------------------------------
        //if (oilInTheDesert.readDataBool() === true)
        //{
            var takeAndHoldName = "Take and Hold";
            var takeAndHold = variables.createVariable(takeAndHoldName);
            var takeAndHoldPos = Qt.point(0.26, 0.525);
            if (takeAndHold.readDataBool() === false)
            {
                openMaps.push(takeAndHoldPos);
                openMapNames.push("takeAndHold.map");
                var newTakeAndHold = variables.createVariable("new" + takeAndHoldName);
                if (newTakeAndHold.readDataBool() === false)
                {
                    newMapPositions.push(mapCount);
                    newTakeAndHold.writeDataBool(true);
                }
                ++mapCount;
            }
            else
            {
                if (lastWonMapName === takeAndHoldName)
                {
                    data.setNewlyWonMap(mapWinCount);
                }
                wonMaps.push(oilInTheDesertPos);
                ++mapWinCount;
            }
        //}

        // --------------------------------------------------------------
        // Day at the beach
        // --------------------------------------------------------------
        // 2080 805
        var dayAtTheBeachName = "Day at the Beach";
        var dayAtTheBeach = variables.createVariable(dayAtTheBeachName);
        var dayAtTheBeachPos = Qt.point(0.671875, 0.513158);
        if (dayAtTheBeach.readDataBool() === false)
        {
            openMaps.push(dayAtTheBeachPos);
            openMapNames.push("dayAtTheBeach.map");
            var newDayAtTheBeach = variables.createVariable("new" + dayAtTheBeachName);
            if (newDayAtTheBeach.readDataBool() === false)
            {
                newMapPositions.push(mapCount);
                newDayAtTheBeach.writeDataBool(true);
            }
            ++mapCount;
        }
        else
        {
            if (lastWonMapName === dayAtTheBeachName)
            {
                data.setNewlyWonMap(mapWinCount);
            }
            wonMaps.push(dayAtTheBeachPos);
            ++mapWinCount;
        }
        // --------------------------------------------------------------

        // --------------------------------------------------------------
        // Urprising at the docks
        // --------------------------------------------------------------
        var uprisingAtTheDocksName = "Uprising at the Docks";
        var uprisingAtTheDocks = variables.createVariable(uprisingAtTheDocksName);
        if (dayAtTheBeach.readDataBool() === true)
        {
            var uprisingAtTheDocksPos = Qt.point(0.712565, 0.54778);
            if (uprisingAtTheDocks.readDataBool() === false)
            {
                openMaps.push(uprisingAtTheDocksPos);
                openMapNames.push("uprisingAtTheDocks.map");
                var newUprisingAtTheDocks = variables.createVariable("new" + uprisingAtTheDocksName);
                if (newUprisingAtTheDocks.readDataBool() === false)
                {
                    newMapPositions.push(mapCount);
                    newUprisingAtTheDocks.writeDataBool(true);
                }
                ++mapCount;
            }
            else
            {
                if (lastWonMapName === uprisingAtTheDocksName)
                {
                    data.setNewlyWonMap(mapWinCount);
                }
                wonMaps.push(uprisingAtTheDocksPos);
                ++mapWinCount;
            }
        }
        // --------------------------------------------------------------

        // --------------------------------------------------------------
        // Mecha in the Middle
        // --------------------------------------------------------------
        var mechaInTheMiddleName = "Mecha in the Middle";
        var mechaInTheMiddle = variables.createVariable(mechaInTheMiddleName);
        if (uprisingAtTheDocks.readDataBool() === true)
        {
            var mechaInTheMiddlePos = Qt.point(0.7630208, 0.5166205);
            if (mechaInTheMiddle.readDataBool() === false)
            {
                openMaps.push(mechaInTheMiddlePos);
                openMapNames.push("mechaInTheMiddle.map");
                var newMechaInTheMiddle = variables.createVariable("new" + mechaInTheMiddleName);
                if (newMechaInTheMiddle.readDataBool() === false)
                {
                    newMapPositions.push(mapCount);
                    newMechaInTheMiddle.writeDataBool(true);
                }
                ++mapCount;
            }
            else
            {
                if (lastWonMapName === mechaInTheMiddleName)
                {
                    data.setNewlyWonMap(mapWinCount);
                }
                wonMaps.push(mechaInTheMiddlePos);
                ++mapWinCount;
            }
        }
        // --------------------------------------------------------------

        // --------------------------------------------------------------
        // Communication Devastation
        // --------------------------------------------------------------
        // 2175 602
        var communicationDevastationName = "Mecha in the Middle";
        var communicationDevastation = variables.createVariable(communicationDevastationName);
        if (mechaInTheMiddle.readDataBool() === true)
        {
            var communicationDevastationPos = Qt.point(0.70345, 0.3732687);
            if (communicationDevastation.readDataBool() === false)
            {
                openMaps.push(communicationDevastationPos);
                openMapNames.push("communicationDevastation.map");
                var newCommunicationDevastation = variables.createVariable("new" + communicationDevastationName);
                if (newCommunicationDevastation.readDataBool() === false)
                {
                    newMapPositions.push(mapCount);
                    newCommunicationDevastation.writeDataBool(true);
                }
                ++mapCount;
            }
            else
            {
                if (lastWonMapName === communicationDevastationName)
                {
                    data.setNewlyWonMap(mapWinCount);
                }
                wonMaps.push(communicationDevastationPos);
                ++mapWinCount;
            }
        }

        // --------------------------------------------------------------        
        // --------------------------------------------------------------
        // Communication Devastation
        // --------------------------------------------------------------
        // 2660 752
        var hurricaneInTheHarborName = "hurricaneInTheHarbor";
        var hurricaneInTheHarbor = variables.createVariable(hurricaneInTheHarborName);
        //if (communicationDevastation.readDataBool() === true)
        //{
            var hurricaneInTheHarborPos = Qt.point(0.865885, 0.479224);
            if (hurricaneInTheHarbor.readDataBool() === false)
            {
                openMaps.push(hurricaneInTheHarborPos);
                openMapNames.push("hurricaneInTheHarbor.map");
                var newHurricaneInTheHarbor = variables.createVariable("new" + hurricaneInTheHarborName);
                if (newHurricaneInTheHarbor.readDataBool() === false)
                {
                    newMapPositions.push(mapCount);
                    newHurricaneInTheHarbor.writeDataBool(true);
                }
                ++mapCount;
            }
            else
            {
                if (lastWonMapName === hurricaneInTheHarborName)
                {
                    data.setNewlyWonMap(mapWinCount);
                }
                wonMaps.push(hurricaneInTheHarborPos);
                ++mapWinCount;
            }
        //}
        // --------------------------------------------------------------
        data.setOpenMapPositions(openMaps);
        data.setNewMapPosition(newMapPositions);
        data.setWonMapPositions(wonMaps);
        data.setMapFilenames(openMapNames);
    };
};

Constructor.prototype = BASECAMPAIGN;
var campaignScript = new Constructor();
