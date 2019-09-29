var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 0;
    };

    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(BEACH.getName());
    };

    this.getName = function()
    {
        return qsTr("Beach");
    };

    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
		terrain.loadBaseTerrain("SEA");
    };
    this.loadBaseSprite = function (terrain) {
            var surroundingsLand = terrain.getSurroundings("SEA", true, true, GameEnums.Directions_Direct, false);
            var surroundingsBeach = terrain.getSurroundings("BEACH", false, false, GameEnums.Directions_Direct, false);
            // var surroundingsSea = terrain.getSurroundings("SEA", false, false, GameEnums.Directions_Direct, false);
            // GameConsole.print("surroundingsLand: " + surroundingsLand, 1);
            // GameConsole.print("surroundingsBeach: " + surroundingsBeach, 1);

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
                if (surroundingsBeach === "+E+S")
                {
                    terrain.loadBaseSprite("beach" + surroundingsBeach);
                }
                if (surroundingsBeach === "+N+E")
                {
                    terrain.loadBaseSprite("beach" + surroundingsBeach);
                }
                if (surroundingsBeach === "+S+W")
                {
                    terrain.loadBaseSprite("beach" + surroundingsBeach);
                }
                if (surroundingsBeach === "+N+W")
                {
                    terrain.loadBaseSprite("beach" + surroundingsBeach);
                }
                terrain.loadBaseSprite("beach");
            }
            else
            {
                terrain.loadBaseSprite("beach");
            }
        };
        this.canBePlaced = function (x, y) {
            var terrain = map.getTerrain(x, y);
            var surroundingsLand = terrain.getSurroundings("SEA", true, true, GameEnums.Directions_Direct, false);
            var surroundingsSea = terrain.getSurroundings("SEA", false, true, GameEnums.Directions_All, false);
            var surroundingsBeach = terrain.getSurroundings("BEACH", false, false, GameEnums.Directions_Direct, false);
            // var c = (surroundingsLand === "" && surroundingsBeach === "");
            var c = (false);
            var d = (surroundingsLand.includes("+S") && surroundingsLand.includes("+E") && surroundingsSea.includes("+NW"));
            var e = (surroundingsLand.includes("+S") && surroundingsLand.includes("+W") && surroundingsSea.includes("+NE"));
            var f = (surroundingsLand.includes("+W") && surroundingsLand.includes("+S") && surroundingsSea.includes("+NE"));
            var g = (surroundingsLand.includes("+W") && surroundingsLand.includes("+N") && surroundingsSea.includes("+SE"));
            var h = (surroundingsLand.includes("+N") && surroundingsLand.includes("+W") && surroundingsSea.includes("+SE"));
            var i = (surroundingsLand.includes("+N") && surroundingsLand.includes("+E") && surroundingsSea.includes("+SW"));
            var j = (surroundingsLand.includes("+E") && surroundingsLand.includes("+S") && surroundingsSea.includes("+NW"));
            var k = (surroundingsLand.includes("+E") && surroundingsLand.includes("+N") && surroundingsSea.includes("+SW"));
            if (c) // || d || e || f || g || h || i || j || k)
            {
                return false;
            }
            else
            {
                return true;
            }
        }

    this.getMiniMapIcon = function()
    {
        return "minimap_beach";
    };

    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "fore_beach";
    };

    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        return "back_beach";
    };

    this.getDescription = function()
    {
        return qsTr("Beachs over great landing places for landing ground troops.");
    };
};
Constructor.prototype = TERRAIN;
var BEACH = new Constructor();
