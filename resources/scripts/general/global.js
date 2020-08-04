Global = this;
getIndexOf1 = function(table, item)
{
    for (var i = 0; i < table.length; i++)
    {
        if (table[i][0] === item)
        {
           return i;
        }
    }
    GameConsole.print("Item not found " + item, 0);
    return -1;
}
getArmyNameFromPlayerTable = function(player, table)
{
    return Global.getArmyNameFromTable(player.getArmy(), table);
}
getArmyNameFromTable = function(army, table)
{
    var armyName = army.toLowerCase();
    for (var i = 0; i < table.length; i++)
    {
        if (table[i][0] === armyName)
        {
           return table[i][1].toLowerCase();
        }
    }
    return "os";
}
