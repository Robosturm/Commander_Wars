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

getFogOfWarColor = function()
{
    // the color used for the fog of war overlay
    return "#64464646";
}

getFogOfMistColor = function()
{
    // the color used for the fog of war overlay
    return "#64B4B4B4";
}

getGridColor = function()
{
    // the color used for the grid overlay
    return "#B4464646";
}

getMultiturnFieldColor = function()
{
    // the color used for the multi turn path planing tiles
    return "#8000FF";
}

getTurnFieldColor = function()
{
    // the color used for the normal move tiles
    return "#80FF00";
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

getArmyDataFromTable = function(army, table)
{
    var armyName = army.toLowerCase();
    for (var i = 0; i < table.length; i++)
    {
        if (table[i][0] === armyName)
        {
           return table[i][1];
        }
    }
    return table[0][0];
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


sortDataArray = function(data)
{
    var sortedData = [];
    var dataSize = data.length;
    if (dataSize > 0)
    {
        sortedData.push(data[0]);
        for (var i = 1; i < dataSize; i++)
        {
            var currentLength = sortedData.length;
            for (var i2 = 0; i2 < currentLength; i2++)
            {
                if (data[i][0] < sortedData[i2][0])
                {
                    sortedData.splice(i2, 0, data[i]);
                    break;
                }
                else if (i2 === currentLength - 1)
                {
                    sortedData.push(data[i]);
                }
            }
        }
    }
    return sortedData;
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
