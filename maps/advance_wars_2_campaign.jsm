var Constructor = function()
{
    this.getDescription = function()
    {
        return qsTr("This is the Advance Wars 2 Normal Campaign.");
    };

    this.getAuthor = function()
    {
        return "Robosturm";
    };

    this.getCampaignName = function()
    {
        return qsTr("Advance Wars 2 Normal Campaign");
    };

    this.getBasicBuildList = function()
    {
        return ["APC", "ARTILLERY", "BATTLESHIP", "BOMBER", "CRUISER", "FIGHTER", "FLAK",
                "HEAVY_TANK", "INFANTRY", "K_HELI", "LANDER", "LIGHT_TANK", "MECH",
                "MISSILE", "RECON", "ROCKETTHROWER", "SUBMARINE", "T_HELI"]
    }
    this.getOSBuildList = function()
    {
        var ret = campaignScript.getBasicBuildList();
         var campaignVariables = map.getCampaign().getVariables();
        var osNeotanks = campaignVariables.createVariable("osNeotanks");
        if (osNeotanks.readDataBool() === true)
        {
            ret.push("NEOTANK");
        }
        return ret;
    }
    this.getBMBuildList = function()
    {
        var ret = campaignScript.getBasicBuildList();
        var campaignVariables = map.getCampaign().getVariables();
        var bmNeotanks = campaignVariables.createVariable("bmNeotanks");
        if (bmNeotanks.readDataBool() === true)
        {
            ret.push("NEOTANK");
        }
        return ret;
    }
    this.getYCBuildList = function()
    {
        var ret = campaignScript.getBasicBuildList();
         var campaignVariables = map.getCampaign().getVariables();
        var ycNeotanks = campaignVariables.createVariable("ycNeotanks");
        if (ycNeotanks.readDataBool() === true)
        {
            ret.push("NEOTANK");
        }
        return ret;
    }
    this.getGEBuildList = function()
    {
        var ret = campaignScript.getBasicBuildList();
         var campaignVariables = map.getCampaign().getVariables();
        var geNeotanks = campaignVariables.createVariable("geNeotanks");
        if (geNeotanks.readDataBool() === true)
        {
            ret.push("NEOTANK");
        }
        return ret;
    }
    this.getBHBuildList = function()
    {
        var ret = campaignScript.getBasicBuildList();
        ret.push("NEOTANK");
        return ret;
    }


    this.getCurrentCampaignMaps = function(campaign)
    {
        // set campaign folder
        var ret = ["maps/advance_wars_2_campaign.camp/"]
        // get campaign variables
        // get campaign variables
        var variables = campaign.getVariables();
        // read all  map variables

        // orange star maps
        var CleanupWon = variables.createVariable("Cleanup");
        var Border_SkirmishWon = variables.createVariable("Border Skirmish");
        var Orange_DawnWon = variables.createVariable("Orange Dawn");
        var Flak_AttackWon = variables.createVariable("Flak Attack");
        var Lash_OutWon = variables.createVariable("Lash Out");
        var Andy_s_TimeWon = variables.createVariable("Andy's Time");
        var osLabFound = variables.createVariable("osLabFound");
        var Test_of_TimeWon = variables.createVariable("Test of Time");
        var LiberationWon = variables.createVariable("Liberation");
        if (CleanupWon.readDataBool() === false)
        {
            ret.push("Cleanup.map");
        }
        if (CleanupWon.readDataBool() === true &&
            Border_SkirmishWon.readDataBool() === false)
        {
            ret.push("Border Skirmish.map");
        }
        if (Border_SkirmishWon.readDataBool() === true &&
            Orange_DawnWon.readDataBool() === false)
        {
            ret.push("Orange Dawn.map");
        }
        if (Orange_DawnWon.readDataBool() === true &&
            Flak_AttackWon.readDataBool() === false)
        {
            ret.push("Flak Attack.map");
        }
        if (Orange_DawnWon.readDataBool() === true &&
            Lash_OutWon.readDataBool() === false)
        {
            ret.push("Lash Out.map");
        }

        if (Orange_DawnWon.readDataBool() === true &&
            Andy_s_TimeWon.readDataBool() === false)
        {
            ret.push("Andy's Time.map");
        }
        // check for os lab enable
        if (osLabFound.readDataBool() === true &&
            Test_of_TimeWon.readDataBool() === false &&
            LiberationWon.readDataBool() === false)
        {
            ret.push("Test of Time.map");
        }
        // check for os factory map
        if (Flak_AttackWon.readDataBool() === true &&
            Lash_OutWon.readDataBool() === true &&
            Andy_s_TimeWon.readDataBool() === true &&
            LiberationWon.readDataBool() === false)
        {

        }
        ret.push("Liberation.map");
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

        // not all maps are won so you didn't won the campaign
        return false;
    };
};

Constructor.prototype = BASECAMPAIGN;
var campaignScript = new Constructor();
