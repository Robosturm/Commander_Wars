WEAPON.calculateDamage = function(hp, damage, offBonus, defBonus, luckDamage = 0)
{
    if (defBonus <= 0)
    {
        offBonus += defBonus;
        defBonus = 1;
    }
    if (hp < 0)
    {
        hp = 0;
    }
    var calcDamage = (hp / 10.0) * (damage * offBonus) / 100 * ((200 - defBonus) / 100)
            + luckDamage * (hp / 10.0) * ((200 - defBonus) / 100);
	if (calcDamage < 0 && damage >= 0)
	{
		calcDamage = 0;
	}
    return calcDamage;
};
