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

onZoomLevelChanged = function()
{
}

Math.random = function()
{
    console.print("Unsupported call to Math.random. Please use globals.randInt(min, max) instead", 4)
}

bresenhamFromTerrain = function(terrain1, terrain2, includeStartEnd, asTerrainArray)
{
    return Global.bresenhamLine(terrain1.getX(), terrain1.getY(), terrain2.getX(), terrain2.getY(), includeStartEnd, asTerrainArray);
}

bresenhamLine = function (x0, y0, x1, y1, includeStartEnd, asTerrainArray)
{
    var ret = [];
    if (typeof map !== 'undefined')
    {
        var sx0 = x0;
        var sy0 = y0;
        var dx = Math.abs(x1 - x0);
        var sx = x0 < x1 ? 1 : -1;
        var dy = Math.abs(y1 - y0);
        var sy = y0 < y1 ? 1 : -1;
        var err = (dx > dy ? dx : -dy) / 2;
        while (true)
        {
            var foundTerrain = map.getTerrain(x0, y0);
            if ((x0 === sx0 && y0 === sy0) ||
                (x0 === x1 && y0 === y1))
            {
                if (includeStartEnd === true)
                {
                    if(asTerrainArray === true)
                    {
                        ret.push(foundTerrain);
                    }
                    else
                    {
                        ret.push(Qt.point(x0, y0));
                    }
                }
            }
            else
            {
                if(asTerrainArray === true)
                {
                    ret.push(foundTerrain);
                }
                else
                {
                    ret.push(Qt.point(x0, y0));
                }
            }
            if (x0 === x1 && y0 === y1)
            {
                break;
            }
            else
            {
                var e2 = err;
                if (e2 > -dx) { err -= dy; x0 += sx; }
                if (e2 < dy) { err += dx; y0 += sy; }
            }
        }
    }
    return ret;
}
