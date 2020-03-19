// this is the base class for terrain
var UNITRANKINGSYSTEM =
{
	co1 : -1, // don't change this value
	co2 : -2, // don't change this value
	// moddable part starts here:
    getMaxRang : function(unit)
    {
        return 3;
    },
    icons : ["", "lieutenant", "general", "veteran"],
    names : [qsTr("Soldier"), qsTr("Experienced"), qsTr("Veteran"), qsTr("Elite")],
    offensiveBonus : [0, 5, 10, 20],
    defensiveBonus : [0, 0,  0, 20],
    getIcon : function(rang)
	{
        if (rang === UNITRANKINGSYSTEM.co1)
		{
            return "co0";
		}
        else if (rang === UNITRANKINGSYSTEM.co2)
		{
            return "co1";
		}
		else
		{			
			return UNITRANKINGSYSTEM.icons[rang];
		}
    },
    increaseRang : function(unit)
	{
		var rang = unit.getUnitRank();
		if (rang >= 0)
		{
			rang = rang + 1;
            if (rang > UNITRANKINGSYSTEM.getMaxRang())
			{
                rang = UNITRANKINGSYSTEM.getMaxRang();
			}			
		}
		unit.setUnitRank(rang);
	},
	unloadIcons : function(unit)
	{
        // unload all unit ranking icons here
        for (var i = 0; i < UNITRANKINGSYSTEM.icons.length; i++)
        {
            unit.unloadIcon(UNITRANKINGSYSTEM.icons[i]);
        }
	},
    getName : function(rang)
    {
        if (rang === UNITRANKINGSYSTEM.co2)
        {
            return qsTr("CO 1");
        }
        else if (rang === UNITRANKINGSYSTEM.co1)
        {
            return qsTr("CO 2");
        }
        else
        {
            return UNITRANKINGSYSTEM.names[rang];
        }
    },
    getOffensiveBonus : function(unit)
    {
        var rang = unit.getUnitRank();
        if (rang < 0)
        {
            return 20;
        }
        return UNITRANKINGSYSTEM.offensiveBonus[rang];
    },
    getDefensiveBonus : function(unit)
    {
        var rang = unit.getUnitRank();
        if (rang < 0)
        {
            return 20;
        }
        return UNITRANKINGSYSTEM.defensiveBonus[rang];
    },
};
