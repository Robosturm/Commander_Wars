var Constructor = function() { // campaign
    this.getCampaignName = function() { //campaignName
        return qsTr("Advance Wars Days of Ruin");
    }; //campaignName
    this.getAuthor = function() { // campaignAuthor
        return qsTr("Robosturm");
    }; // campaignAuthor
    this.getDescription = function() { // campaignDescription
        return qsTr("This is the Advance Wars Days of Ruin campaign minus the final map sunrise");
    }; // campaignDescription
    this.getCurrentCampaignMaps = function(campaign) { // campaignMaps
        var variables = campaign.getVariables();
        var ret = ["maps/advance_wars_day_of_ruin_campaign.camp/"]; // campaignMapsFolder
        var map0Won = variables.createVariable("A Single Life"); // campaignMapNames
        var map1Won = variables.createVariable("Freehaven"); // campaignMapNames
        var map2Won = variables.createVariable("Moving On"); // campaignMapNames
        var map3Won = variables.createVariable("New Allies"); // campaignMapNames
        var map4Won = variables.createVariable("Fear Experiment"); // campaignMapNames
        var map5Won = variables.createVariable("A Kind of Home"); // campaignMapNames
        var map6Won = variables.createVariable("A new Threat"); // campaignMapNames
        var map7Won = variables.createVariable("The Beast"); // campaignMapNames
        var map8Won = variables.createVariable("Almost Home"); // campaignMapNames
        var map9Won = variables.createVariable("A Storm Brews"); // campaignMapNames
        var map10Won = variables.createVariable("History of Hate"); // campaignMapNames
        var map11Won = variables.createVariable("Greyfield Strikes"); // campaignMapNames
        var map12Won = variables.createVariable("A Hero's Farewell"); // campaignMapNames
        var map13Won = variables.createVariable("Icy Retreat"); // campaignMapNames
        var map14Won = variables.createVariable("Hope Rising"); // campaignMapNames
        var map15Won = variables.createVariable("The Creeper"); // campaignMapNames
        var map16Won = variables.createVariable("Panic in the Ranks"); // campaignMapNames
        var map17Won = variables.createVariable("Salvation"); // campaignMapNames
        var map18Won = variables.createVariable("Waylon Flies Again"); // campaignMapNames
        var map19Won = variables.createVariable("Lin's Gambit"); // campaignMapNames
        var map20Won = variables.createVariable("The Great Owl"); // campaignMapNames
        var map21Won = variables.createVariable("Sacrificial Lamb"); // campaignMapNames
        var map22Won = variables.createVariable("Crash Landing"); // campaignMapNames
        var map23Won = variables.createVariable("Final Experiment"); // campaignMapNames
        var map24Won = variables.createVariable("Days of Ruin"); // campaignMapNames
        var map0EnableCount = 0; // campaignMapEnabled
        if (map24Won.readDataBool() === true) { map0EnableCount++;} 
        // campaignMapEnabled
        var map0DisableCount = 0; // campaignMapDisabled
        // campaignMapDisabled
        if (map0DisableCount < 1 && map0EnableCount >= 1) {ret.push("A Single Life.map");} // campaignMapAdd
        var map1EnableCount = 0; // campaignMapEnabled
        if (map0Won.readDataBool() === true) { map1EnableCount++;} 
        // campaignMapEnabled
        var map1DisableCount = 0; // campaignMapDisabled
        // campaignMapDisabled
        if (map1DisableCount < 1 && map1EnableCount >= 1) {ret.push("Freehaven.map");} // campaignMapAdd
        var map2EnableCount = 0; // campaignMapEnabled
        if (map1Won.readDataBool() === true) { map2EnableCount++;} 
        // campaignMapEnabled
        var map2DisableCount = 0; // campaignMapDisabled
        // campaignMapDisabled
        if (map2DisableCount < 1 && map2EnableCount >= 1) {ret.push("Moving On.map");} // campaignMapAdd
        var map3EnableCount = 0; // campaignMapEnabled
        if (map2Won.readDataBool() === true) { map3EnableCount++;} 
        // campaignMapEnabled
        var map3DisableCount = 0; // campaignMapDisabled
        // campaignMapDisabled
        if (map3DisableCount < 1 && map3EnableCount >= 1) {ret.push("New Allies.map");} // campaignMapAdd
        var map4EnableCount = 0; // campaignMapEnabled
        if (map3Won.readDataBool() === true) { map4EnableCount++;} 
        // campaignMapEnabled
        var map4DisableCount = 0; // campaignMapDisabled
        // campaignMapDisabled
        if (map4DisableCount < 1 && map4EnableCount >= 1) {ret.push("Fear Experiment.map");} // campaignMapAdd
        var map5EnableCount = 0; // campaignMapEnabled
        if (map4Won.readDataBool() === true) { map5EnableCount++;} 
        // campaignMapEnabled
        var map5DisableCount = 0; // campaignMapDisabled
        // campaignMapDisabled
        if (map5DisableCount < 1 && map5EnableCount >= 1) {ret.push("A Kind of Home.map");} // campaignMapAdd
        var map6EnableCount = 0; // campaignMapEnabled
        if (map5Won.readDataBool() === true) { map6EnableCount++;} 
        // campaignMapEnabled
        var map6DisableCount = 0; // campaignMapDisabled
        // campaignMapDisabled
        if (map6DisableCount < 1 && map6EnableCount >= 1) {ret.push("A new Threat.map");} // campaignMapAdd
        var map7EnableCount = 0; // campaignMapEnabled
        if (map6Won.readDataBool() === true) { map7EnableCount++;} 
        // campaignMapEnabled
        var map7DisableCount = 0; // campaignMapDisabled
        // campaignMapDisabled
        if (map7DisableCount < 1 && map7EnableCount >= 1) {ret.push("The Beast.map");} // campaignMapAdd
        var map8EnableCount = 0; // campaignMapEnabled
        if (map7Won.readDataBool() === true) { map8EnableCount++;} 
        // campaignMapEnabled
        var map8DisableCount = 0; // campaignMapDisabled
        // campaignMapDisabled
        if (map8DisableCount < 1 && map8EnableCount >= 1) {ret.push("Almost Home.map");} // campaignMapAdd
        var map9EnableCount = 0; // campaignMapEnabled
        if (map8Won.readDataBool() === true) { map9EnableCount++;} 
        // campaignMapEnabled
        var map9DisableCount = 0; // campaignMapDisabled
        // campaignMapDisabled
        if (map9DisableCount < 1 && map9EnableCount >= 1) {ret.push("A Storm Brews.map");} // campaignMapAdd
        var map10EnableCount = 0; // campaignMapEnabled
        if (map9Won.readDataBool() === true) { map10EnableCount++;} 
        // campaignMapEnabled
        var map10DisableCount = 0; // campaignMapDisabled
        // campaignMapDisabled
        if (map10DisableCount < 1 && map10EnableCount >= 1) {ret.push("History of Hate.map");} // campaignMapAdd
        var map11EnableCount = 0; // campaignMapEnabled
        if (map10Won.readDataBool() === true) { map11EnableCount++;} 
        // campaignMapEnabled
        var map11DisableCount = 0; // campaignMapDisabled
        // campaignMapDisabled
        if (map11DisableCount < 1 && map11EnableCount >= 1) {ret.push("Greyfield Strikes.map");} // campaignMapAdd
        var map12EnableCount = 0; // campaignMapEnabled
        if (map11Won.readDataBool() === true) { map12EnableCount++;} 
        // campaignMapEnabled
        var map12DisableCount = 0; // campaignMapDisabled
        // campaignMapDisabled
        if (map12DisableCount < 1 && map12EnableCount >= 1) {ret.push("A Hero's Farewell.map");} // campaignMapAdd
        var map13EnableCount = 0; // campaignMapEnabled
        if (map12Won.readDataBool() === true) { map13EnableCount++;} 
        // campaignMapEnabled
        var map13DisableCount = 0; // campaignMapDisabled
        // campaignMapDisabled
        if (map13DisableCount < 1 && map13EnableCount >= 1) {ret.push("Icy Retreat.map");} // campaignMapAdd
        var map14EnableCount = 0; // campaignMapEnabled
        if (map13Won.readDataBool() === true) { map14EnableCount++;} 
        // campaignMapEnabled
        var map14DisableCount = 0; // campaignMapDisabled
        // campaignMapDisabled
        if (map14DisableCount < 1 && map14EnableCount >= 1) {ret.push("Hope Rising.map");} // campaignMapAdd
        var map15EnableCount = 0; // campaignMapEnabled
        if (map14Won.readDataBool() === true) { map15EnableCount++;} 
        // campaignMapEnabled
        var map15DisableCount = 0; // campaignMapDisabled
        // campaignMapDisabled
        if (map15DisableCount < 1 && map15EnableCount >= 1) {ret.push("The Creeper.map");} // campaignMapAdd
        var map16EnableCount = 0; // campaignMapEnabled
        if (map15Won.readDataBool() === true) { map16EnableCount++;} 
        // campaignMapEnabled
        var map16DisableCount = 0; // campaignMapDisabled
        // campaignMapDisabled
        if (map16DisableCount < 1 && map16EnableCount >= 1) {ret.push("Panic in the Ranks.map");} // campaignMapAdd
        var map17EnableCount = 0; // campaignMapEnabled
        if (map16Won.readDataBool() === true) { map17EnableCount++;} 
        // campaignMapEnabled
        var map17DisableCount = 0; // campaignMapDisabled
        // campaignMapDisabled
        if (map17DisableCount < 1 && map17EnableCount >= 1) {ret.push("Salvation.map");} // campaignMapAdd
        var map18EnableCount = 0; // campaignMapEnabled
        if (map17Won.readDataBool() === true) { map18EnableCount++;} 
        // campaignMapEnabled
        var map18DisableCount = 0; // campaignMapDisabled
        // campaignMapDisabled
        if (map18DisableCount < 1 && map18EnableCount >= 1) {ret.push("Waylon Flies Again.map");} // campaignMapAdd
        var map19EnableCount = 0; // campaignMapEnabled
        if (map18Won.readDataBool() === true) { map19EnableCount++;} 
        // campaignMapEnabled
        var map19DisableCount = 0; // campaignMapDisabled
        // campaignMapDisabled
        if (map19DisableCount < 1 && map19EnableCount >= 1) {ret.push("Lin's Gambit.map");} // campaignMapAdd
        var map20EnableCount = 0; // campaignMapEnabled
        if (map19Won.readDataBool() === true) { map20EnableCount++;} 
        // campaignMapEnabled
        var map20DisableCount = 0; // campaignMapDisabled
        // campaignMapDisabled
        if (map20DisableCount < 1 && map20EnableCount >= 1) {ret.push("The Great Owl.map");} // campaignMapAdd
        var map21EnableCount = 0; // campaignMapEnabled
        if (map20Won.readDataBool() === true) { map21EnableCount++;} 
        // campaignMapEnabled
        var map21DisableCount = 0; // campaignMapDisabled
        // campaignMapDisabled
        if (map21DisableCount < 1 && map21EnableCount >= 1) {ret.push("Sacrificial Lamb.map");} // campaignMapAdd
        var map22EnableCount = 0; // campaignMapEnabled
        if (map21Won.readDataBool() === true) { map22EnableCount++;} 
        // campaignMapEnabled
        var map22DisableCount = 0; // campaignMapDisabled
        // campaignMapDisabled
        if (map22DisableCount < 1 && map22EnableCount >= 1) {ret.push("Crash Landing.map");} // campaignMapAdd
        var map23EnableCount = 0; // campaignMapEnabled
        if (map22Won.readDataBool() === true) { map23EnableCount++;} 
        // campaignMapEnabled
        var map23DisableCount = 0; // campaignMapDisabled
        // campaignMapDisabled
        if (map23DisableCount < 1 && map23EnableCount >= 1) {ret.push("Final Experiment.map");} // campaignMapAdd
        var map24EnableCount = 0; // campaignMapEnabled
        // campaignMapEnabled
        var map24DisableCount = 0; // campaignMapDisabled
        // campaignMapDisabled
        if (map24DisableCount < 1 && map24EnableCount >= 0) {ret.push("Days of Ruin.map");} // campaignMapAdd
        return ret;
    }; // campaignMaps
    this.mapFinished = function(campaign, map, result) { // campaignMapFinished
        var variables = campaign.getVariables();
        var mapVar = variables.createVariable(map.getMapName());
        mapVar.writeDataBool(result);
    }; // campaignMapFinished
    this.getCampaignFinished = function(campaign){ // campaignFinished
        var variables = campaign.getVariables();
        var wonCounter = 0;
        return false;
    } // campaignFinished
// campaign
};
Constructor.prototype = BASECAMPAIGN;
var campaignScript = new Constructor();
