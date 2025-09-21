#pragma once

namespace vml
{
    namespace textures
    {

        class RectPack;

        class RectPackNode
        {
            private:

                std::string    FileName;
                int            Px;
                int            Py;
                int            Width;
                int            Height;
                unsigned char* Data;
                int            Pitch;
                int            Size;
                int            BytesPerPixel;
                bool           WasPacked;

                friend class RectPack;

            public:

                // ----------------------------------------------------------------------------

                const std::string& GetFileName()      const { return FileName; }         // return filename
                int                GetWidth()         const { return Width; }            // return width texture dimension
                int                GetHeight()        const { return Height; }           // return height texture dimension
                int                GetSize()          const { return Size; }             // return texture area
                int                GetBytesPerPixel() const { return BytesPerPixel; }    // ret8urns texture bytes per pixell
                int                GetPitch()         const { return Pitch; }            // return texture pitch
                unsigned char* GetData()          const { return Data; }             // return data pointer
                int                GetPx()            const { return Px; }               // returns x coordinate in the atlas texture space
                int                GetPy()            const { return Py; }               // returns y coordinate in the atlas texture space
                bool               IsPacked()         const { return WasPacked; }        // returns if texture has been packed in the texture atlas

                // ----------------------------------------------------------------------------
                // ctor / dtor

                RectPackNode()
                {
                    Px            = -1;
                    Py            = -1;
                    Width         =  0;
                    Height        =  0;
                    Pitch         =  0;
                    Size          =  0;
                    BytesPerPixel =  0;
                    WasPacked     =  false;
                    Data          =  nullptr;
                }

                RectPackNode(const std::string& filename)
                {
                    FileName      =  filename;
                    Data          =  nullptr;
                    WasPacked     =  false;
                    Px            = -1;
                    Py            = -1;
                    Width         =  0;
                    Height        =  0;
                    Size          =  0;
                    Pitch         =  0;
                    BytesPerPixel =  0;

                    // load texture

                    Data = stbi_load(&filename[0], &Width, &Height, &BytesPerPixel, STBI_rgb_alpha);

                    if (!Data) {
                        vml::os::Message::Error("no data");
                    }

                    Size  = Width * Height;
                    Pitch = Width * BytesPerPixel;
                    Pitch = (Pitch + 3) & ~3;

                    //       std::cout << filename << " : " << Width << " , " << Height << " , " << BytesPerPixel << std::endl;
                }

                ~RectPackNode()
                {
                    vml::os::SafeDelete(Data);

                    //        std::cout << "deleting rectpacknode " << FileName << std::endl;
                }
        };

        ///////////////////////////////////////////////////////////////////////////////////////////////////////

        class RectPack
        {
            private:

                // ------------------------------------------------------

                uint32_t       InternalFlags;
                int            AtlasWidth;
                int            AtlasHeight;
                int            AtlasSize;
                int            BytesPerPixel;
                int            Pitch;
                float          Ratio;
                RectPackNode** Rects;

                // ------------------------------------------------------

                static const unsigned int TEXTURE_ATLAS_SUCCESS          = 0;
                static const unsigned int TEXTURE_ATLAS_NOT_POWER_OF_TWO = 1;
                static const unsigned int TEXTURE_ATLAS_FILE_ERROR       = 2;

                // ------------------------------------------------------

                bool IsPowerOfTwo(unsigned int x)
                {
                    return ((x != 0) && ((x & (~x + 1)) == x));
                }

                // ------------------------------------------------------

                void ReleaseAll()
                {
                    // delete allocatred mem

                    vml::os::SafeDelete(Data);
                    vml::os::SafeDelete(StbRectNodes);
                    vml::os::SafeDelete(StbRects);
                    for (size_t i = 0; i < RectsCount; ++i)
                        vml::os::SafeDelete(Rects[i]);
                    vml::os::SafeDeleteArray(Rects);

                    // reset data members

                    InternalFlags = 0;
                    AtlasWidth    = 0;
                    AtlasHeight   = 0;
                    AtlasSize     = 0;
                    BytesPerPixel = 0;
                    Pitch         = 0;
                    Ratio         = 0.0f;
                }

            public:

                // ------------------------------------------------------

                unsigned char* Data;
                stbrp_node*    StbRectNodes;
                stbrp_rect*    StbRects;
                int            RectsCount;
                stbrp_context  StbCtx{ 0 };

