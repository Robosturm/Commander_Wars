BUILDING.replenishUnit = function(building)
{
    // default impl replenishes our units
    // gets called at the start of a turn
    var constructionList = building.getConstructionList();
    var unit = building.getTerrain().getUnit();
    if ((unit !== null) &&
            (unit.getOwner() === building.getOwner()) &&
            (constructionList.indexOf(unit.getUnitID()) >= 0))
    {

        var x = unit.getX();
        var y = unit.getY();
        if (unit.canBeRepaired(Qt.point(x, y)))
        {
            // our unit and a repairable one
            // replenish it
            unit.refill(true);
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
};
