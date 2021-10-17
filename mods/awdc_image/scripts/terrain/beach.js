BEACH.loadBaseSprite = function (terrain) 
{
    var surroundingsLand = terrain.getSurroundings("SEA", true, true, GameEnums.Directions_Direct, false);
    var surroundingsBeach = terrain.getSurroundings("BEACH", false, false, GameEnums.Directions_Direct, false);
    if (surroundingsLand !== "")
    {
        if (surroundingsLand === "+S" && surroundingsBeach.includes("+N"))
        {
            surroundingsBeach = surroundingsBeach.replace("+N", "");
        }
        if (surroundingsLand === "+N" && surroundingsBeach.includes("+S"))
        {
            surroundingsBeach = surroundingsBeach.replace("+S", "");
        }
        if (surroundingsLand === "+E" && surroundingsBeach.includes("+W"))
        {
            surroundingsBeach = surroundingsBeach.replace("+W", "");
        }
        if (surroundingsLand === "+W" && surroundingsBeach.includes("+E"))
        {
            surroundingsBeach = surroundingsBeach.replace("+E", "");
        }
        terrain.loadBaseSprite("beach" + surroundingsBeach + "+land" + surroundingsLand);

    }
    else if (surroundingsBeach !== "")
    {
        terrain.loadBaseSprite("beach");
    }
    else
    {
        terrain.loadBaseSprite("beach");
    }
};