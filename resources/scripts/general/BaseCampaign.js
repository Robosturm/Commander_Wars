var BASECAMPAIGN =
{
    getDescription : function()
    {
        // description of this campaign
        return "";
    },

    getAuthor : function()
    {
        // author of this campaign
        return "";
    },

    getCampaignName : function()
    {
        // name of this campaign
        return "";
    },

    getCurrentCampaignMaps : function(campaign)
    {
        // return a string list of current selectable maps
        // the first item is the folder the rest are the map-files in the folder
        return ["", ""];
    },

    getCampaignFinished : function(campaign)
    {
        // should return true when the campaign is at it's end
        return false;
    },

    mapFinished : function(campaign, map, result)
    {
        // called when a map has been finished
        // campaign = campaign object
        // map = finished map object
        // result = true -> won, false -> lost
        // it's up to the campaign developer how to handle the map result
    },

    getSelectableCOs : function(campaign, map, player, coIndex)
    {
        // return the co's for a certain map a player [player] and the first or second co [coIndex]
        // make no co selectable
        return [""];
        // make all co's including random co selectable
        // return [];
        // example for making three co's selectable
        // return ["CO_ANDY", "CO_MAX", "CO_SAMI"];
    },

    getAllowArmyCustomization : function(campaign, map)
    {
        // if true you can change the army style and color for each player
        return false;
    },
};
