CO_CAULDER.init = function(co)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(0);
};
CO_CAULDER.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
{
	if (co.inCORange(Qt.point(defPosX, defPosY), defender))
	{
		return 60;
	}
	else
	{
		return 0;
	}
};
CO_CAULDER.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                             defender, defPosX, defPosY, isDefender)
{
	if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
	{
		return 60;
	}
	else
	{
		return 0;
	}
};