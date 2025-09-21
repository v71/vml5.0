#pragma once

namespace vml
{
	namespace meshes
	{
		////////////////////////////////////////////////////////////////////////////
		//

		class RemoveDuplicates3D
		{
			private:

				// ----------------------------------------------------------------------
				// 

				class HashedVertex
				{
					private:

						int Id;

					public:

						// ---------------------------------------------------------------

						int GetId() const { return Id; }

						// ---------------------------------------------------------------

						vml::geo3d::Vertex Vertex;

						// ---------------------------------------------------------------
						// ctor / dtor

						HashedVertex(const vml::geo3d::Vertex& v, const int id)
						{
							Vertex = v;
							Id = id;
						}

						HashedVertex()
						{
							Id = -1;
						}

						~HashedVertex()
						{}
				};

				// ----------------------------------------------------------------------
				// Gridcell class 
				// this class contains the vertices enclosed in the cell's bounding box

				class GridCell
				{
					private:

					public:

						// ---------------------------------------------------------------
						// 

						std::vector<HashedVertex> VertexArray;

						// ---------------------------------------------------------------
						// vertex comparison predicate 

						int FindVertex(const vml::geo3d::Vertex& vertex, const float eps = 0.000000000001f) const
						{
							for (size_t i = 0; i < VertexArray.size(); ++i)
							{
								if (fabsf(VertexArray[i].Vertex.Pos.x    - vertex.Pos.x)    < eps &&
									fabsf(VertexArray[i].Vertex.Pos.y    - vertex.Pos.y)    < eps &&
									fabsf(VertexArray[i].Vertex.Pos.z    - vertex.Pos.z)    < eps &&
									fabsf(VertexArray[i].Vertex.Normal.x - vertex.Normal.x) < eps &&
									fabsf(VertexArray[i].Vertex.Normal.y - vertex.Normal.y) < eps &&
									fabsf(VertexArray[i].Vertex.Normal.z - vertex.Normal.z) < eps &&
									fabsf(VertexArray[i].Vertex.UV.x     - vertex.UV.x)     < eps &&
									fabsf(VertexArray[i].Vertex.UV.y     - vertex.UV.y)     < eps)
								{
									return VertexArray[i].GetId();
								}
							}
							return -1;
						}

						// ---------------------------------------------------------------
						// ctor / dtor

						GridCell()
						{}

						~GridCell()
						{}

				};

				// ---------------------------------------------------------------
				// 

				static const unsigned int CELLSIZE = 16;

				// ---------------------------------------------------------------
				// 

				glm::vec3	 BoundingBoxMin;			// bounding box minimum position
				glm::vec3	 BoundingBoxMax;			// bounding box maximum position
				glm::vec3	 BoundingBoxExtents;		// bounding box half extents
				float		 GainRatio;					// gain ratio for node
				bool		 Initted;

				// ---------------------------------------------------------------
				// 

				GridCell* Cell;
				int GridSize;

				// ---------------------------------------------------------------
				// 

				GridCell* GetCellFromVertex(const vml::geo3d::Vertex& p)
				{
					float ix = (p.Pos.x - BoundingBoxMin.x) * (CELLSIZE - 1) / BoundingBoxExtents.x;
					float iy = (p.Pos.y - BoundingBoxMin.y) * (CELLSIZE - 1) / BoundingBoxExtents.y;
					float iz = (p.Pos.z - BoundingBoxMin.z) * (CELLSIZE - 1) / BoundingBoxExtents.z;

					int ii = int(ix);
					int jj = int(iy);
					int kk = int(iz);

					if (ii < 0 || ii >= CELLSIZE) { vml::os::Message::Error("removeduplicates :", "ii overflow ", ii); }
					if (jj < 0 || jj >= CELLSIZE) { vml::os::Message::Error("removeduplicates :", "jj overflow ", jj); }
					if (kk < 0 || kk >= CELLSIZE) { vml::os::Message::Error("removeduplicates :", "kk overflow ", kk); }

					return &Cell[ii + jj * CELLSIZE + kk * CELLSIZE * CELLSIZE];
				}

			public:

				// ---------------------------------------------------------------
				// 

				void Begin(const glm::vec3& bmin, const glm::vec3& bmax, const float& eps = vml::math::EPSILON)
				{
					// clear grid

					for (size_t i = 0; i < GridSize; ++i)
						Cell[i].VertexArray.clear();

					// compute bounding box

					BoundingBoxMin = bmin;
					BoundingBoxMax = bmax;

					if (fabs(BoundingBoxMin.x) < eps) BoundingBoxMin.x = -eps;
					if (fabs(BoundingBoxMin.y) < eps) BoundingBoxMin.y = -eps;
					if (fabs(BoundingBoxMin.z) < eps) BoundingBoxMin.z = -eps;
					if (fabs(BoundingBoxMax.x) < eps) BoundingBoxMax.x = eps;
					if (fabs(BoundingBoxMax.y) < eps) BoundingBoxMax.y = eps;
					if (fabs(BoundingBoxMax.z) < eps) BoundingBoxMax.z = eps;

					BoundingBoxExtents = BoundingBoxMax - BoundingBoxMin;

					// set flag as initted

					Initted = true;

				}

