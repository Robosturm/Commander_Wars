var WEATHER =
{
    getWeatherName : function()
    {

        return "";
    },

    getWeatherTerrainSprite : function()
    {
        // only called when the weather is active
        return "";
    },

    getWeatherSymbol : function()
    {
        return "";
    },

    getOffensiveModifier : function(weather)
    {
        // only called when the weather is active
        return 0;
    },

    getDefensiveModifier : function(weather)
    {
        // only called when the weather is active
        return 0;
    },

    getMovementCostModifier : function(weather)
    {
        // only called when the weather is active
        return 0;
    },

    getFirerangeModifier : function(weather)
    {
        // only called when the weather is active
        return 0;
    },

    getVisionrangeModifier : function(weather)
    {
        // only called when the weather is active
        return 0;
    },

    activate : function(weather)
    {
        // called when the weather starts
    },

    deactivate : function(weather)
    {
        // called when the weather ends
    },

};
