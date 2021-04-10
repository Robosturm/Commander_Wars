var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setVisionHigh(2);
        terrain.setTerrainName(WELD.getName());
        terrain.setHp(100);
    };
    this.getName = function()
    {
        return qsTr("Weak Wall");
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
        if (currentTerrainID === "SNOW")
        {
            terrain.loadBaseTerrain("SNOW");
        }
        else if (currentTerrainID === "DESERT")
        {
            terrain.loadBaseTerrain("DESERT");
        }
        else
        {
            terrain.loadBaseTerrain("PLAINS");
        }
    };
    this.loadBaseSprite = function(terrain)
    {
        var surroundings = terrain.getSurroundings("WALL,WEAK_WALL,ZGATE_E_W,ZGATE_N_S", false, false, GameEnums.Directions_Direct, true, true);
        var x = terrain.getX();
        var y = terrain.getY();
        if ((surroundings === ""))
        {
            terrain.loadBaseSprite("weak_wall+E+W");
        }
        else if ((surroundings === "+N+S"))
        {

            terrain.loadBaseSprite("weak_wall+N+S");
        }
        else if ((surroundings === "+E+W"))
        {
            terrain.loadBaseSprite("weak_wall+E+W");
        }
        else
        {
            terrain.loadBaseSprite("weak_wall+E+W");
        }
    };
    this.canBePlaced = function(x, y)
    {
        var terrain = map.getTerrain(x, y);
        var surroundings = terrain.getSurroundings("WALL,WEAK_WALL,ZGATE_E_W,ZGATE_N_S", false, false, GameEnums.Directions_Direct, true, true);
        if ((surroundings === "+E+W") || (surroundings === "+N+S"))
        {
            return true;
        }
        else
        {
            return false;
        }
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_weld";
    };
    this.onDestroyed = function(terrain)
    {
        // called when the terrain is destroyed and replacing of this terrain starts
        var x = terrain.getX();
        var y = terrain.getY();
        map.replaceTerrainOnly("PLAINS_DESTROYED", x, y);
        map.getTerrain(x, y).loadSprites();
        var animation = GameAnimationFactory.createAnimation(x, y);
        animation.addSprite("explosion+land", -map.getImageSize() / 2, -map.getImageSize(), 0, 1.5);
        animation.addScreenshake(30, 0.95, 1000, 200);
        animation.setSound("pipe_destroyed.wav");
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "fore_pipeline";
    };
    this.getDescription = function()
    {
        return qsTr("Dark Matter Walls can't be crossed by most units. The wall can be destroyed here to produce a path through the wall.");
    };

    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["weak_wall+E+W",
                "weak_wall+N+S"];
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "fore_walltop";
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        return "";
    };
};
Constructor.prototype = TERRAIN;
var WEAK_WALL = new Constructor();
