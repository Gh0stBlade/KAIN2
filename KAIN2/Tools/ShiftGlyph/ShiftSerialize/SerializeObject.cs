using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;

namespace ShiftSerialize
{
    [Serializable]
    public unsafe class _Vertex
    {
        public short x;
        public short y;
        public short z;
    }
    
    [Serializable]
    public unsafe class _MVertex
    {
        public _Vertex vertex;
        public ushort normal;
    }

    [Serializable]
    public unsafe class _SVectorNoPad
    {
        public short x;
        public short y;
        public short z;
    }

    [Serializable]
    public unsafe class _Face
    {
        public ushort v0;
        public ushort v1;
        public ushort v2;
    };

    [Serializable]
    public unsafe class _MFace
    {
        _Face face;
        byte normal;
        byte flags;
        long color;
    };

    [Serializable]
    public unsafe class _HInfo
    {
        long numHFaces;
        public _HFace *hfaceList;
        long numHSpheres;
        public _HSphere *hsphereList;
        long numHBoxes;
        public _HBox *hboxList;
    };

    [Serializable]
    public unsafe class _Segment
    {
        long flags; // size=0, offset=0
        short firstTri; // size=0, offset=4
        short lastTri; // size=0, offset=6
        short firstVertex; // size=0, offset=8
        short lastVertex; // size=0, offset=10
        short px; // size=0, offset=12
        short py; // size=0, offset=14
        short pz; // size=0, offset=16
        short parent; // size=0, offset=18
        struct _HInfo *hInfo; // size=24, offset=20
};

    [Serializable]
    public unsafe class _Model
    {
        public long numVertices;
        public _MVertex* vertexList;
        public long numNormals;
        public _SVectorNoPad* normalList;
        public long numFaces;
        public _MFace* faceList;
        public long numSegments;
        public struct _Segment *segmentList;
        public struct AniTex *aniTextures;
        public short maxRad;
        public short pad;
        public long maxRadSq;
        public struct MultiSpline *multiSpline;
        public struct TextureMT3 *startTextures;
        public struct TextureMT3 *endTextures;
    }

    [Serializable]
    public unsafe class Object
    {
        public long oflags;
        public short id;
        public short sfxFileHandle;
        public short numModels;
        public short numAnims;
        public List<_Model> modelList;
        public List<_G2AnimKeylist_Type> animList;
        public short introDist;
        public short vvIntroDist;
        public short removeDist;
        public short vvRemoveDist;
        public IntPtr* data;
        public sbyte* script;
        public sbyte* name;
        public byte* soundData;
        public long oflags2;
        public short sectionA;
        public short sectionB;
        public short sectionC;
        public short numberOfEffects;
        public ObjectEffect* effectList;
        public ulong* relocList;
        public IntPtr* relocModule;
        publicVramSize vramSize;

        public void Serialize(RelocationTable relocationTable, BinaryReader reader)
        {
            IFormatter formatter = new BinaryFormatter();

            //formatter.Serialize(reader, this);
        }
    }
}