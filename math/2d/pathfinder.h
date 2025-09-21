#pragma once

//////////////////////////////////////////////////////////////
//	2d navmesh

namespace vml
{
	namespace geo2d
	{

		//////////////////////////////////////////////////////////////
		//	navigation mesh computation

		class PathFinder
		{
			
			private:

                // ------------------------------------------------------------------------
                // Node class , this hold data for pathfinder algorithm

                class Node
                {
                    public:

                        int X, Y;           // cell position
                        int F, G, H;        // path finding parameters

                        // ------------------------------------------------------------------------
                        // ctor / dtor

                        Node(int x, int y, int g = 0, int h = 0) : X(x), Y(y), G(g), H(h)
                        {
                            X = x;
                            Y = y;
                            G = g;
                            H = h;
                            F = g + h;
                        }

                        Node() : X(0), Y(0), F(0), G(0), H(0)
                        {}

                        ~Node()
                        {}

                };

                // ----------------------------------------------------------
                //

                class CompareNode
                {
                    public:

                        bool operator()(Node a, Node b)
                        {
                            return a.F > b.F;
                        }
                };

				// ----------------------------------------------------------
				//
                    
				int			   Width;                      // Width of Namvmesh
				int			   Height;                     // Height of NavmEsh
                float          Dw;                         // Cell width
                float          Dh;                         // Cell height
                float          ZDepth;                     // Z position
				int			   Size;                       // Size of navmesh matrix
				std::string    FileName;                   // File name
                unsigned char *Data;                       // Data made of 0s and 1s as a monochrome bitmap
                int           *IdData;                     // Connected componennts bitmap
                int           *Distance;                   // Distance matrix usde for pathfinding
                Node          *Parent;                     // Parent matrix used for reconstrucintg path
                Node           Adjacent[8];                // Vector to store adjacent cell grid nodes
                glm::vec3     *Pa;                         // first vertex of mesh quad
                glm::vec3     *Pb;                         // second vertex of mesh quad
                glm::vec3     *Pc;                         // third vertex of mesh quad
                glm::vec3     *Pd;                         // forth vertex of mesh quad
                glm::vec3     *PAvgrIn;
                glm::vec3     *PAvgrOut;
                glm::vec3     *PAvgrDir;
                float         *PAvgrInvSegmentLength;
                float         *PAvgrSegmentLength;
                glm::ivec2    *Path;
                int            PathCount;
                int            PAvgrOutPathCount;
                glm::vec3      BoundingBoxMin;
                glm::vec3      BoundingBoxMax;
                int            AvgNeighbours;
                int            Ai, Aj;
                int            Bi, Bj;
                float          PathLength;
                float          Scale;

                // ------------------------------------------------------------------------------
                //

