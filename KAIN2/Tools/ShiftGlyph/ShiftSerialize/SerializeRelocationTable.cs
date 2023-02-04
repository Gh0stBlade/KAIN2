using System.Collections.Generic;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;
using System.Runtime.Serialization;
using System.Text;
using System;

namespace ShiftSerialize
{
    [Serializable]
    public class RelocationTable
    {
        public List<int> Relocations { get; set; }

        public RelocationTable()
        {
            Relocations = new List<int>();
        }

        public void Serialize(BinaryReader reader)
        {
            int relocationCount = reader.ReadInt32();

            for (int i = 0; i < relocationCount; i++)
            {
                Relocations.Add(reader.ReadInt32());
            }

            int tableSize = (relocationCount + 512 < 0) ? (relocationCount + 1023) : (relocationCount + 512);
            tableSize /= 512;
            tableSize *= 512;
            tableSize *= 4;
            reader.BaseStream.Position = tableSize;

            IFormatter formatter = new BinaryFormatter();
            Stream stream = new FileStream("MyFile.bin",
                                     FileMode.Create,
                                     FileAccess.Write, FileShare.None);
            formatter.Serialize(stream, this);
            stream.Close();
        }
    }
}