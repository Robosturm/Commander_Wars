var MapExtendingWithMap =
{
    getDirectionNames : function()
    {
        return [qsTr("North"), qsTr("East"), qsTr("South"), qsTr("West")];
    },
    onDirectionChanged : function(value)
    {
        if (value === 0)
        {
            currentMenu.setDirection(GameEnums.Directions_North);
        }
        else if (value === 1)
        {
            currentMenu.setDirection(GameEnums.Directions_East);
        }
        else if (value === 2)
        {
            currentMenu.setDirection(GameEnums.Directions_South);
        }
        else
        {
            currentMenu.setDirection(GameEnums.Directions_West);
        }
    }
}