                void LoadFile2(const float scale)
                {
                    FILE* stream;

                    errno_t err = fopen_s(&stream, FileName.c_str(), "rb");

                    if (err == 0)
                    {
                        
                        // scale factor

                        Scale = scale;
                        
                        // read data in

                        unsigned int maskw;
						unsigned int maskh;
                        float maskdw;
                        float maskdh;

						fread(&maskw, sizeof(unsigned int), 1, stream);
						fread(&maskh, sizeof(unsigned int), 1, stream);

                        fread(&maskdw, sizeof(unsigned int), 1, stream);
                        fread(&maskdh, sizeof(unsigned int), 1, stream);

                        Width  = maskw;
                        Height = maskh;

                        // *scale dw and dhg factors by Scale* //
                        
                        Dw     = maskdw * Scale;
                        Dh     = maskdh * Scale;
                        Size   = Width * Height;

                    //    std::cout << "Mask Width " << Width << std::endl;
                    //    std::cout << "Mask Height " << Height << std::endl;
                    //    std::cout << "Mask dW " << Dw << std::endl;
                    //    std::cout << "Mask dh " << Dh << std::endl;
                    //    std::cout << "Size " << Size << std::endl;
                        
                        // allocate data structures

                        Data                  = new unsigned char[Size];
                        IdData                = new int[Size];
                        Distance              = new int[Size];
                        Parent                = new Node[Size];
                        Path                  = new glm::ivec2[Size];
                        Pa                    = new glm::vec3[Size];
                        Pb                    = new glm::vec3[Size];
                        Pc                    = new glm::vec3[Size];
                        Pd                    = new glm::vec3[Size];
                        PAvgrIn               = new glm::vec3[Size];
                        PAvgrOut              = new glm::vec3[Size];
                        PAvgrDir              = new glm::vec3[Size];
                        PAvgrInvSegmentLength = new float[Size];
                        PAvgrSegmentLength    = new float[Size];
                        
                        // init data for path finding

                        for (size_t i = 0; i < Size; ++i) IdData[i]   = -1;
                        for (size_t i = 0; i < Size; ++i) Distance[i] =  INT_MAX;
                        for (size_t i = 0; i < Size; ++i) Parent[i]   =  Node(-1, -1);
                        for (size_t i = 0; i < Size; ++i) Path[i]     =  glm::ivec2(0, 0);

                        // read data

						for (unsigned int j = 0; j < Height; ++j)
						{

							for (unsigned int i = 0; i < Width; ++i)
							{

                                unsigned int di, dj;
                                unsigned int val;
                    
                                glm::vec3 ra(-FLT_MAX, -FLT_MAX, -FLT_MAX);
                                glm::vec3 rb(-FLT_MAX, -FLT_MAX, -FLT_MAX);
                                glm::vec3 rc(-FLT_MAX, -FLT_MAX, -FLT_MAX);
                                glm::vec3 rd(-FLT_MAX, -FLT_MAX, -FLT_MAX);

                                // read i, j cell position

                                fread(&di, sizeof(unsigned int), 1, stream);
                                fread(&dj, sizeof(unsigned int), 1, stream);

                                // read pa,pb,pc,pd cell vertices

								fread(&ra.x, sizeof(float), 1, stream);
								fread(&ra.y, sizeof(float), 1, stream);
								fread(&ra.z, sizeof(float), 1, stream);

                                fread(&rb.x, sizeof(float), 1, stream);
                                fread(&rb.y, sizeof(float), 1, stream);
                                fread(&rb.z, sizeof(float), 1, stream);

                                fread(&rc.x, sizeof(float), 1, stream);
                                fread(&rc.y, sizeof(float), 1, stream);
                                fread(&rc.z, sizeof(float), 1, stream);

                                fread(&rd.x, sizeof(float), 1, stream);
                                fread(&rd.y, sizeof(float), 1, stream);
                                fread(&rd.z, sizeof(float), 1, stream);

                                // read cell value '1' for full, '0' for empty space

                                fread(&val, sizeof(unsigned int), 1, stream);

                                unsigned int offset = di + Width * dj;

                                Pa[offset].x = ra.x   * Scale;
                                Pa[offset].y = ra.y   * Scale;
                                Pa[offset].z = ZDepth * Scale;

                                Pb[offset].x = rb.x   * Scale;
                                Pb[offset].y = rb.y   * Scale;
                                Pb[offset].z = ZDepth * Scale;

                                Pc[offset].x = rc.x   * Scale;
                                Pc[offset].y = rc.y   * Scale;
                                Pc[offset].z = ZDepth * Scale;

                                Pd[offset].x = rd.x   * Scale;
                                Pd[offset].y = rd.y   * Scale;
                                Pd[offset].z = ZDepth * Scale;

                                Data[offset] = val;

							}

						}

                        // close stream
                        
                        if (fclose(stream) != 0)
                            vml::os::Message::Error("PathFinder : ", "Cannot close ' ", FileName.c_str(), " '");

                        // compute connected components
                        // the idData bitmap array will contain
                        // the ids of each connected component

                        CreateConnectedComponentsBitmap();

                        // compute bounding box
                       
                        BoundingBoxMin = Pa[0];
                        
                        int offset = Width - 1 + (Height - 1) * Width;
                        
                        if (offset < Size) {
                            BoundingBoxMax = Pd[offset];
                        }
                        else
                        {
                            vml::os::Message::Error("PathFinder : ", "Offset error ' ", FileName.c_str(), " : offset is ", offset , " Size is ", Size);
                        }

                        // dump navmask

                     //  	for (int j = 0; j < Height; ++j)
                     // 	{
                     //  		for (int i = 0; i < Width; ++i)
                     //  		{
                     //  			if (Data[i + j * Width] == 1)
                     //  			{
                     //  				std::cout << ("#");
                     //  			}
                     //  			else
                     //  			{
                     //  				std::cout << (".");
                     //  			}
                     //  		}
                     //  			std::cout << std::endl;
                     //  	}

                    }
                    else
                    {
                        vml::os::Message::Error("PathFinder : ", "Cannot load mesh ' ", FileName.c_str(), " '", " ( error code : ", err, " )");
                    }

                }

                // ------------------------------------------------------------------------------
                //

                void GetAdjacentCells(const Node& point)
                {
                    Adjacent[0].X = -1; Adjacent[0].Y = -1;
                    Adjacent[1].X = -1; Adjacent[1].Y = -1;
                    Adjacent[2].X = -1; Adjacent[2].Y = -1;
                    Adjacent[3].X = -1; Adjacent[3].Y = -1;
                    Adjacent[4].X = -1; Adjacent[4].Y = -1;
                    Adjacent[5].X = -1; Adjacent[5].Y = -1;
                    Adjacent[6].X = -1; Adjacent[6].Y = -1;
                    Adjacent[7].X = -1; Adjacent[7].Y = -1;

                    int i = point.X;
                    int j = point.Y;

                    long a = i - 1;
                    long b = j - 1;
                    long c = i + 1;
                    long d = j + 1;

                    // top left

                    if (a >= 0 && b >= 0)
                    {
                        if (Data[a + b * Width])
                        {
                            Adjacent[0].X = a;
                            Adjacent[0].Y = b;
                        }
                    }

                    // top

                    if (i >= 0 && b >= 0)
                    {
                        if (Data[i + b * Width])
                        {
                            Adjacent[1].X = i;
                            Adjacent[1].Y = b;
                        }
                    }

                    // top right

                    if (c < Width && b >= 0)
                    {
                        if (Data[c + b * Width])
                        {
                            Adjacent[2].X = c;
                            Adjacent[2].Y = b;
                        }
                    }

                    // left

                    if (a >= 0)
                    {
                        if (Data[a + j * Width])
                        {
                            Adjacent[3].X = a;
                            Adjacent[3].Y = j;
                        }
                    }

                    // right

                    if (c < Width)
                    {
                        if (Data[c + j * Width])
                        {
                            Adjacent[4].X = c;
                            Adjacent[4].Y = j;
                        }
                    }

                    // bottom left

                    if (a >= 0 && d < Height)
                    {
                        if (Data[a + d * Width])
                        {
                            Adjacent[5].X = a;
                            Adjacent[5].Y = d;
                        }
                    }

                    // bottom

                    if (d < Height)
                    {
                        if (Data[i + d * Width])
                        {
                            Adjacent[6].X = i;
                            Adjacent[6].Y = d;
                        }
                    }

                    // bottom right

                    if (c < Width && d < Height)
                    {
                        if (Data[c + d * Width])
                        {
                            Adjacent[7].X = c;
                            Adjacent[7].Y = d;
                        }
                    }

                }

