var Constructor = function()
{
    this.bonus = 5;
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action, luckmode, map)
    {
		if (CO_PERK.isActive(co))
		{
			if (map !== null)
			{
				if (map.onMap(atkPosX, atkPosY))
				{
					var terrainID = map.getTerrain(atkPosX, atkPosY).getID();
                    var isPlain = 	(terrainID === "PLAINS_PLASMA") ||
                                    (terrainID === "PLAINS_DESTROYED") ||
                                    (terrainID === "PLAINS");
                    if (isPlain)
                    {
                        return CO_PERK_PLAIN_FIGHTER.bonus;
                    }
				}
			}
		}
        return 0;
    };
    // Perk - Intel
    this.getDescription = function()
    {
        var text = qsTr("Increases the attack from plains by %0%.");
        text = replaceTextArgs(text, [CO_PERK_PLAIN_FIGHTER.bonus]);
        return text;
    };
    this.getIcon = function(map)
    {
        return "prairiedog";
    };
    this.getName = function()
    {
        return qsTr("Plain fighter");
    };
    this.getGroup = function()
    {
        return qsTr("Terrain");
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_PLAIN_FIGHTER = new Constructor();