                //---------------------------------------------------------------------
                // copy constructor is private
                // no copies allowed since classes
                // are referenced

                RectPack(const RectPack& RectPack) = delete;

                //---------------------------------------------------------------------
                // overload operator is private, 
                // no copies allowed since classes
                // are referenced

                RectPack& operator = (const RectPack& RectPack) = delete;

                // ------------------------------------------------------
                // return if initialized 

                bool IsInitialized() const
                {
                    return vml::utils::bits32::Get(InternalFlags, vml::utils::InternalFlags::INITTED) == 1;
                }

                // ------------------------------------------------------

                int           GetWidth()          const { return AtlasWidth; }     // return width texture dimension
                int           GetHeight()         const { return AtlasHeight; }     // return height texture dimension
                int           GetRectSize()       const { return AtlasSize; }     // return texture area
                int           GetBytesPerPixel()  const { return BytesPerPixel; }     // ret8urns texture bytes per pixell
                int           GetPitch()          const { return Pitch; }     // return texture pitch
                float         GetRatio()          const { return Ratio; }     // return ratio betwwen used and empty texture space
                RectPackNode* GetRectAt(size_t i) const { return Rects[i]; }    // gets rect node at index

                const std::string& GetErrorString(const unsigned int& errcode) const
                {
                    switch (errcode)
                    {
                        case TEXTURE_ATLAS_SUCCESS: return "Success";                                   break;
                        case TEXTURE_ATLAS_NOT_POWER_OF_TWO: return "Non power of two input textures";  break;
                        case TEXTURE_ATLAS_FILE_ERROR: return "IO Error";                               break;
                    }
                    return "Undefined ErrorCode";
                }

                // ------------------------------------------------------

                int Init(int w, int h)
                {
                    // clean memory

                    ReleaseAll();

                    // check if dimesions oare power of two

                    if (!IsPowerOfTwo(w) || !IsPowerOfTwo(h))
                        return TEXTURE_ATLAS_NOT_POWER_OF_TWO;

                    // set data

                    AtlasWidth    = w;
                    AtlasHeight   = h;
                    BytesPerPixel = 4;
                    AtlasSize     = AtlasWidth * AtlasHeight;
                    Pitch         = AtlasWidth * BytesPerPixel;
                    Pitch         = (Pitch + 3) & ~3;
                    RectsCount    = 0;

                    StbRectNodes = new stbrp_node[AtlasWidth];
                    StbRects     = new stbrp_rect[AtlasWidth];
                    Rects        = new RectPackNode * [AtlasWidth];

                    // allocate texture data

                    Data = new unsigned char[AtlasSize * BytesPerPixel];

                    // initialize texture data

                    for (int j = 0; j < AtlasHeight; ++j)
                    {
                        for (int i = 0; i < AtlasWidth; ++i)
                        {
                            int offset = (i + j * AtlasWidth) * 4;

                            Data[offset] = 0;
                            Data[offset + 1] = 0;
                            Data[offset + 2] = 0;
                            Data[offset + 3] = 255;
                        }
                    }

                    //set flag to initted

                    vml::utils::bits32::SetToTrue(InternalFlags, vml::utils::InternalFlags::INITTED);

                    // return success

                    return TEXTURE_ATLAS_SUCCESS;

                }

                // ------------------------------------------------------

                int Init(int w)
                {
                    return Init(w, w);
                }

                // ------------------------------------------------------

                bool AddTexture(const std::string& filename)
                {
                    if (!IsInitialized())
                        vml::os::Message::Error("RectPack : RectPack not initted");

                    // check if texture has already been inserted

                    for (size_t i = 0; i < RectsCount; ++i)
                        if (Rects[i]->GetFileName() == filename)
                            return false;

                    // add texture

                    Rects[RectsCount++] = new RectPackNode(filename);

                    return true;
                }

                // ------------------------------------------------------