                // ------------------------------------------------------------------------------
                //

                [[nodiscard]] int GetAddress(const int i, const int j) const
                {
                    return i + j * Width;
                }

                // ------------------------------------------------------------------------------
                //

                [[nodiscard]] int GetAddressFromPoint(const Node& p) const
                {
                    return p.X + p.Y * Width;
                }

                // ------------------------------------------------------------------------------
                //

                [[nodiscard]] bool FindNextFreeCell(const unsigned char* visited, Node& point)
                {
                    for (int j = 0; j < Height; ++j)
                    {
                        for (int i = 0; i < Width; ++i)
                        {
                            int offset = GetAddress(i, j);

                            if (Data[offset] && !visited[offset])
                            {
                                point = Node(i, j);

                                return true;
                            }
                        }
                    }
                    return false;
                }

                // ------------------------------------------------------------------------------
                //

                void CreateConnectedComponentsBitmap()
                {
                    Node* stack;
                    unsigned char* visited;
                    int            idcounter;
                    bool           found;

                    stack = new Node[Size];
                    visited = new unsigned char[Size];

                    for (size_t i = 0; i < Size; ++i)
                        visited[i] = 0;

                    for (size_t i = 0; i < Size; ++i)
                        stack[i] = Node(-1, -1);

                    idcounter = 0;
                                        
                    do
                    {
                        Node point = Node(-1, -1);

                        found = FindNextFreeCell(visited, point);

                        if (found)
                        {
              //              std::cout << point.X << " " << point.Y << std::endl;

                            int stackcounter = 1;

                            stack[0] = point;

                            for (int sc = 0; sc < stackcounter; ++sc)
                            {
                                int offset = GetAddressFromPoint(stack[sc]);

                                IdData[offset] = idcounter;

                                visited[offset] = 1;

                                GetAdjacentCells(stack[sc]);

                                for (int i = 0; i < 8; ++i)
                                {

                                    if (Adjacent[i].X != -1 && Adjacent[i].Y != -1)
                                    {
                                        int offset = GetAddressFromPoint(Adjacent[i]);

                                        if (Data[offset] && !visited[offset])
                                        {
                                            visited[offset] = 1;

                                            IdData[offset] = idcounter;

                                            stack[stackcounter] = Adjacent[i];

                                            stackcounter++;
                                        }
                                    }
                                }
                            }

                            idcounter++;

                        }
 
                    } while (found);
                   
                    if ( idcounter>1)
                        std::cout << "PathFinder : WARNING : Connected Componets : " << idcounter << std::endl;
                    else
                        std::cout << "PathFinder : Connected Componets : " << idcounter << std::endl;

                    delete[] stack;
                    delete[] visited;
                    
                }

                // ------------------------------------------------------------------------------
                //

                [[nodiscard]] int heuristic(const Node& start, const Node& dest)
                {
                    int dx = start.X - dest.X;
                    int dy = start.Y - dest.Y;
                    return dx * dx + dy * dy;
                }

                // ------------------------------------------------------------------------------
                //

                [[nodiscard]] int heuristic2(const Node& start, const Node& dest)
                {
                    return abs(start.X - dest.X) + abs(start.Y - dest.Y);
                }

		public:

                // ----------------------------------------------------------
                //

                class DebugCell
                {
                    public:

                        glm::vec3 Pa;          // first vertex of mesh quad
                        glm::vec3 Pb;          // second vertex of mesh quad
                        glm::vec3 Pc;          // third vertex of mesh quad
                        glm::vec3 Pd;          // forth vertex of mesh quad
                        int       IdData;      // Connected componennts bitmap

                        // ------------------------------------------------------------------------
                        // ctor / dtor

                        DebugCell()
                        {
                            Pa = glm::vec3(0, 0, 0);
                            Pb = glm::vec3(0, 0, 0);
                            Pc = glm::vec3(0, 0, 0);
                            Pd = glm::vec3(0, 0, 0);
                            IdData = -1;
                        }

                        ~DebugCell()
                        {}

                };

                // ---------------------------------------------------------------
                //

