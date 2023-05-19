CO_ISABELLA.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_ISABELLA.activateSuperpower = function(co, powerMode, map)
{
	CO_ISABELLA.activatePower(co, map);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
