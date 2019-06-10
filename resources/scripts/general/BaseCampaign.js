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
};