				// ---------------------------------------------------------------
				// 

				void RemoveDuplicates(std::vector<vml::geo3d::Vertex>& srcvertexarray,
									  std::vector<vml::geo3d::IndexedTriangle>& surfacesarray)
				{
					if (!Initted)
						vml::os::Message::Error("RemoveDupliacted : ","Cannot initt function");

					// remove duplicates

					std::vector<int> ReMapping;

					int destvertices = 0;

					int oldverts = (int)srcvertexarray.size();

					ReMapping.resize(oldverts);

					std::vector<vml::geo3d::Vertex> destvertexarray;

					for (size_t i = 0; i < srcvertexarray.size(); ++i)
					{

						GridCell* cell = GetCellFromVertex(srcvertexarray[i]);

						int vertexindex = cell->FindVertex(srcvertexarray[i]);

						//	vml::os::Trace("vertex %d ( %f %f %f , %f %f %f ) mapped to %d\n", i,
						//		vertexarray[i].Pos.x, vertexarray[i].Pos.y, vertexarray[i].Pos.z,
						//		vertexarray[i].Normal.x, vertexarray[i].Normal.y, vertexarray[i].Normal.z,
						//		vertexindex);

						if (vertexindex != -1)
						{

							// Vertex i is a duplicate of one inserted earlier into the
							// table.  Map vertex i to the first-found copy.

							ReMapping[i] = vertexindex;

						}
						else
						{

							// emplace vertex in the current cell

							cell->VertexArray.emplace_back(HashedVertex(srcvertexarray[i], destvertices));

							// copy in dest vertex array

							destvertexarray.emplace_back(srcvertexarray[i]);

							// save index to remap

							ReMapping[i] = destvertices++;

						}

					}

					// copy new array into old one

					srcvertexarray = destvertexarray;

					int newverts = (int)srcvertexarray.size();

					GainRatio = 100.0f * ((float)oldverts - (float)newverts) / (float)oldverts;

				//	vml::os::Message::Trace("Mesh Builder : ","Gain ", GainRatio,"%");
					
					std::cout << "Mesh Builder : Gain : " << GainRatio << std::endl;

					// reindex surface indices

					for (int i = 0; i < surfacesarray.size(); i++)
					{
						//			vml::os::Trace("surface %d ( %d %d %d )\n", i, ReMapping[surfacesrray[i].I0],ReMapping[surfacesrray[i].I1],ReMapping[surfacesrray[i].I2]);

						surfacesarray[i].I0 = ReMapping[surfacesarray[i].I0];
						surfacesarray[i].I1 = ReMapping[surfacesarray[i].I1];
						surfacesarray[i].I2 = ReMapping[surfacesarray[i].I2];
					}

					//

				//	for (size_t i = 0; i < n; ++i)
					{
						//	vml::os::Trace("cell %d\n", i);

				//		for (size_t j = 0; j < Cell[i].VertexArray.size(); ++j)
						{

							//		vml::os::Trace("		vertex %d ( %f %f %f ), (%f %f %f)\n", j,
							//						Cell[i].VertexArray[j].Vertex.Pos.x, Cell[i].VertexArray[j].Vertex.Pos.y, Cell[i].VertexArray[j].Vertex.Pos.z,
							//						Cell[i].VertexArray[j].Vertex.Normal.x, Cell[i].VertexArray[j].Vertex.Normal.y, Cell[i].VertexArray[j].Vertex.Normal.z);

						}
					}

				}

				// ---------------------------------------------------------------

				float GetGainRatio() const { return GainRatio; }

				// ---------------------------------------------------------------
				// ctor / dtor

				RemoveDuplicates3D()
				{
					BoundingBoxMin     = glm::vec3(0, 0, 0);
					BoundingBoxMax     = glm::vec3(0, 0, 0);
					BoundingBoxExtents = glm::vec3(0, 0, 0);
					Initted			   = false;
					GainRatio          = 0.0f;
					GridSize           = CELLSIZE * CELLSIZE * CELLSIZE;
					Cell               = new GridCell[GridSize];
				}

				~RemoveDuplicates3D()
				{
					vml::os::SafeDeleteArray(Cell);
				}

		};

	}
}
