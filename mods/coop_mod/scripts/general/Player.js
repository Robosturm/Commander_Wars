PLAYER.startOfTurn = function(player)
{
    for (var i = 0; i < map.getPlayerCount(); i++)
    {
        var otherPlayer = map.getPlayer(i);
        if (otherPlayer.isAlly(player) && otherPlayer !== player)
        {
            var buildings = otherPlayer.getBuildings();
            for (var j = 0; j < buildings.size(); j++)
            {
                var building = buildings.at(j);
                var constructionList = building.getConstructionList();
                var repairList = building.getRepairTypes();
                var unit = building.getTerrain().getUnit();
                if ((unit !== null) &&
                        (unit.getOwner() === player) &&
                        ((repairList.indexOf(unit.getUnitType()) >= 0) ||
                         (constructionList.indexOf(unit.getUnitID()) >= 0)))
                {
                    var x = unit.getX();
                    var y = unit.getY();
                    if (unit.canBeRepaired(Qt.point(x, y)))
                    {
                        unit.refill();
                        var repairAmount = 2 + unit.getRepairBonus(Qt.point(x, y));
                        UNIT.repairUnit(unit, repairAmount);
                        if (!unit.isStealthed(map.getCurrentViewPlayer()))
                        {
                            var animation = GameAnimationFactory.createAnimation(x, y);
                            var width = animation.addText(qsTr("REPAIR"), map.getImageSize() / 2 + 25, 2, 1);
                            animation.addBox("info", map.getImageSize() / 2, 0, width + 32, map.getImageSize(), 400);
                            animation.addSprite("repair", map.getImageSize() / 2 + 8, 1, 400, 1.7);
                        }
                    }
                }
            }
            buildings.remove();
        }
    }
    PLAYER.coop_ModApplyDefenceDebuf(player);
};

PLAYER.coop_ModApplyDefenceDebuf = function(player)
{
    var variables = player.getVariables();
    var boostVariable = variables.createVariable("COOP_MOD_DEFENSEBOOST_UNITS");
    var boosts = boostVariable.readDataListInt32();
    for (var i = 0; i < boosts.length; i++)
    {
        var unitId = boosts[i];
        var defenseUnit = map.getUnit(unitId);
        if (defenseUnit !== null)
        {
            // negate boost for the rest of the day
            defenseUnit.addDefensiveBonus(-200, 1);
        }
    }
    var empty = [];
    boostVariable.writeDataListInt32(empty);
};
