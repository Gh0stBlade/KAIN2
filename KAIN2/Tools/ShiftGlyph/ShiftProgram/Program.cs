using System;
using ShiftSerialize;

class ShiftGlyph
{
    static void Main(string[] args)
    {
        if(args.Length > 0)
        {
            SerializeFileHelper.SerializeFile(args[0]);
        }
    }
}