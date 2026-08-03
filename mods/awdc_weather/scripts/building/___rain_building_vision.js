var AWDC_RAIN_BUILDING_VISION =
{
    wrapGetVision : function(getVision)
    {
        return function(building, map)
        {
            var vision = getVision(building, map);
            var owner = building.getOwner();
            if (vision >= 0 &&
                owner !== null &&
                map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_RAIN" &&
                !owner.getWeatherImmune())
            {
                return 0;
            }
            return vision;
        };
    },

    apply : function()
    {
        var buildingIds = buildingSpriteManager.getLoadedBuildings();
        for (var i = 0; i < buildingIds.length; ++i)
        {
            var buildingData = Global[buildingIds[i]];
            if (buildingData !== null &&
                typeof buildingData !== "undefined" &&
                typeof buildingData.getVision === "function")
            {
                buildingData.getVision = AWDC_RAIN_BUILDING_VISION.wrapGetVision(buildingData.getVision);
            }
        }
    },
};

AWDC_RAIN_BUILDING_VISION.apply();