                [[nodiscard]] int   GetSize()                         const { return Size; }
                [[nodiscard]] int   GetWidth()                        const { return Width; }
                [[nodiscard]] int   GetHeight()                       const { return Height; }
                [[nodiscard]] float GetDw()                           const { return Dw; }
                [[nodiscard]] float GetDh()                           const { return Dh; }
                [[nodiscard]] const glm::vec3& GetBoundingBoxMin()    const { return BoundingBoxMin; }
                [[nodiscard]] const glm::vec3& GetBoundingBoxMax()    const { return BoundingBoxMax; }
                [[nodiscard]] int   GetPathCount()                    const { return PAvgrOutPathCount; }
                [[nodiscard]] const glm::vec3* GetPAvgrOut()          const { return PAvgrOut; }
                [[nodiscard]] const glm::vec3* GetPAvgrDir()          const { return PAvgrDir; }
                [[nodiscard]] const float *GetPAvgrInvSegmentLength() const { return PAvgrInvSegmentLength; }
                [[nodiscard]] const float* GetPAvgrSegmentLength()    const { return PAvgrSegmentLength; }
                [[nodiscard]] float GetZDepth()                       const { return ZDepth; }

                // -------------------------------------------------------------
                //

                [[nodiscard]] bool IsPointInNavMesh(const glm::vec3& ra, const glm::vec3& rb) const
                {
                    
                    glm::vec3 p;
                    float t;

                    glm::vec3 bmin = BoundingBoxMin;
                    glm::vec3 bmax = BoundingBoxMax;

                    if (!vml::geo3d::intersections::TriangleVsRay(glm::vec3(bmin.x, bmin.y, bmin.z),
                                                                  glm::vec3(bmin.x, bmax.y, bmin.z),
                                                                  glm::vec3(bmax.x, bmax.y, bmin.z),
                                                                  ra, rb, p, t))
                    {
                        if (!vml::geo3d::intersections::TriangleVsRay(glm::vec3(bmin.x, bmin.y, bmin.z),
                                                                      glm::vec3(bmax.x, bmax.y, bmin.z),
                                                                      glm::vec3(bmax.x, bmin.y, bmin.z),
                                                                      ra, rb, p, t))
                        {
                            return false;
                        }
                    }
                    
                    return true;
                }

                // ---------------------------------------------------------------
                //

                bool IsPointInQuad(const glm::vec3& p) const
                {
                    glm::vec3 p0= glm::vec3(BoundingBoxMin.x, BoundingBoxMin.y, BoundingBoxMin.z);
                    float ux = BoundingBoxMax.x- BoundingBoxMin.x;
                    float vy = BoundingBoxMax.y- BoundingBoxMin.y;
                    float a = (p.x - p0.x) * ux / (ux * ux);
                    float b = (p.y - p0.y) * vy / (vy * vy);
                    return (a > -vml::math::EPSILON && a < 1.0f + vml::math::EPSILON) && (b > -vml::math::EPSILON && b < 1.0f + vml::math::EPSILON);
                }

                // ---------------------------------------------------------------
                //

                [[nodiscard]] bool GetCellCenterFromPoint(const glm::vec3& p, glm::vec3 &r) const
                {
                    int i = (int)((p.x - BoundingBoxMin.x) / Dw);
                    int j = (int)((p.y - BoundingBoxMin.y) / Dh);
                    if(!IsPointInQuad(p))
                    if (i < 0 || j < 0 || i >= Width || j >= Height)
                        return false;
                    int offset = i + j * Width;
                    r=( Pa[offset] + Pd[offset] )*0.5f;
                    return true;
                }

                // ---------------------------------------------------------------
                //

                [[nodiscard]] int GetCellAddressFromPoint(const glm::vec3& p) const
                {
                    int i = (int)((p.x - BoundingBoxMin.x) / Dw);
                    int j = (int)((p.y - BoundingBoxMin.y) / Dh);
                    if (i < 0 || j < 0 || i >= Width || j >= Height)
                        vml::os::Message::Error("Pathfinder : ", "index out of range");
                    return i + j * Width;
                }

                // ---------------------------------------------------------------
                //

                [[nodiscard]] bool GetCellIndicesFromPoint(const glm::vec3& p,glm::ivec2 &ci) const
                {
                    if (!IsPointInQuad(p))
                        return false;
                    int i = (int)((p.x - BoundingBoxMin.x) / Dw);
                    int j = (int)((p.y - BoundingBoxMin.y) / Dh);
                    ci=glm::ivec2 (i,j);
                    return true;
                }

                // ---------------------------------------------------------------
                //

                [[nodiscard]] int GetCellIdFromIndices(const glm::ivec2 &uv) const
                {
                    int i = uv.x;
                    int j = uv.y;
                    if (i < 0 || j < 0 || i >= Width || j >= Height)
                        vml::os::Message::Error("Pathfinder : ","index out of range");
                    return IdData[i + j * Width];
                }

                // ---------------------------------------------------------------
                //

                [[nodiscard]] int GetCellAddressFromIndices(const glm::ivec2& uv) const
                {
                    int i = uv.x;
                    int j = uv.y;
                    if (i < 0 || j < 0 || i >= Width || j >= Height)
                        vml::os::Message::Error("Pathfinder : ", "index out of range");
                    return i + j * Width;
                }

                // ---------------------------------------------------------------
                //

                [[nodiscard]] glm::vec3 GetCellCenterFromIndices(const glm::ivec2& uv) const
                {
                    int i = uv.x;
                    int j = uv.y;
                    if (i < 0 || j < 0 || i >= Width || j >= Height)
                        vml::os::Message::Error("Pathfinder : ", "index out of range");
                    int offset = i + j * Width;
                    return (Pa[offset] + Pd[offset]) * 0.5f;
                }

