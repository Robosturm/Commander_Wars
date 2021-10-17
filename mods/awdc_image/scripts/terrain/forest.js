FOREST.loadBaseSprite = function(terrain)
{
    var surroundings = terrain.getSurroundings("OREST", false, false, GameEnums.Directions_East, false);
    surroundings += terrain.getSurroundings("FOREST", false, false, GameEnums.Directions_West, false);
    terrain.loadBaseSprite("forest" + surroundings);
};