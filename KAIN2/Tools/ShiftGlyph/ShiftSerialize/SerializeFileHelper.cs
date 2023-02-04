using System.IO;

namespace ShiftSerialize
{
    class SerializeFileHelper
    {
        public static void SerializeFile(string filePath)
        {
            bool isObject = filePath.Contains("kain2//object//");
            bool isArea = filePath.Contains("kain2//area//");
            bool isMusic = filePath.Contains("kain2//music//");
            bool isSFX = filePath.Contains("kain2//sfx//");

            var stream = File.Open(filePath, FileMode.Open);
            var reader = new BinaryReader(stream);

            if (isObject)
            {
                ShiftSerialize.RelocationTable relocationTable = new ShiftSerialize.RelocationTable();

                relocationTable.Serialize(reader);

                ShiftSerialize.Object obj = new ShiftSerialize.Object();

                //obj.Serialize(reader);
            }
        }
    }
}
