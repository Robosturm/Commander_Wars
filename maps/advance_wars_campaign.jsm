var Constructor = function()
{
    this.getDescription = function()
    {
        return qsTr("This is the Advance Wars Normal Campaign.");
    };

    this.getAuthor = function()
    {
        return "Robosturm";
    };

    this.getCampaignName = function()
    {
        return qsTr("Advance Wars Normal Campaign");
    };

    this.getBasicBuildList = function()
    {
        return ["APC", "ARTILLERY", "BATTLESHIP", "BOMBER", "CRUISER", "FIGHTER", "FLAK",
                "HEAVY_TANK", "INFANTRY", "K_HELI", "LANDER", "LIGHT_TANK", "MECH", "MISSILE",
                "RECON", "ROCKETTHROWER", "SUBMARINE", "T_HELI"]
    }

    this.getCurrentCampaignMaps = function(campaign)
    {
        // set campaign folder
        var ret = ["maps/advance_wars_1_campaign.camp/"]
        // get campaign variables
        // get campaign variables
        var variables = campaign.getVariables();
        // read all  map variables
        // orange star start
        var itsWarWon = variables.createVariable("It's War!");
        if (itsWarWon.readDataBool() === false)
        {
            ret.push("It's War.map");
        }
        var gunfighterWon = variables.createVariable("Gunfighter");
        if (itsWarWon.readDataBool() === true &&
            gunfighterWon.readDataBool() === false)
        {
            ret.push("Gunfighter.map");
        }
        var airAceWon = variables.createVariable("Air Ace");
        if (gunfighterWon.readDataBool() === true &&
            airAceWon.readDataBool() === false)
        {
            ret.push("Air Ace.map");
        }

        var maxStrikeMaxWon = variables.createVariable("Max Strike! (Max)");
        var maxStrikeAndyWon = variables.createVariable("Max Strike! (Andy)");
        if (airAceWon.readDataBool() === true &&
            maxStrikeMaxWon.readDataBool() === false &&
            maxStrikeAndyWon.readDataBool() === false)
        {
            ret.push("Max Strike! (Max).map");
            ret.push("Max Strike! (Andy).map");
        }
        // split paths between andy route and max route
        var historyLessonWon = variables.createVariable("History Lesson!");
        var olafsSeaStrikeWon = variables.createVariable("Olaf's Sea Strike!");
        var olafsSeaStrikeEnabled = variables.createVariable("olafsSeaStrikeEnabled");
        var olafsNavyWon = variables.createVariable("Olaf's Navy!");
        if (maxStrikeMaxWon.readDataBool() === true)
        {
            // max route
            var sniperWon = variables.createVariable("Sniper!");
            if (sniperWon.readDataBool() === false)
            {
                ret.push("Sniper.map");
            }
            var blizzardBattleWon = variables.createVariable("Blizzard Battle!");
            if (blizzardBattleWon.readDataBool() === false &&
                sniperWon.readDataBool() === true)
            {
                ret.push("Blizzard Battle.map");
            }
            if (historyLessonWon.readDataBool() === false &&
                blizzardBattleWon.readDataBool() === true)
            {
                ret.push("History Lesson.map");
            }
        }
        else if (maxStrikeAndyWon.readDataBool() === true)
        {
            // andy route
            var maxsFollyWon = variables.createVariable("Max's Folly?");
            if (maxsFollyWon.readDataBool() === false)
            {
                ret.push("Max's Folly.map");
            }
            if (maxsFollyWon.readDataBool() === true &&
                olafsNavyWon.readDataBool() === false)
            {
                ret.push("Olaf's Navy.map");
            }
            // select olaf's sea strike if enemy was routed
            if (olafsSeaStrikeEnabled.readDataBool() === true &&
                olafsSeaStrikeWon.readDataBool() === false)
            {
                ret.push("Olaf's Sea Strike.map");
            }
        }
        // eagle sami mission
        var samisDebutWon = variables.createVariable("Sami's Debut!");
        if ((historyLessonWon.readDataBool() === true && samisDebutWon.readDataBool() === false) ||
             olafsSeaStrikeWon.readDataBool()  === true ||
            (olafsSeaStrikeEnabled.readDataBool() === false && olafsNavyWon.readDataBool() === true))
        {
            ret.push("Sami's Debut.map");
        }
        // kanbei missions coming in
        var kanbeiArrivesWon = variables.createVariable("Kanbei Arrives!");
        if (samisDebutWon.readDataBool() === true &&
            kanbeiArrivesWon.readDataBool() === false)
        {
            ret.push("Kanbei Arrives.map");
        }
        var mightyKanbeiWon = variables.createVariable("Mighty Kanbei!");
        if (kanbeiArrivesWon.readDataBool() === true &&
            mightyKanbeiWon.readDataBool() === false)
        {
            ret.push("Mighty Kanbei.map");
        }
        var kanbeisErrorWon = variables.createVariable("Kanbei's Error?");
        if (mightyKanbeiWon.readDataBool() === true &&
            kanbeisErrorWon.readDataBool() === false)
        {
            ret.push("Kanbei's Error.map");
        }
        // sonja maps are ahead
        var sonjaCounter = variables.createVariable("sonjaCounter");
        var sonjasGoalWon = variables.createVariable("Sonja's Goal!");
        if (sonjaCounter.readDataInt32() >= 3)
        {
            var divideandConquerWon = variables.createVariable("Divide and Conquer!");
            if (kanbeisErrorWon.readDataBool() === true &&
                divideandConquerWon.readDataBool() === false)
            {
                ret.push("Divide and Conquer.map");
            }
            var samiMarchesOnWon = variables.createVariable("Sami Marches On!");
            if (divideandConquerWon.readDataBool() === true &&
                samiMarchesOnWon.readDataBool() === false)
            {
                ret.push("Sami Marches On.map");
            }
            if (samiMarchesOnWon.readDataBool() === true &&
                sonjasGoalWon.readDataBool() === false)
            {
                ret.push("Sonja's Goal.map");
            }
        }
        // next split up path ahead

        var captainDrakeMaxWon = variables.createVariable("Captain Drake! (Max)");
        var captainDrakeSamiWon = variables.createVariable("Captain Drake! (Sami)");
        var captainDrakeAndyWon = variables.createVariable("Captain Drake! (Andy)");
        if ((sonjaCounter.readDataInt32() < 3 && kanbeisErrorWon.readDataBool() === true)||
            sonjasGoalWon.readDataBool() === true &&
            captainDrakeMaxWon.readDataBool() === false &&
            captainDrakeSamiWon.readDataBool() === false &&
            captainDrakeAndyWon.readDataBool() === false)
        {
            ret.push("Captain Drake (Max).map");
            ret.push("Captain Drake (Sami).map");
            ret.push("Captain Drake (Andy).map");
        }
        var navalClashMaxWon = variables.createVariable("Naval Clash! (Max)");
        var navalClashSamiWon = variables.createVariable("Naval Clash! (Sami)");
        var navalClashAndyWon = variables.createVariable("Naval Clash! (Andy)");
        if ((captainDrakeMaxWon.readDataBool() === true ||
            captainDrakeSamiWon.readDataBool() === true ||
            captainDrakeAndyWon.readDataBool() === true) &&
            navalClashMaxWon.readDataBool() === false &&
            navalClashSamiWon.readDataBool() === false &&
            navalClashAndyWon.readDataBool() === false)
        {
            ret.push("Naval Clash (Max).map");
            ret.push("Naval Clash (Sami).map");
            ret.push("Naval Clash (Andy).map");
        }
        var wingsOfVictoryMaxWon = variables.createVariable("Wings of Victory! (Max)");
        var wingsOfVictorySamiWon = variables.createVariable("Wings of Victory! (Sami)");
        var wingsOfVictoryAndyWon = variables.createVariable("Wings of Victory! (Andy)");
        if ((navalClashMaxWon.readDataBool() === true ||
            navalClashSamiWon.readDataBool() === true ||
            navalClashAndyWon.readDataBool() === true) &&
            wingsOfVictoryMaxWon.readDataBool() === false &&
            wingsOfVictorySamiWon.readDataBool() === false &&
            wingsOfVictoryAndyWon.readDataBool() === false)
        {
            ret.push("Wings of Victory (Max).map");
            ret.push("Wings of Victory (Sami).map");
            ret.push("Wings of Victory (Andy).map");
        }
        var battleMysteryMaxWon = variables.createVariable("Battle Mystery! (Max)");
        var battleMysterySamiWon = variables.createVariable("Battle Mystery! (Sami)");
        var battleMysteryAndyWon = variables.createVariable("Battle Mystery! (Andy)");
        if ((wingsOfVictoryMaxWon.readDataBool() === true ||
            wingsOfVictorySamiWon.readDataBool() === true ||
            wingsOfVictoryAndyWon.readDataBool() === true) &&
            battleMysteryMaxWon.readDataBool() === false &&
            battleMysterySamiWon.readDataBool() === false &&
            battleMysteryAndyWon.readDataBool() === false)
        {
            ret.push("Battle Mystery (Max).map");
            ret.push("Battle Mystery (Sami).map");
            ret.push("Battle Mystery (Andy).map");
        }
        // here we meet again
        var andyTimesTwoWon = variables.createVariable("Andy Times Two!");
        if ((battleMysteryMaxWon.readDataBool() === true ||
            battleMysterySamiWon.readDataBool() === true ||
            battleMysteryAndyWon.readDataBool() === true) &&
            andyTimesTwoWon.readDataBool() === false)
        {
            ret.push("Andy Times Two.map");
        }
        var engimaWon = variables.createVariable("Enigma");
        if (andyTimesTwoWon.readDataBool() === true &&
            engimaWon.readDataBool() === false)
        {
            ret.push("Enigma.map");
        }
        var theFinalBattleWon = variables.createVariable("The Final Battle");
        if (engimaWon.readDataBool() === true &&
            theFinalBattleWon.readDataBool() === false)
        {
            ret.push("The Final Battle.map");
        }

        var samiCounter = variables.createVariable("samiCounter");


        ret.push("Naval Clash (Sami).map");
        ret.push("Wings of Victory (Max).map");
        ret.push("Wings of Victory (Sami).map");
        ret.push("Wings of Victory (Andy).map");

        ret.push("The Final Battle.map");
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
        var samiCounter = variables.createVariable("samiCounter");
        var theFinalBattleWon = variables.createVariable("The Final Battle");
        var rivalsWon = variables.createVariable("Rivals!");
        if (samiCounter.readDataInt32() < 4)
        {
            return theFinalBattleWon.readDataBool();
        }
        else
        {
            return rivalsWon.readDataBool();
        }
    };

    this.getSelectableCOs = function(campaign, map, player, coIndex)
    {
        if ((map.getMapName() === "Sniper!" ||
             map.getMapName() === "Blizzard Battle!" ||
             map.getMapName() === "Olaf's Navy!" ||
             map.getMapName() === "Olaf's Sea Strike!") &&
                coIndex === 0)
        {
            return ["CO_ANDY", "CO_MAX"];
        }
        if (player === 0 && coIndex === 0)
        {
            return ["CO_ANDY", "CO_MAX", "CO_SAMI"];
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
                coid === "CO_GRIT")
            {
                return "BM";
            }
            if (coid === "CO_KANBEI" ||
                coid === "CO_SONJA")
            {
                return "YC";
            }
            if (coid === "CO_EAGLE" ||
                coid === "CO_DRAKE")
            {
                return "GE";
            }
        }
        return "";
    };
    this.setArmyData = function(player, army)
    {
        if (army === "OS")
        {
            map.getPlayer(player).setColor(PLAYER.getDefaultColor(0));
        }
        else if (army === "BM")
        {
            map.getPlayer(player).setColor(PLAYER.getDefaultColor(1));
        }
        else if (army === "GE")
        {
            map.getPlayer(player).setColor(PLAYER.getDefaultColor(2));
        }
        else if (army === "YC")
        {
            map.getPlayer(player).setColor(PLAYER.getDefaultColor(3));
        }
        var list = campaignScript.getBasicBuildList();
        map.getPlayer(player).setBuildList(list);
    };
};

Constructor.prototype = BASECAMPAIGN;
var campaignScript = new Constructor();
