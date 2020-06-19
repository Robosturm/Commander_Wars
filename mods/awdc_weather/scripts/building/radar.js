RADAR.getVision = function(building)
{
    if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_RAIN" &&
        !building.getOwner().getWeatherImmune())
    {
        return 0;
    }
    else
    {
        return 5;
    }
};
