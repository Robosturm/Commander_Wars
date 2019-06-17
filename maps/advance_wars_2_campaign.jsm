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
            ret.push("Liberation.map");
        }
        var factoryBluesWon = variables.createVariable("Factory Blues");
        var theHuntsEndWon = variables.createVariable("The Hunt's End");
        var greatSeaBattleWon = variables.createVariable("Great Sea Battle");
        if (LiberationWon.readDataBool() === true)
        {
            // blue moon
            var toyboxWon = variables.createVariable("Toy Box");
            var reclamationWon = variables.createVariable("Reclamation");
            var tanksWon = variables.createVariable("Tanks!!!");
            var natureWalkWon = variables.createVariable("Nature Walk");
            var twoWeekTestWon = variables.createVariable("Two Week Test");
            var tminus15Won = variables.createVariable("T Minus 15");
            var neotanksWon = variables.createVariable("Neotanks?!");
            if (factoryBluesWon.readDataBool() === false)
            {
                var bmLabFound = variables.createVariable("bmLabFound");
                var blueMoonCount = 0;
                if (toyboxWon.readDataBool() === false)
                {
                    ret.push("Toy Box.map");
                }
                else
                {
                    blueMoonCount += 1;
                }
                if (reclamationWon.readDataBool() === false)
                {
                    ret.push("Reclamation.map");
                }
                else
                {
                    blueMoonCount += 1;
                }
                if (tanksWon.readDataBool() === false)
                {
                    ret.push("Tanks!!!.map");
                }
                else
                {
                    blueMoonCount += 1;
                }
                if (blueMoonCount >= 2)
                {
                    // enable the last three missions
                    if (natureWalkWon.readDataBool() === false)
                    {
                        ret.push("Nature Walk.map");
                    }
                    else
                    {
                        blueMoonCount += 1;
                    }
                    if (twoWeekTestWon.readDataBool() === false)
                    {
                        ret.push("Two Week Test.map");
                    }
                    else
                    {
                        blueMoonCount += 1;
                    }
                    if (tminus15Won.readDataBool() === false)
                    {
                        ret.push("T Minus 15.map");
                    }
                    else
                    {
                        blueMoonCount += 1;
                    }
                }
                if (bmLabFound.readDataBool() === true &&
                        neotanksWon.readDataBool() === false)
                {
                    ret.push("Neotanks.map");
                }
                if (blueMoonCount >= 4)
                {
                    // enable factory mission
                    ret.push("Factory Blues.map");
                }
            }
            // yellow comet
            var ycLabFound = variables.createVariable("ycLabFound");
            var senseisReturnWon = variables.createVariable("Sensei's Return");
            var siloScrambleWon = variables.createVariable("Silo Scramble");
            var showStopperWon = variables.createVariable("Show Stopper");
            var dutyAndHonorWon = variables.createVariable("Duty and Honor");
            var foulPlayWon = variables.createVariable("Foul Play");
            var aMirrorDarklyWon = variables.createVariable("A Mirror Darkly");
            var seaofHopeWon = variables.createVariable("Sea of Hope");
            var yellowCometCount = 0;
            if (theHuntsEndWon.readDataBool() === false)
            {
                if (senseisReturnWon.readDataBool() === false)
                {
                    ret.push("Sensei's Return.map");
                }
                else
                {
                    yellowCometCount += 1;
                }
                if (siloScrambleWon.readDataBool() === false)
                {
                    ret.push("Silo Scramble.map");
                }
                else
                {
                    yellowCometCount += 1;
                }
                if (showStopperWon.readDataBool() === false)
                {
                    ret.push("Show Stopper.map");
                }
                else
                {
                    yellowCometCount += 1;
                }
                if (yellowCometCount >= 2)
                {
                    if (dutyAndHonorWon.readDataBool() === false)
                    {
                        ret.push("Duty and Honor.map");
                    }
                    else
                    {
                        yellowCometCount += 1;
                    }
                    if (foulPlayWon.readDataBool() === false)
                    {
                        ret.push("Foul Play.map");
                    }
                    else
                    {
                        yellowCometCount += 1;
                    }
                    if (aMirrorDarklyWon.readDataBool() === false)
                    {
                        ret.push("A Mirror Darkly.map");
                    }
                    else
                    {
                        yellowCometCount += 1;
                    }
                }
                if (ycLabFound.readDataBool() === true &&
                    seaofHopeWon.readDataBool() === false)
                {
                    ret.push("Sea of Hope.map");
                }
                if (yellowCometCount >= 4)
                {
                    ret.push("The Hunt's End.map");
                }
            }
        }
        // ge
        if (factoryBluesWon.readDataBool() === true &&
            theHuntsEndWon.readDataBool() === true)
        {
            if (greatSeaBattleWon.readDataBool() === false)
            {
                var geLabFound = variables.createVariable("geLabFound");
                var navyVSAirWon = variables.createVariable("Navy VS Air");
                var seaFortressWon = variables.createVariable("Sea Fortress");

                //            var showStopperWon = variables.createVariable("Show Stopper");
                //            var dutyAndHonorWon = variables.createVariable("Duty and Honor");
                //            var foulPlayWon = variables.createVariable("Foul Play");
                //            var aMirrorDarklyWon = variables.createVariable("A Mirror Darkly");
                //            var seaofHopeWon = variables.createVariable("Sea of Hope");
                var greenEarthCount = 0;
                if (seaFortressWon.readDataBool() === false)
                {
                    ret.push("Sea Fortress.map");
                }
                else
                {
                    greenEarthCount += 1;
                }

                if (greenEarthCount >= 2)
                {
                    if (navyVSAirWon.readDataBool() === false)
                    {
                        ret.push("Navy VS Air.map");
                    }
                    else
                    {
                        greenEarthCount += 1;
                    }
                }
                if (greenEarthCount >= 4)
                {
                    ret.push("Great Sea Battle.map");
                }
            }
        }
        ret.push("Great Sea Battle.map");
        ret.push("Hot Pursuit.map");
        ret.push("Last Mission.map");
        ret.push("Factory Blues.map");
        ret.push("The Hunt's End.map");
        // bh
        if (greatSeaBattleWon.readDataBool() === true)
        {
            ret.push("Hot Pursuit.map");
        }
        var hotPursuitWon = variables.createVariable("Hot Pursuit");
        if (hotPursuitWon.readDataBool() === true)
        {
            ret.push("Last Mission.map");
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
        var variables = campaign.getVariables();
        var lastMissionWon = variables.createVariable("Last Mission");
        return lastMissionWon.readDataBool();
    };

    this.getSelectableCOs = function(campaign, map, player, coIndex)
    {
        var variables = campaign.getVariables();
        // bm labor
        if (map.getMapName() === "Neotanks?!" && player === 0 && coIndex === 0)
        {
            return ["CO_COLIN", "CO_OLAF", "CO_GRIT"];
        }
        // yc labor
        if (map.getMapName() === "Sea of Hope" && player === 0 && coIndex === 0)
        {
            return ["CO_KANBEI", "CO_SENSEI", "CO_SONJA"];
        }
        // ge labor
        if (map.getMapName() === "Danger X 9" && player === 0 && coIndex === 0)
        {
            return ["CO_EAGLE", "CO_DRAKE", "CO_JESS"];
        }

        // os factory
        if (map.getMapName() === "Liberation" && coIndex === 0)
        {
            return ["CO_ANDY", "CO_MAX", "CO_SAMI"];
        }
        // bm factory
        if (map.getMapName() === "Factory Blues")
        {
            var theHuntsEndWon = variables.createVariable("The Hunt's End");
            if (coIndex === 0 && player === 0)
            {
                return ["CO_COLIN", "CO_OLAF", "CO_GRIT"];
            }
            if ( coIndex === 0 && player === 1)
            {
                if (theHuntsEndWon.readDataBool() === true)
                {
                    return ["CO_ANDY", "CO_MAX", "CO_SAMI", "CO_KANBEI", "CO_SENSEI", "CO_SONJA"];
                }
                else
                {
                    return ["CO_ANDY", "CO_MAX", "CO_SAMI"];
                }
            }
        }
        // yc factory
        if (map.getMapName() === "The Hunt's End")
        {
            var factoryBluesWon = variables.createVariable("Factory Blues");
            if (coIndex === 0 && player === 0)
            {
                return ["CO_KANBEI", "CO_SENSEI", "CO_SONJA"];
            }
            if ( coIndex === 0 && player === 1)
            {
                if (factoryBluesWon.readDataBool() === true)
                {
                    return ["CO_ANDY", "CO_MAX", "CO_SAMI", "CO_COLIN", "CO_OLAF", "CO_GRIT"];
                }
                else
                {
                    return ["CO_ANDY", "CO_MAX", "CO_SAMI"];
                }
            }
        }
        // ge factory
        if (map.getMapName() === "Great Sea Battle")
        {
            if (coIndex === 0 && player === 2)
            {
                return ["CO_KANBEI", "CO_SENSEI", "CO_SONJA"];
            }
            if ( coIndex === 0 && player === 1)
            {
                return ["CO_ANDY", "CO_MAX", "CO_SAMI"];
            }
            if ( coIndex === 0 && player === 0)
            {
                return ["CO_EAGLE", "CO_JESS", "CO_DRAKE"];
            }
        }
        // bh missions
        if ((map.getMapName() === "Hot Pursuit" || map.getMapName() === "Last Mission") &&
            coIndex === 0 && player <= 2)
        {
            // make each army unique selectable
            var ret = [];
            var co0 = null;
            if (player !== 0)
            {
                co0 = map.getPlayer(0).getCO(0);
            }
            var co1 = null;
            if (player !== 1)
            {
                co1 = map.getPlayer(1).getCO(0);
            }
            var co2 = null;
            if (player !== 2)
            {
                co2 = map.getPlayer(2).getCO(0);
            }
            var armies = [];
            armies.push(campaignScript.getArmy(co0));
            armies.push(campaignScript.getArmy(co1));
            armies.push(campaignScript.getArmy(co2));
            if (armies.indexOf("OS") < 0)
            {
                ret.push("CO_ANDY");
                ret.push("CO_MAX");
                ret.push("CO_SAMI");
            }
            if (armies.indexOf("BM") < 0)
            {
                ret.push("CO_OLAF");
                ret.push("CO_COLIN");
                ret.push("CO_GRIT");
            }
            if (armies.indexOf("GE") < 0)
            {
                ret.push("CO_EAGLE");
                ret.push("CO_JESS");
                ret.push("CO_DRAKE");
            }
            if (armies.indexOf("YC") < 0)
            {
                ret.push("CO_KANBEI");
                ret.push("CO_SONJA");
                ret.push("CO_SENSEI");
            }
            return ret;
        }
        // make no co selectable
        return [""];
    };
    this.getArmy = function(co)
    {
        if (co !== null)
        {
            var coid = co.getCoID();
            if (coid === "CO_ANDY" ||
                coid === "CO_MAX" ||
                coid === "CO_SAMI" )
            {
                return "OS";
            }
            if (coid === "CO_OLAF" ||
                coid === "CO_GRIT" ||
                coid === "CO_COLIN" )
            {
                return "BM";
            }
            if (coid === "CO_KANBEI" ||
                coid === "CO_SONJA" ||
                coid === "CO_SENSEI" )
            {
                return "YC";
            }
            if (coid === "CO_EAGLE" ||
                coid === "CO_DRAKE" ||
                coid === "CO_JESS" )
            {
                return "GE";
            }
        }
        return "";
    };
};

Constructor.prototype = BASECAMPAIGN;
var campaignScript = new Constructor();