                int Go()
                {
                    if (!IsInitialized())
                        vml::os::Message::Error("RectPack : RectPack not initted");
                    if (RectsCount == 0)
                        vml::os::Message::Error("RectPack : no rects to pack");

                    // check if each single textures is a power of two

                    for (int i = 0; i < RectsCount; i++)
                        if (!IsPowerOfTwo(Rects[i]->Width) || !IsPowerOfTwo(Rects[i]->Height))
                            return TEXTURE_ATLAS_NOT_POWER_OF_TWO;

                    // init SbRects

                    for (int i = 0; i < RectsCount; i++)
                    {
                        StbRects[i].id = i;
                        StbRects[i].w  = Rects[i]->Width;
                        StbRects[i].h  = Rects[i]->Height;
                    }

                    // for (int i = 0; i < RectsCount; i++)
                    // {
                    //     std::cout << i << " " << Rects[i]->GetFileName() << std::endl;
                    // }

                    // create texture atlas positions

                    stbrp_init_target(&StbCtx, AtlasWidth, AtlasHeight, StbRectNodes, AtlasWidth);
                    stbrp_pack_rects(&StbCtx, StbRects, RectsCount);

                    // compute ratio between empty and filled space

                    size_t totalrectsize = 0;
                    for (int i = 0; i < RectsCount; i++)
                        if (StbRects[i].was_packed)
                            totalrectsize += Rects[i]->Size;
                    Ratio = 100.0f * totalrectsize / AtlasSize;

                    // fill relevant data in the rects array
                    // once the textures have been packed

                    for (int i = 0; i < RectsCount; i++)
                    {
                        if (StbRects[i].was_packed)
                        {
                            Rects[i]->WasPacked = true;
                            Rects[i]->Px = StbRects[i].x;
                            Rects[i]->Py = StbRects[i].y;
                            //                   std::cout << "Rect : " << StbRects[i].id << " { " << Rects[i]->GetFileName() << "  , x = " << StbRects[i].x << " y = " << StbRects[i].y << " } " << std::endl;
                        }
                        else
                        {
                            Rects[i]->WasPacked = false;
                            Rects[i]->Px = -1;
                            Rects[i]->Py = -1;
                            //               std::cout << "Rect " << StbRects[i].id << " , " << Rects[i]->GetFileName() << " not packed" << std::endl;
                        }
                    }

                    // copy each single texture into the main texture

                    for (size_t k = 0; k < RectsCount; ++k)
                    {
                        if (StbRects[k].was_packed)
                        {

                            RectPackNode* rpn  = Rects[k];
                            int            w   = rpn->Width;
                            int            h   = rpn->Height;
                            int            bpp = rpn->BytesPerPixel;
                            unsigned char* ptr = rpn->Data;

                            size_t i = Rects[k]->Px;
                            size_t j = Rects[k]->Py;

                            for (size_t v = 0; v < h; ++v)
                            {
                                for (size_t u = 0; u < w; ++u)
                                {

                                    size_t sourceoffset = (u + v * w) * bpp;
                                    size_t destoffset = (i + j * AtlasWidth) * BytesPerPixel;

                                    Data[destoffset] = ptr[sourceoffset];
                                    Data[destoffset + 1] = ptr[sourceoffset + 1];
                                    Data[destoffset + 2] = ptr[sourceoffset + 2];
                                    Data[destoffset + 3] = ptr[sourceoffset + 3];

                                    i++;
                                }

                                i = Rects[k]->Px;

                                j++;
                            }
                        }


                    }

                    // return success

                    return TEXTURE_ATLAS_SUCCESS;

                }

                // ------------------------------------------------------
                // save texture atals 

                bool Save(const std::string& destpath) const
                {
                    if (!IsInitialized())
                        vml::os::Message::Error("RectPack : RectPack not initted");
                    if (!Data)
                        vml::os::Message::Error("RectPack : Data is null");
                    if (destpath.empty())
                        vml::os::Message::Error("RectPack : path can't be null");

                    // if CHANNEL_NUM is 4, you can use alpha channel in png
                    if (!stbi_write_png(destpath.c_str(), AtlasWidth, AtlasHeight, 4, Data, AtlasWidth * 4))
                        return TEXTURE_ATLAS_SUCCESS;
                    return TEXTURE_ATLAS_FILE_ERROR;
                }

                // ------------------------------------------------------
                // ctor / dtor

                RectPack()
                {
                    InternalFlags = 0;
                    RectsCount    = 0;
                    AtlasWidth    = 0;
                    AtlasHeight   = 0;
                    AtlasSize     = 0;
                    BytesPerPixel = 0;
                    Pitch         = 0;
                    Ratio         = 0.0f;
                    Rects         = nullptr;
                    Data          = nullptr;
                    StbRectNodes  = nullptr;
                    StbRects      = nullptr;
                }

                ~RectPack()
                {
                    ReleaseAll();
                }

        };
    }
}
