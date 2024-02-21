var Constructor = function()
{
    this.getBuildingGroup = function()
    {
        return 2;
    };
	
	this.getBaseIncome = function()
	{
		return 0;
	};
    
    this.loadSprites = function(building, neutral, map)
    {
		// masking now includes neutral colors
        building.loadSpriteV2("field_base+mask", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), "", neutral);
		
		//building.loadSpriteV2("field_base+shade", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), TERRAIN.getBuildingTerrainPalette(building));
		
        if (building.getOwnerID() >= 0 && !neutral)
        {
            building.loadSpriteV2("field_base", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), "", neutral);
			
			building.loadSpriteV2("field_base+shade", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), TERRAIN.getBuildingTerrainPalette(building));
        }
        else
        {
            building.loadSpriteV2("field_base+neutral", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), "", neutral);
			
			building.loadSpriteV2("field_base+shade", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), TERRAIN.getBuildingTerrainPalette(building));
        }
    };
	
	this.addCaptureAnimationBuilding = function(animation, building, startPlayer, capturedPlayer)
    {
        animation.addBuildingSprite("field_base+mask", startPlayer , capturedPlayer, GameEnums.Recoloring_Matrix);
    };
	
    this.getName = function()
    {
        return qsTr("Field Base");
    };

    this.getDefense = function()
    {
        return 3;
    };

    this.getMiniMapIcon = function()
    {
        return "minimap_hq";
    };

    this.getDescription = function()
    {
        return qsTr("<div c='#00ff00'>Battle ends </div><r>when an army's last Field Base is captured. The Field Base cannot produce, repair or resupply units");
    };

    this.getVisionHide = function(building)
    {
        return true;
    };

    this.getEditorPlacementSound = function()
    {
        return "placeHq.wav";
    };
    this.getVision = function(building, map)
    {
        return map.getGameRules().getBuildingVision();
    };
}

Constructor.prototype = BUILDING;
var FIELD_BASE = new Constructor();
