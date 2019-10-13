ACTION_UNLOAD.performPostAnimation = function(postAnimation)
{
    // unloading the units here :)
    for (var i = 0; i < ACTION_UNLOAD.postAnimationTransportUnits.length; i++)
    {
        // check if the field is empty before unloading
        if (map.getTerrain(ACTION_UNLOAD.postAnimationTransportUnitsPosX[i],
                           ACTION_UNLOAD.postAnimationTransportUnitsPosY[i]).getUnit() === null)
        {
            ACTION_UNLOAD.postAnimationUnit.unloadUnit(ACTION_UNLOAD.postAnimationTransportUnits[i],
                                                       Qt.point(ACTION_UNLOAD.postAnimationTransportUnitsPosX[i],
                                                                ACTION_UNLOAD.postAnimationTransportUnitsPosY[i]));
            // set unloaded unit to have been moved
            var unit = map.getTerrain(ACTION_UNLOAD.postAnimationTransportUnitsPosX[i],
                                      ACTION_UNLOAD.postAnimationTransportUnitsPosY[i]).getUnit();
            unit.setHasMoved(true);
        }
    }
    audio.playSound("unload.wav");
    ACTION_UNLOAD.postAnimationUnit = null;
    ACTION_UNLOAD.postAnimationTransportUnits = [];
    ACTION_UNLOAD.postAnimationTransportUnitsPosX = [];
    ACTION_UNLOAD.postAnimationTransportUnitsPosY = [];
};
