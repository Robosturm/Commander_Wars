var Constructor = function()
{
    this.getAdditionalBuildingActions = function(co, building, map)
    {
		if (CO_PERK.isActive(co))
		{
			switch (co.getPowerMode())
			{
				case GameEnums.PowerMode_Tagpower:
				case GameEnums.PowerMode_Superpower:
					if (building.getBuildingID() === "TOWN" &&
						building.getOwner() === co.getOwner())
					{
						return "ACTION_BUILD_UNITS";
					}
					break;
				case GameEnums.PowerMode_Power:
					break;
				default:
					break;
			}
		}
        return "";
    };
	// Perk - Intel
    this.getDescription = function()
    {
        return qsTr("During your Superpower you can build units from cities.");
    };
    this.getIcon = function(map)
    {
        return "hachisoul";
    };
    this.getName = function()
    {
        return qsTr("Hachi's Soul");
    };
    this.getGroup = function()
    {
        return qsTr("Power");
    };
};

Constructor.prototype = CO_PERK;
var CO_PERK_HACHIS_SOUL = new Constructor();
