var Constructor = function()
{
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, map)
    {
		if (CO_PERK.isActive(co))
		{
			var towers = co.getOwner().getBuildingCount("TOWER");        
			return towers * 5;
		}
        return 0;
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return qsTr("Increases the defense boost per tower by 5%.");
    };
    this.getIcon = function(map)
    {
        return "towerpower";
    };
    this.getName = function()
    {
        return qsTr("Com Defender");
    };
    this.getGroup = function()
    {
        return qsTr("Defensive");
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_COM_DEFENDER = new Constructor();
