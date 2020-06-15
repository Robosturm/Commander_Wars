Global = this;
getIndexOf1 = function(table, item)
{
    for (var i = 0; i < table.length; i++)
    {
        if (table[i][0] === item)
        {
           return i;
        }
    }
    console.print("Item not found " + item, 0);
    return -1;
}
