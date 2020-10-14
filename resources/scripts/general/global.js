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
replaceTextArgs = function(text, args)
{
    for (var i = 0; i < args.length; i++)
    {
        while (text.includes("%" + i.toString()))
        {
            text = text.replace("%" + i.toString(), args[i]);
        }
    }
    return text;
}

Math.random = function()
{
    console.print("Unsupported call to Math.random. Please use globals.randInt(min, max) instead", 4)
}