                // ---------------------------------------------------------------
                //

                [[nodiscard]] float GetPathLength() const
                {
                    return PathLength;
                }

                // ---------------------------------------------------------------
                //

                void GetCellVertices(const glm::ivec2& uv, glm::vec3& p0, glm::vec3& p1, glm::vec3& p2, glm::vec3& p3)
                {
                    int i = uv.x;
                    int j = uv.y;
                    if (i < 0) vml::os::Message::Error("PathFinder : ", "x component is less than 0");
                    if (j < 0) vml::os::Message::Error("PathFinder : ", "y component is less than 0");
                    if (i >= Width) vml::os::Message::Error("PathFinder : ", "x component is greater than Width ( ", Width, " )");
                    if (j >= Height) vml::os::Message::Error("PathFinder : ", "y component is greater than Width ( ", Height, " )");

                    int offset = i + j * Width;

                    p0 = Pa[offset];
                    p1 = Pb[offset];
                    p2 = Pc[offset];
                    p3 = Pd[offset];

                }

                // ------------------------------------------------------------------------------
                //
                
                void SetZDepth(const float zdepth)
                {
                    ZDepth = zdepth;
                }

                // ------------------------------------------------------------------------------
                //

                [[nodiscard]] int FindPath(int ai, int aj, int bi, int bj,const glm::vec3 &origin=glm::vec3(0,0,0))
                {
                                       
                    // points are coinciding , there is no path

                    PathCount         =  0;
                    PAvgrOutPathCount =  0;
                    Ai                = ai;
                    Aj                = aj;
                    Bi                = bi;
                    Bj                = bj;

                    if (ai < 0) vml::os::Message::Error("PathFinder : ", "ai component is less than 0");
                    if (aj < 0) vml::os::Message::Error("PathFinder : ", "aj component is less than 0");
                    if (ai >= Width) vml::os::Message::Error("PathFinder : ", "ai component is greater than Width ( ", Width," )");
                    if (aj >= Height) vml::os::Message::Error("PathFinder : ", "aj component is greater than Width ( ", Height," )");

                    if (bi < 0) vml::os::Message::Error("PathFinder : ", "bi component is less than 0");
                    if (bj < 0) vml::os::Message::Error("PathFinder : ", "bj component is less than 0");
                    if (bi >= Width) vml::os::Message::Error("PathFinder : ", "bi component is greater than Width ( ", Width," )");
                    if (bj >= Height) vml::os::Message::Error("PathFinder : ", "bj component is greater than Width ( ", Height," )");

                    // path starts and ends in the same cell

                    if (ai == bi && aj == bj)
                        return 0;

                    int offseta = ai + aj * Width;
                    int offsetb = bi + bj * Width;

                //    if (IdData[offseta] != -1 && IdData[offsetb] != -1)
                //    {
                //       DrawCell(view, ai, aj, vml::colors::Green);
                //       DrawCell(view, bi, bj, vml::colors::Yellow);
                //   }
                //    else
                //    {
                //      DrawCell(view, ai, aj, vml::colors::Red);
                //       DrawCell(view, bi, bj, vml::colors::Red);
                //    }

                    if (IdData[offseta] == -1 || IdData[offsetb] == -1)
                        return 0;

                    // source and target are on the same
                    // separated sub - bitmap

                    if (IdData[offseta] == IdData[offsetb])
                    {
                        Ai = ai;
                        Aj = aj;
                        Bi = bi;
                        Bj = bj;

                        // auto timerstart = std::chrono::steady_clock::now();

                        std::priority_queue<Node, std::vector<Node>, CompareNode> Queue;

                        // clear maps

                        for (size_t i = 0; i < Size; ++i) Distance[i] = INT_MAX;
                        for (size_t i = 0; i < Size; ++i) Parent[i]   = Node(-1, -1);
                        
                        Node start;
                        Node dest;
                        Node adj;
                        int aoffset;
                        int uoffset;

                        start.X = ai;
                        start.Y = aj;
                        start.G = 0;
                        start.H = heuristic(start, dest);
                        start.F = start.G + start.H;

                        dest.X = bi;
                        dest.Y = bj;
                        dest.G = 0;
                        dest.H = 0;
                        dest.F = 0;

                        // set initial distance to zero

                        Distance[start.X + start.Y * Width] = 0;

                        // init path finding

                        Queue.push(start);
                        
                        while (!Queue.empty())
                        {
                            // pop current node

                            Node u = Queue.top();

                            Queue.pop();

                            // current cell address

                            uoffset = u.X + u.Y * Width;

                            // compute adjacent cells

                            int i = u.X;
                            int j = u.Y;

                            long a = i - 1;
                            long b = j - 1;
                            long c = i + 1;
                            long d = j + 1;

                            // topleft cell

                            if (a >= 0 && b >= 0)
                            {
                                aoffset = a + b * Width;

                                if (Data[aoffset])
                                {

                                    adj.X = a;
                                    adj.Y = b;
                                    adj.G = Distance[uoffset];
                                    adj.H = heuristic(adj, dest);
                                    adj.F = adj.G + adj.H;

                                    if (Distance[aoffset] == INT_MAX)
                                    {
                                        // 2 and 1 are squared of 1.41 and 1 for distance computation

                                        Queue.push(adj);
                                        Distance[aoffset] = Distance[uoffset] + 2;
                                        Parent[aoffset] = u;

                                    }
                                }
                            }

                            // top cell

                            if (i >= 0 && b >= 0)
                            {
                                aoffset = i + b * Width;

                                if (Data[aoffset])
                                {

                                    adj.X = i;
                                    adj.Y = b;
                                    adj.G = Distance[uoffset];
                                    adj.H = heuristic(adj, dest);
                                    adj.F = adj.G + adj.H;

                                    if (Distance[aoffset] == INT_MAX)
                                    {
                                        // 2 and 1 are squared of 1.41 and 1 for distance computation

                                        Queue.push(adj);
                                        Distance[aoffset] = Distance[uoffset] + 1;
                                        Parent[aoffset] = u;

                                    }
                                }
                            }

                            // top right cell

                            if (c < Width && b >= 0)
                            {
                                aoffset = c + b * Width;

                                if (Data[c + b * Width])
                                {

                                    adj.X = c;
                                    adj.Y = b;
                                    adj.G = Distance[uoffset];
                                    adj.H = heuristic(adj, dest);
                                    adj.F = adj.G + adj.H;

                                    if (Distance[aoffset] == INT_MAX)
                                    {
                                        // 2 and 1 are squared of 1.41 and 1 for distance computation

                                        Queue.push(adj);
                                        Distance[aoffset] = Distance[uoffset] + 2;
                                        Parent[aoffset] = u;

                                    }
                                }
                            }

                            // left cell

                            if (a >= 0)
                            {
                                aoffset = a + j * Width;

                                if (Data[aoffset])
                                {

                                    adj.X = a;
                                    adj.Y = j;
                                    adj.G = Distance[uoffset];
                                    adj.H = heuristic(adj, dest);
                                    adj.F = adj.G + adj.H;

                                    if (Distance[aoffset] == INT_MAX)
                                    {
                                        // 2 and 1 are squared of 1.41 and 1 for distance computation

                                        Queue.push(adj);
                                        Distance[aoffset] = Distance[uoffset] + 1;
                                        Parent[aoffset] = u;

                                    }
                                }
                            }

                            // right cell

                            if (c < Width)
                            {
                                aoffset = c + j * Width;

                                if (Data[aoffset])
                                {

                                    adj.X = c;
                                    adj.Y = j;
                                    adj.G = Distance[uoffset];
                                    adj.H = heuristic(adj, dest);
                                    adj.F = adj.G + adj.H;

                                    if (Distance[aoffset] == INT_MAX)
                                    {
                                        // 2 and 1 are squared of 1.41 and 1 for distance computation

                                        Queue.push(adj);
                                        Distance[aoffset] = Distance[uoffset] + 1;
                                        Parent[aoffset] = u;

                                    }
                                }
                            }

                            // bottom left cell

                            if (a >= 0 && d < Height)
                            {
                                aoffset = a + d * Width;

                                if (Data[aoffset])
                                {

                                    adj.X = a;
                                    adj.Y = d;
                                    adj.G = Distance[uoffset];
                                    adj.H = heuristic(adj, dest);
                                    adj.F = adj.G + adj.H;

                                    if (Distance[aoffset] == INT_MAX)
                                    {
                                        // 2 and 1 are squared of 1.41 and 1 for distance computation

                                        Queue.push(adj);
                                        Distance[aoffset] = Distance[uoffset] + 2;
                                        Parent[aoffset] = u;

                                    }
                                }
                            }

                            // bottom cell

                            if (d < Height)
                            {
                                aoffset = i + d * Width;

                                if (Data[aoffset])
                                {

                                    adj.X = i;
                                    adj.Y = d;
                                    adj.G = Distance[uoffset];
                                    adj.H = heuristic(adj, dest);
                                    adj.F = adj.G + adj.H;

                                    if (Distance[aoffset] == INT_MAX)
                                    {
                                        // 2 and 1 are squared of 1.41 and 1 for distance computation

                                        Queue.push(adj);
                                        Distance[aoffset] = Distance[uoffset] + 1;
                                        Parent[aoffset] = u;

                                    }
                                }
                            }

                            // bottom right cell

                            if (c < Width && d < Height)
                            {
                                aoffset = c + d * Width;

                                if (Data[aoffset])
                                {

                                    adj.X = c;
                                    adj.Y = d;
                                    adj.G = Distance[uoffset];
                                    adj.H = heuristic(adj, dest);
                                    adj.F = adj.G + adj.H;

                                    if (Distance[aoffset] == INT_MAX)
                                    {
                                        // 2 and 1 are squared of 1.41 and 1 for distance computation

                                        Queue.push(adj);
                                        Distance[aoffset] = Distance[uoffset] + 2;
                                        Parent[aoffset] = u;

                                    }
                                }
                            }
                        }
                        
                        // store path into path array

                        Node point = dest;

                        bool found = false;

                        Path[0].x = point.X;
                        Path[0].y = point.Y;

                        PathCount = 1;

                        while (!found)
                        {

                            if (point.X == start.X && point.Y == start.Y)
                            {
                                found = true;
                            }
                            else
                            {

                                point = Parent[dest.X + dest.Y * Width];

                                Path[PathCount].x = point.X;
                                Path[PathCount].y = point.Y;

                                PathCount++;

                                dest = point;

                            }

                        }

                        // compute average path for smoothness

                        int offset;
                        glm::vec3 p(0, 0, 0);
                        glm::vec3 q(0, 0, 0);

                        // compute centre cell points

                        for (int i = 0; i < PathCount; ++i)
                        {
                            offset = Path[i].x + Path[i].y * Width;
                        
                            PAvgrIn[i] = (Pa[offset] + Pd[offset]) * 0.5f;
                        }

                        //

                        float denum = 1.0f / ( (float)AvgNeighbours * 2.0f + 1.0f );

                        // add initial point to the curve path array  
                        
                       PAvgrOut[0] = origin;
                       PAvgrOutPathCount = 1;

                        // compute average curve path array

                        for (int i = 1; i < PathCount; i++)
                        {
                            p.x = 0;
                            p.y = 0;
                            p.z = 0;

                            for (int j = -AvgNeighbours; j <= AvgNeighbours; j++)
                            {
                                p += PAvgrIn[ (i + j <= 0) || (i + j >= PathCount) ? i : i + j ];
                            }
                            
                            PAvgrOut[PAvgrOutPathCount++] = p * denum;
                        }
                        
                        // add last point to the curve path array

                        PAvgrOut[PAvgrOutPathCount++] = PAvgrIn[PathCount-1];

                        // compute path lenght

                        PathLength = 0.0f;
                        
                        for (int i = 0; i < PAvgrOutPathCount - 1; ++i)
                        {
                            PAvgrDir[i]= PAvgrOut[i + 1] - PAvgrOut[i];
                            float denum = sqrtf(PAvgrDir[i].x * PAvgrDir[i].x + PAvgrDir[i].y * PAvgrDir[i].y);
                            PAvgrInvSegmentLength[i] = 1.0f / denum;
                            PAvgrSegmentLength[i] = denum;
                            PathLength += denum;
                        }

                        return 1;

                    }   // if sorce and dest are inside the same connected component
                    
                    return 0;
                }

