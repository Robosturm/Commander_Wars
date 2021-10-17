MOUNTAIN.loadBaseSprite = function(terrain)
{
    var surroundings = terrain.getSurroundings("MOUNTAIN", false, false, GameEnums.Directions_East, false);
    surroundings += terrain.getSurroundings("MOUNTAIN", false, false, GameEnums.Directions_West, false);
    terrain.loadBaseSprite("mountain" + surroundings);
};