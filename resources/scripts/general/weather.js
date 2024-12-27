var WEATHER =
{
    getName : function(id)
    {
        return Global[id].getWeatherName();
    },

    getWeatherName : function()
    {

        return "";
    },
    getWeatherName : null,

    getDescription : function()
    {
        return qsTr("Unknown");
    },

    getIcon : function(id)
    {
        return Global[id].getWeatherSymbol();
    },

    getWeatherTerrainSprite : function(weather, map)
    {
        // only called when the weather is active
        return "";
    },
    getWeatherTerrainSprite : null,

    getWeatherSymbol : function(weather, map)
    {
        return "";
    },
    getWeatherSymbol : null,

    getOffensiveModifier : function(weather, map)
    {
        // only called when the weather is active
        return 0;
    },
    getOffensiveModifier : null,

    getDefensiveModifier : function(weather, map)
    {
        // only called when the weather is active
        return 0;
    },
    getDefensiveModifier : null,

    getMovementCostModifier : function(weather, unit, terrain, map)
    {
        // only called when the weather is active
        return 0;
    },
    getMovementCostModifier : null,

    getFirerangeModifier : function(weather, map, unit, terrain)
    {
        // only called when the weather is active
        return 0;
    },
    getFirerangeModifier : null,

    getMinFirerangeModifier : function(weather, map, unit, terrain)
    {
        // only called when the weather is active
        return 0;
    },
    getMinFirerangeModifier : null,

    getMovementpointModifier : function(weather, unit, terrain, map)
    {
        return 0;
    },
    getMovementpointModifier : null,

    getVisionrangeModifier : function(weather, map, unit, terrain)
    {
        // only called when the weather is active
        return 0;
    },
    getVisionrangeModifier : null,

    getMovementFuelCostModifier: function(weather, unit, fuelCost, map)
    {
        // only called when the weather is active
        return 0;
    },
    getMovementFuelCostModifier : null,

    activate : function(weather, map)
    {
        // called when the weather starts
    },
    activate : null,

    deactivate : function(weather, map)
    {
        // called when the weather ends
    },
    deactivate : null,

    getDefaultWeatherChance : function()
    {
        return 0;
    },
    getDefaultWeatherChance : null,
};