                // ------------------------------------------------------------------------------
                // Expands a rectagnel from the given cell until it finds a non void cell
                // the rectangel dimension obtained contains the nearest cell(s) , and the
                // the nearest cell returned
                // id is the belonign id of the bitmap island

                [[nodiscard]] glm::ivec2 FindNearestCellToAnotherCell(int ai, int aj, int id)
                {
                    
                    // finds maximum recangel dimensions

                    int maxdim = (Width > Height) ? Width : Height;
                    
                    bool found = false;
                    int mindim = -1;

                    for ( int s=1; s<maxdim && !found; ++s)
                    {
                        // upper border

                        if (aj - s >= 0)
                        {
                            int i0 = ai - s;
                            int i1 = ai + s;
                            int j0 = aj - s;

                            if (i0 < 0) i0 = 0;
                            if (i1 > Width - 1) i1 = Width - 1;

                            for (int i = i0; i <= i1; ++i)
                            {

                                int offset = i + j0 * Width;
                               
                                if (IdData[offset] == id)
                                {
                             //       DrawCell(view, i, j0, vml::colors::Green);

                                    found = true;
                                    mindim = s;
                                }
                            //    else
                            //    {
                            //        DrawCell(view, i, j0, vml::colors::Red);
                            //    }
                            }
                        }

                        // right border

                        if (ai - s >= 0)
                        {
                            int i0 = ai - s;
                            int j0 = aj - s + 1;
                            int j1 = aj + s - 1;

                            if (j0 < 0) j0 = 0;
                            if (j1 > Height - 1) j1 = Height - 1;

                            for (int j = j0; j <= j1; ++j)
                            {

                                int offset = i0 + j * Width;

                                if (IdData[offset] == id)
                                {
                            //        DrawCell(view, i0, j, vml::colors::Green);

                                    found = true;
                                    mindim = s;
                                }
                          //      else
                          //      {
                          //          DrawCell(view, i0, j, vml::colors::Red);
                          //      }

                            }
                        }

                        // bottom border

                        if (aj + s < Height)
                        {
                            int i0 = ai - s;
                            int j0 = aj + s;
                            int i1 = ai + s;

                            if (i0 < 0) i0 = 0;
                            if (i1 > Width - 1) i1 = Width - 1;

                            for (int i = i0; i <= i1; ++i)
                            {

                                int offset = i + j0 * Width;

                                if (IdData[offset] == id)
                                {
                              //      DrawCell(view, i, j0, vml::colors::Green);

                                    found = true;
                                    mindim = s;
                                }
                              //  else
                              //  {
                              //      DrawCell(view, i, j0, vml::colors::Red);
                              //  }
                            }
                        }

                        // left borderd

                        if (ai + s < Width)
                        {
                            int i0 = ai + s;
                            int j0 = aj - s + 1;
                            int j1 = aj + s - 1;

                            if (j0 < 0) j0 = 0;
                            if (j1 > Height - 1) j1 = Height - 1;

                            for (int j = j0; j <= j1; ++j)
                            {

                                int offset = i0 + j * Width;

                                if (IdData[offset] == id)
                                {
                               //     DrawCell(view, i0, j, vml::colors::Green);

                                    found = true;
                                    mindim = s;
                                }
                             //   else
                             //   {
                             //       DrawCell(view, i0, j, vml::colors::Red);
                             //   }
                            }
                        }
                    }

                    // when a rectagnel contaiing at least one non void cell has been found
                    // find the neraeast cells to the given cell

                    glm::ivec2 cell(-1, -1);

                    if (mindim != -1)
                    {
                        int dist = INT_MAX;

                        for (int j = aj - mindim; j <= aj + mindim; ++j)
                        {
                            if (j >= 0 && j < Height)
                            {
                                for (int i = ai - mindim; i <= ai + mindim; ++i)
                                {
                                    if (i >= 0 && i < Width)
                                    {
                                        int offset = i + j * Width;

                                        if (IdData[offset] == id)
                                        {
                                            int dx = ai - i;
                                            int dy = aj - j;
                                            int cdist = dx * dx + dy * dy;
                                            if (cdist < dist) { dist = cdist; cell.x = i; cell.y = j; }
                                            
                                   //         DrawCell2(view, i, j, vml::colors::Green,1);
                                        }
                                        else
                                        {
                                    //        DrawCell2(view, i, j, vml::colors::Red,1);
                                        }
                                    }
                                }
                            }
                        }
                    }

                    //   std::cout << cellidi << " " << cellidj << std::endl;
                    /*
                    if (cell.x != -1 && cell.y != -1)
                    {
                        int offset;
                        glm::vec3 a,b;
                        offset = ai + aj * Width;
                        a= ( Pa[offset] + Pd[offset]) * 0.5f;
                        offset = cell.x + cell.y * Width;
                        b = (Pa[offset] + Pd[offset]) * 0.5f;
                        vml::DebugRender->DrawLine(view, a, b, vml::colors::White,false);
                    }
                    */

                    return cell;
                }

