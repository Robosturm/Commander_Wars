var Constructor = function()
{    
    this.getBuildingGroup = function()
    {
        return 5;
    };
    this.init = function (building)
    {
        building.setHp(100);
        building.setAlwaysVisble(true);
    };
    this.loadSprites = function(building, neutral, map)
    {
        if (map !== null)
        {
            var day = map.getCurrentDay();
            if (day % 3 === 1)
            {
                building.loadSprite("nest_laser+S", false, 400, Qt.point(0, building.getImageSize()));
            }
            else if (day % 3 === 2)
            {
                building.loadSprite("nest_laser+SE", false, 400, Qt.point(0, building.getImageSize()));
            }
            else if (day % 3 === 0)
            {
                building.loadSprite("nest_laser+SW", false, 400, Qt.point(0, building.getImageSize()));
            }
            if (day > 1)
            {
                audio.playSound("nestlaserrotate.wav");
            }

        }
        else
        {
            building.loadSprite("nest_laser+S", false, 400, Qt.point(0, building.getImageSize()));
        }
    };
    this.getBaseIncome = function()
    {
        return 0;
    };
    this.getDefense = function()
    {
        return 0;
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_blackholebuilding";
    };
    this.getName = function()
    {
        return qsTr("Nest");
    };
    this.getDescription = function()
    {
        return qsTr("Nest of Dark Matter. Currently only a visual.");
    };
    this.getBuildingWidth = function()
    {
        return 3;
    };
    this.getBuildingHeigth = function()
    {
        return 1;
    };
    this.canBuildingBePlaced = function(terrain, building, map)
    {
        return BUILDING.canLargeBuildingPlaced(terrain, building, ZNEST_NEST_LASER.getBuildingWidth(), ZNEST_NEST_LASER.getBuildingHeigth(), map);
    };
    this.startOfTurn = function(building, map)
    {
        building.unloadSprites();
        var variables = building.getVariables();
        var day = map.getCurrentDay();
        if (day % 3 === 1)
        {
            building.loadSprite("nest_laser+S", false, 400, Qt.point(0, building.getImageSize()));
        }
        else if (day % 3 === 2)
        {
            building.loadSprite("nest_laser+SE", false, 400, Qt.point(0, building.getImageSize()));
        }
        else if (day % 3 === 0)
        {
            building.loadSprite("nest_laser+SW", false, 400, Qt.point(0, building.getImageSize()));
        }
        if (day > 1)
        {
            audio.playSound("nestlaserrotate.wav");
        }
        building.setFireCount(1);
    };
    this.getActionTargetFields = function(building)
    {
        var map = building.getMap();
        var targets = globals.getEmptyPointArray();
        var day = map.getCurrentDay();
        if (day % 3 === 1)
        {
            for (var i = 1; i < 20; i++)
            {
                targets.append(Qt.point(-1, i));
            }
        }
        else if (day % 3 === 2)
        {
            for (var i = 1; i < 20; i++)
            {
                targets.append(Qt.point(i - 1, i));
            }
        }
        else if (day % 3 === 0)
        {
            for (var i = 1; i < 20; i++)
            {
                targets.append(Qt.point(-i - 1, i));
            }
        }
        return targets;
    };
    this.getRotation = function(building)
    {
        var map = building.getMap();
        var day = map.getCurrentDay();
        if (day % 3 === 1)
        {
            return 0;
        }
        else if (day % 3 === 2)
        {
            return -45;
        }
        else if (day % 3 === 0)
        {
            return 45;
        }
    };
    this.usesMapLayer = function()
    {
        return true;
    };
    this.getIsAttackable = function(building, x, y)
    {
        var buildX = building.getX();
        var buildY = building.getY();
        if (y === buildY && buildX - 1 === x)
        {
            return true;
        }
        return false;
    };
    this.onDestroyed = function(building, map)
    {
        // called when the terrain is destroyed and replacing of this terrain starts
        var x = building.getX();
        var y = building.getY();
        var animation = GameAnimationFactory.createAnimation(map, x - 1, y);
        animation.addSprite("explosion+land", -map.getImageSize() / 2, -map.getImageSize(), 0, 2);
        animation.addScreenshake(30, 0.95, 1000, 200);
        animation.setSound("explosion+land.wav");
        map.getTerrain(x, y).loadBuilding("ZNEST_NEST_LASER_DESTROYED");
    };
    this.getDamage = function(building, unit)
    {
        return 8;
    };
    this.actionList = ["ACTION_NEST_LASER"];
}

Constructor.prototype = BUILDING;
var ZNEST_NEST_LASER = new Constructor();