                // ---------------------------------------------------------------
                //

                [[nodiscard]] DebugCell GetCellAt(const int x, const int y) const
                {
                    if (x < 0) vml::os::Message::Error("PathFinder : ", "x component is less than 0");
                    if (y < 0) vml::os::Message::Error("PathFinder : ", "y component is less than 0");
                    if (x >= Width) vml::os::Message::Error("PathFinder : ", "x component is greater than Width ", Width);
                    if (y >= Height) vml::os::Message::Error("PathFinder : ", "y component is greater than Width ", Height);

                    DebugCell c;
                    int offset = x + y * Width;
                    c.Pa = Pa[offset];
                    c.Pb = Pb[offset];
                    c.Pc = Pc[offset];
                    c.Pd = Pd[offset];
                    c.IdData = IdData[offset];
                    return c;
                }

                // ---------------------------------------------------------------
                //

                [[nodiscard]] DebugCell GetCellFromPoint(const glm::vec3& p) const
                {
                    const int i = (int)((p.x - BoundingBoxMin.x) / Dw);
                    const int j = (int)((p.y - BoundingBoxMin.y) / Dh);
                    if (i < 0 || j < 0 || i >= Width || j >= Height)
                        vml::os::Message::Error("Pathfinder : ", "index out of range");
                    return GetCellAt(i, j);
                }

				// ---------------------------------------------------------------
				// ctor / dtor

				PathFinder(const std::string& filename,const float scale)
				{
					Width                 =  0;
					Height                =  0;
					Size                  =  0;
					Data                  =  nullptr;
                    Data                  =  nullptr;
                    IdData                =  nullptr;
                    Distance              =  nullptr;
                    Parent                =  nullptr;
                    Path                  =  nullptr;
                    Pa                    =  nullptr;
                    Pb                    =  nullptr;
                    Pc                    =  nullptr;
                    Pd                    =  nullptr;
                    Dw                    =  0.0f;
                    Dh                    =  0.0f;
                    PAvgrIn               =  nullptr;
                    PAvgrOut              =  nullptr;
                    PAvgrDir              =  nullptr;
                    PAvgrInvSegmentLength =  nullptr;
                    PAvgrSegmentLength    =  nullptr;
                    PathCount             =  0;
                    PAvgrOutPathCount     =  0;
                    AvgNeighbours         =  2;
                    BoundingBoxMin        =  glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
                    BoundingBoxMax        =  glm::vec3( FLT_MAX,  FLT_MAX,  FLT_MAX);
					FileName              =  filename;
                    Ai                    = -1;
                    Aj                    = -1;
                    Bi                    = -1;
                    Bj                    = -1;
                    ZDepth                =  0.01f;
                    PathLength            =  0.0f;
                    Scale                 =  1.0f;

                    // load navmesh file

					LoadFile2(scale);
					
				}

				~PathFinder()
				{
					vml::os::SafeDelete(Data);
                    vml::os::SafeDelete(IdData);
                    vml::os::SafeDelete(Distance);
                    vml::os::SafeDeleteArray(Parent);
                    vml::os::SafeDeleteArray(Path);
                    vml::os::SafeDeleteArray(Pa);
                    vml::os::SafeDeleteArray(Pb);
                    vml::os::SafeDeleteArray(Pc);
                    vml::os::SafeDeleteArray(Pd);
                    vml::os::SafeDeleteArray(PAvgrIn);
                    vml::os::SafeDeleteArray(PAvgrOut);
                    vml::os::SafeDeleteArray(PAvgrDir);
                    vml::os::SafeDeleteArray(PAvgrInvSegmentLength);
                    vml::os::SafeDeleteArray(PAvgrSegmentLength);
                }

		};
	}
}
