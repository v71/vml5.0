#pragma once

#define JPH_FLOATING_POINT_EXCEPTIONS_ENABLED
#define JPH_OBJECT_STREAM
#define JPH_PROFILE_ENABLED 
#define JPH_DEBUG_RENDERER

#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Collision/Shape/MeshShape.h>
//#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
//#include <Jolt/Physics/Collision/Shape/CompoundShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>
#include <Jolt/Physics/Collision/NarrowPhaseQuery.h>
#include <Jolt/Physics/Collision/CollideShape.h> 
#include <Jolt/Physics/Collision/Shape/RotatedTranslatedShape.h>
#include <Jolt/Physics/Collision/CollisionDispatch.h>
#include <Jolt/Physics/Collision/ShapeCast.h>
#include <Jolt/Physics/Collision/RayCast.h>
#include <jolt/Physics/Collision/CastResult.h>
#include <Jolt/Physics/Collision/CollisionCollector.h>
#include <Jolt/Physics/Collision/CollisionCollectorImpl.h>
#include <Jolt/Physics/Body/BodyManager.h>
#include <Jolt/Renderer/DebugRenderer.h>
#include <Jolt/Renderer/DebugRendererSimple.h>

namespace JPH
{

	// Disable common warnings triggered by Jolt, you can use JPH_SUPPRESS_WARNING_PUSH / JPH_SUPPRESS_WARNING_POP to store and restore the warning state
	JPH_SUPPRESS_WARNINGS

	// If you want your code to compile using single or double precision write 0.0_r 
	// to get a Real value that compiles to double or float depending if 
	// JPH_DOUBLE_PRECISION is set or not.

	using namespace JPH::literals;

	// Callback for traces, connect this to your own trace function if you have one

	static void TraceImpl(const char* inFMT, ...)
	{
		// Format the message
		va_list list;
		va_start(list, inFMT);
		char buffer[1024];
		vsnprintf(buffer, sizeof(buffer), inFMT, list);
		va_end(list);
		// Print to the TTY
		std::cout << buffer << std::endl;
	}

	#ifdef JPH_ENABLE_ASSERTS

	// Callback for asserts, connect this to your own assert handler if you have one

	static bool AssertFailedImpl(const char* inExpression, const char* inMessage, const char* inFile, unsigned int inLine)
	{
		// Print to the TTY
		std::cout << inFile << ":" << inLine << ": (" << inExpression << ") " << (inMessage != nullptr ? inMessage : "") << std::endl;
		// Breakpoint
		return true;
	};

	#endif // JPH_ENABLE_ASSERTS

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Layer that objects can be in, determines which other objects it can collide with
	// Typically you at least want to have 1 layer for moving bodies and 1 layer for static bodies, but you can have more
	// layers if you want. E.g. you could have a layer for high detail collision (which is not used by the physics simulation
	// but only if you do collision testing).

	namespace Layers
	{
		static constexpr JPH::ObjectLayer NON_MOVING = 0;
		static constexpr JPH::ObjectLayer MOVING = 1;
		static constexpr JPH::ObjectLayer NUM_LAYERS = 2;
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Class that determines if two object layers can collide

	class ObjectLayerPairFilterImpl : public JPH::ObjectLayerPairFilter
	{
		public:

			virtual bool ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const override
			{
				switch (inObject1)
				{
				case Layers::NON_MOVING:
					return inObject2 == Layers::MOVING; // Non moving only collides with moving
				case Layers::MOVING:
					return true;						// Moving collides with everything
				default:
					JPH_ASSERT(false);
					return false;
				}
			}
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Each broadphase layer results in a separate bounding volume tree in the broad phase. You at least want to have
	// a layer for non-moving and moving objects to avoid having to update a tree full of static objects every frame.
	// You can have a 1-on-1 mapping between object layers and broadphase layers (like in this case) but if you have
	// many object layers you'll be creating many broad phase trees, which is not efficient. If you want to fine tune
	// your broadphase layers define JPH_TRACK_BROADPHASE_STATS and look at the stats reported on the TTY.

	namespace BroadPhaseLayers
	{
		static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
		static constexpr JPH::BroadPhaseLayer MOVING(1);
		static constexpr unsigned int NUM_LAYERS(2);
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// BroadPhaseLayerInterface implementation
	// This defines a mapping between object and broadphase layers.

	class BPLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface
	{

		private:

			JPH::BroadPhaseLayer mObjectToBroadPhase[Layers::NUM_LAYERS];

		public:

			virtual unsigned int GetNumBroadPhaseLayers() const override
			{
				return BroadPhaseLayers::NUM_LAYERS;
			}

			virtual JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override
			{
				JPH_ASSERT(inLayer < Layers::NUM_LAYERS);
				#pragma warning(suppress:6385)
				return mObjectToBroadPhase[inLayer];
			}

			#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
			virtual const char* GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override
			{
				switch ((JPH::BroadPhaseLayer::Type)inLayer)
				{
				case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING:	return "NON_MOVING";
				case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::MOVING:		return "MOVING";
				default: JPH_ASSERT(false); return "INVALID";
				}
			}
			#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

			// ---------------------------------------------------------------------------
			// ctor / dtor

			BPLayerInterfaceImpl()
			{
				// Create a mapping table from object to broad phase layer
				mObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
				mObjectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;
			}

			~BPLayerInterfaceImpl()
			{
			}
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Class that determines if an object layer can collide with a broadphase layer

	class ObjectVsBroadPhaseLayerFilterImpl : public JPH::ObjectVsBroadPhaseLayerFilter
	{
		public:

			virtual bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override
			{
				switch (inLayer1)
				{
					case Layers::NON_MOVING:
						return inLayer2 == BroadPhaseLayers::MOVING;
					case Layers::MOVING:
						return true;
					default:
						JPH_ASSERT(false);
						return false;
				}
			}

			// ---------------------------------------------------------------------------
			// ctor / dtor

			ObjectVsBroadPhaseLayerFilterImpl()
			{
			}

			~ObjectVsBroadPhaseLayerFilterImpl()
			{
			}

	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// An example contact listener

	class MyContactListener : public JPH::ContactListener
	{
		public:

			// See: ContactListener

			virtual JPH::ValidateResult OnContactValidate(const JPH::Body& inBody1, 
				                                          const JPH::Body& inBody2, 
				                                          JPH::RVec3Arg inBaseOffset, 
				                                          const JPH::CollideShapeResult& inCollisionResult) override
			{
				//		std::cout << "Contact validate callback" << std::endl;
				// Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
				return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
			}

			virtual void OnContactAdded(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings) override
			{
				//		std::cout << "A contact was added" << std::endl;
			}

			virtual void OnContactPersisted(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings) override
			{
				//		std::cout << "A contact was persisted" << std::endl;
			}

			virtual void OnContactRemoved(const JPH::SubShapeIDPair& inSubShapePair) override
			{
				//		std::cout << "A contact was removed" << std::endl;
			}

			// ---------------------------------------------------------------------------
			// ctor / dtor

			MyContactListener()
			{
			}

			~MyContactListener()
			{
			}

	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// An example activation listener

	class MyBodyActivationListener : public JPH::BodyActivationListener
	{
		public:

			virtual void OnBodyActivated(const JPH::BodyID& inBodyID, uint64_t inBodyUserData) override
			{
				//	std::cout << "A body got activated" << std::endl;
			}

			virtual void OnBodyDeactivated(const JPH::BodyID& inBodyID, uint64_t inBodyUserData) override
			{
				//		std::cout << "A body went to sleep" << std::endl;
			}

			// ---------------------------------------------------------------------------
			// ctor / dtor

			MyBodyActivationListener()
			{
			}

			~MyBodyActivationListener()
			{
			}

	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Thread safe debug renderer

	class ThreadSafeDebugRenderer : public JPH::DebugRendererSimple
	{
		private:

			// vertex and fragmenst shaders used for lines & triangles
			// colors are floats

		   const char* g_vs_src = R"glsl(
											#version 330 core
											layout(location = 0) in vec3 inPos;
											layout(location = 1) in vec4 inColor;
											uniform mat4 uViewProj;
											out vec4 vColor;
											void main() {
												gl_Position = uViewProj * vec4(inPos, 1.0);
												vColor = inColor;
											}
											)glsl";

			const char* g_fs_src = R"glsl(
											#version 330 core
											in vec4 vColor;
											out vec4 oColor;
											void main() {
												oColor = vColor;
											}
										  )glsl";

			// debug drawing data structures

			struct Vertex
			{
				float x, y, z;
				float r, g, b, a;
			};

			struct DebugLine
			{
				JPH::RVec3 from, to;
				JPH::Color color;
			};

			struct DebugTri
			{
				JPH::RVec3 v1, v2, v3;
				JPH::Color color;
				bool cast_shadow;
			};

			struct DebugText
			{
				JPH::RVec3 pos;
				std::string text;
				JPH::Color color;
				float height;
			};

			struct DebugBuffer
			{
				std::vector<DebugLine> lines;
				std::vector<DebugTri> tris;
				std::vector<DebugText> texts;

				void clear()
				{
					lines.clear();
					tris.clear();
					texts.clear();
				}

				void swap(DebugBuffer& other)
				{
					lines.swap(other.lines);
					tris.swap(other.tris);
					texts.swap(other.texts);
				}

			};

			// buffers protected by mutex

			DebugBuffer mPhysicsBuffer; // appended by physics thread
			DebugBuffer mRenderBuffer;  // used by render thread
			std::mutex mMutex;

			// Opengl objects

			GLuint mLineVAO;
			GLuint mLineVBO;
			GLuint mTriVAO;
			GLuint mTriVBO;
			GLuint mShaderProgram;
			GLint  mUniformViewProj;

			// Temporary vertex arrays used during upload on render thread

			std::vector<Vertex> mLineVertices;
			std::vector<Vertex> mTriVertices;

			// -------------------------------------------------------------------------------
			// Text rendering

			void RenderTextGL(const DebugText& t, const float viewProj[16])
			{
				// placeholder for text renderer.
			}

			// -------------------------------------------------------------------------------
			//  Gathers triangles from jolt and passes to vertex buffers

			void UploadAndDrawTris(const float viewProj[16])
			{
				if (mRenderBuffer.tris.empty() )
					return;

				// Build triangle vertices: each tri -> 3 vertices

				mTriVertices.clear();
				mTriVertices.reserve(mRenderBuffer.tris.size() * 3);

				JPH::Color col{1,1,1,1};

				for (const auto& t : mRenderBuffer.tris)
				{
					Vertex v1{}, v2{}, v3{};

					v1.x = (float)t.v1.GetX();
					v1.y = (float)t.v1.GetY();
					v1.z = (float)t.v1.GetZ();

					v2.x = (float)t.v2.GetX();
					v2.y = (float)t.v2.GetY();
					v2.z = (float)t.v2.GetZ();

					v3.x = (float)t.v3.GetX();
					v3.y = (float)t.v3.GetY();
					v3.z = (float)t.v3.GetZ();

					v1.r = col.r;
					v1.g = col.g;
					v1.b = col.b;
					v1.a = col.a;

					v2.r = col.r;
					v2.g = col.g;
					v2.b = col.b;
					v2.a = col.a;

					v3.r = col.r;
					v3.g = col.g;
					v3.b = col.b;
					v3.a = col.a;

					// v1.r = t.color.r / 255.0f;
					// v1.g = t.color.g / 255.0f;
					// v1.b = t.color.b / 255.0f;
					// v1.a = t.color.a / 255.0f;

					// v2.r = t.color.r / 255.0f;
					// v2.g = t.color.g / 255.0f;
					// v2.b = t.color.b / 255.0f;
					// v2.a = t.color.a / 255.0f;

					// v31.r = t.color.r / 255.0f;
					// v3.g = t.color.g / 255.0f;
					// v3.b = t.color.b / 255.0f;
					// v3.a = t.color.a / 255.0f;

					mTriVertices.push_back(v1);
					mTriVertices.push_back(v2);
					mTriVertices.push_back(v3);
				}

				// Upload with orphaning for performance reasons

				glBindBuffer(GL_ARRAY_BUFFER, mTriVBO);
				glBufferData(GL_ARRAY_BUFFER, mTriVertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW); // orphan
				glBufferSubData(GL_ARRAY_BUFFER, 0, mTriVertices.size() * sizeof(Vertex), mTriVertices.data());
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glUseProgram(mShaderProgram);
				glUniformMatrix4fv(mUniformViewProj, 1, GL_FALSE, viewProj);
				glBindVertexArray(mTriVAO);
					glDrawArrays(GL_TRIANGLES, 0, (GLsizei)mTriVertices.size());
				glBindVertexArray(0);
				glUseProgram(0);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

			// -------------------------------------------------------------------------------
			//  Gathers lines from jolt and passes to vertex buffers

			void UploadAndDrawLines(const float viewProj[16])
			{
				if (mRenderBuffer.lines.empty())
					return;

				// Build line vertex array: each line is two vertices

				mLineVertices.clear();
				mLineVertices.reserve(mRenderBuffer.lines.size() * 2);

				JPH::Color col{ 1,1,1,1 };

				for (const auto& ln : mRenderBuffer.lines)
				{
					Vertex a{}, b{};

					a.x = (float)ln.from.GetX();
					a.y = (float)ln.from.GetY();
					a.z = (float)ln.from.GetZ();

					b.x = (float)ln.to.GetX();
					b.y = (float)ln.to.GetY();
					b.z = (float)ln.to.GetZ();

					// Jolt::Color is typically 0..255; convert to 0..1

					a.r = col.r;
					a.g = col.g;
					a.b = col.b;
					a.a = col.a;

					b.r = col.r;
					b.g = col.g;
					b.b = col.b;
					b.a = col.a;

				//	a.r = ln.color.r / 255.0f;
				//	a.g = ln.color.g / 255.0f;
				//	a.b = ln.color.b / 255.0f;
				//	a.a = ln.color.a / 255.0f;

				//	b.r = ln.color.r / 255.0f;
				//	b.g = ln.color.g / 255.0f;
				//	b.b = ln.color.b / 255.0f;
				//	b.a = ln.color.a / 255.0f;

					mLineVertices.push_back(a);
					mLineVertices.push_back(b);
				}

				// Upload with orphaning for performance reasons

				glBindBuffer(GL_ARRAY_BUFFER, mLineVBO);
				glBufferData(GL_ARRAY_BUFFER, mLineVertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW); // orphan
				glBufferSubData(GL_ARRAY_BUFFER, 0, mLineVertices.size() * sizeof(Vertex), mLineVertices.data());

				// Draw
				glUseProgram(mShaderProgram);
				glUniformMatrix4fv(mUniformViewProj, 1, GL_FALSE, viewProj);
				glBindVertexArray(mLineVAO);
				glLineWidth(1.0f);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDrawArrays(GL_LINES, 0, (GLsizei)mLineVertices.size());
				glBindVertexArray(0);
				glUseProgram(0);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			}

			// -------------------------------------------------------------------------------
			// compile shaders

			GLuint CompileShader(GLenum type, const char* src)
			{
				GLuint s = glCreateShader(type);
				glShaderSource(s, 1, &src, nullptr);
				glCompileShader(s);
				GLint compiled = 0;
				glGetShaderiv(s, GL_COMPILE_STATUS, &compiled);
				if (!compiled) {
					char log[1024];
					glGetShaderInfoLog(s, sizeof(log), nullptr, log);
					std::cerr << "Shader compile error: " << log << "\n";
					glDeleteShader(s);
					return 0;
				}
				return s;
			}

			// -------------------------------------------------------------------------------
			// create shaders

			void InitShaders()
			{
				GLuint vs = CompileShader(GL_VERTEX_SHADER, g_vs_src);
				GLuint fs = CompileShader(GL_FRAGMENT_SHADER, g_fs_src);
				mShaderProgram = glCreateProgram();
				glAttachShader(mShaderProgram, vs);
				glAttachShader(mShaderProgram, fs);
				glLinkProgram(mShaderProgram);

				GLint linked = 0;
				glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &linked);
				if (!linked) {
					char log[1024];
					glGetProgramInfoLog(mShaderProgram, sizeof(log), nullptr, log);
					std::cerr << "Shader link error: " << log << "\n";
					glDeleteProgram(mShaderProgram);
					mShaderProgram = 0;
				}
				glDeleteShader(vs);
				glDeleteShader(fs);
			}

			// -------------------------------------------------------------------------------
			// initialize opengl , load sahders and created vaos and vbos

			void InitGL()
			{
				if (mShaderProgram != 0)
					return;

				InitShaders();

				// Create line VAO/VBO
				glGenVertexArrays(1, &mLineVAO);
				glGenBuffers(1, &mLineVBO);
				glBindVertexArray(mLineVAO);
				glBindBuffer(GL_ARRAY_BUFFER, mLineVBO);
				// initially allocate nothing; we'll glBufferData on upload
				glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
				// pos (location 0): vec3
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
				// color (location 1): vec4
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, r));
				glBindVertexArray(0);

				// Create tri VAO/VBO
				glGenVertexArrays(1, &mTriVAO);
				glGenBuffers(1, &mTriVBO);
				glBindVertexArray(mTriVAO);
				glBindBuffer(GL_ARRAY_BUFFER, mTriVBO);
				glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, r));
				glBindVertexArray(0);

				// uniform location
				mUniformViewProj = glGetUniformLocation(mShaderProgram, "uViewProj");
			}

			// -------------------------------------------------------------------------------
			// Jolt line drawing callback

			void DrawLine(JPH::RVec3Arg inFrom, JPH::RVec3Arg inTo, JPH::ColorArg inColor) override
			{
				DebugLine cmd{};
				cmd.from = inFrom;
				cmd.to = inTo;
				cmd.color = inColor;
				std::lock_guard<std::mutex> lk(mMutex);
				mPhysicsBuffer.lines.push_back(cmd);
			}

			// -------------------------------------------------------------------------------
			// Jolt triangle drawing callback
			// jolt calls this function for drawing filled triangles
			// but here i draw everything in wireframe

			void DrawTriangle(JPH::RVec3Arg inV1, JPH::RVec3Arg inV2, JPH::RVec3Arg inV3,
							  JPH::ColorArg inColor, ECastShadow inCastShadow) override
			{
				DebugTri cmd{};
				cmd.v1 = inV1;
				cmd.v2 = inV2;
				cmd.v3 = inV3;
				cmd.color = inColor;
				cmd.cast_shadow = (inCastShadow == ECastShadow::Off);
				std::lock_guard<std::mutex> lk(mMutex);
				mPhysicsBuffer.tris.push_back(cmd);
			}

			// -------------------------------------------------------------------------------
			// text drawing

			void DrawText3D(JPH::RVec3Arg inPosition, const std::string_view& inString,
							JPH::ColorArg inColor, float inHeight) override
			{
				DebugText cmd{};
				cmd.pos = inPosition;
				cmd.text = std::string(inString);
				cmd.color = inColor;
				cmd.height = inHeight;
				std::lock_guard<std::mutex> lk(mMutex);
				mPhysicsBuffer.texts.push_back(std::move(cmd));
			}

		public:

			// -------------------------------------------------------------------------------
			// Call from render thread (with GL context current) each frame:
			// Main render function: must run on render thread with GL context

			void RenderAndClear(const float viewProj[16])
			{
				// after swap, render buffer contains
				// the latest commands and physicsBuffer can be reused

				std::lock_guard<std::mutex> lk(mMutex);
				mPhysicsBuffer.swap(mRenderBuffer);

				// Upload & draw lines and triangles

				UploadAndDrawLines(viewProj);
				UploadAndDrawTris(viewProj);

				// Draw texts (simple)

				for (const auto& tx : mRenderBuffer.texts)
					RenderTextGL(tx, viewProj);

				// Clear the render buffer after consuming

				mRenderBuffer.clear();
			}

			//------------------------------------------------------------------
			// ctor / dtor

			ThreadSafeDebugRenderer()
			{
				mLineVAO		 = 0;
				mLineVBO		 = 0;
				mTriVAO			 = 0;
				mTriVBO			 = 0;
				mShaderProgram   = 0;
				mUniformViewProj = 0;
				
				// initialize opengl
				InitGL();
				
				// Initialize Jolt hook
				Initialize();
			}

			~ThreadSafeDebugRenderer() override
			{
				if (mLineVBO) { glDeleteBuffers(1, &mLineVBO); mLineVBO = 0; }
				if (mLineVAO) { glDeleteVertexArrays(1, &mLineVAO); mLineVAO = 0; }
				if (mTriVBO) { glDeleteBuffers(1, &mTriVBO); mTriVBO = 0; }
				if (mTriVAO) { glDeleteVertexArrays(1, &mTriVAO); mTriVAO = 0; }
				if (mShaderProgram) { glDeleteProgram(mShaderProgram); mShaderProgram = 0; }
			}

	};


////////////////////////////////////////////////////////////////////////////////////////////////////

	struct Hit
	{
		bool HasHit = false;
		float penetration = -FLT_MAX;
		glm::vec3 Position = glm::vec3(0, 0, 0);
		glm::vec3 Normal = glm::vec3(0, 0, 0);
	};

	struct SingleHitCollector : public JPH::CollisionCollector<JPH::CollideShapeResult, JPH::CollisionCollectorTraitsCollideShape>
	{

		std::vector<Hit> hits;
		bool hasHits = false;

		virtual void AddHit(const JPH::CollideShapeResult& result) override
		{
			hasHits = true;
			Hit hit{};
			hit.HasHit = true;
			hit.penetration = result.mPenetrationDepth;
			hit.Position = glm::vec3(result.mContactPointOn1.GetX(), result.mContactPointOn1.GetY(), result.mContactPointOn1.GetZ());
			hit.Normal = glm::vec3(result.mPenetrationAxis.GetX(), result.mPenetrationAxis.GetY(), result.mPenetrationAxis.GetZ());
			hits.push_back(hit);
		}
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// shapes

	enum class Shapes : uint32_t
	{
		NO_SHAPE = 0,
		BOX_SHAPE = 1,
		SPHERE_SHAPE = 2,
	};
}

namespace vml
{
	namespace jolt
	{

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Jolt physics engine wrapper

		class JoltPhysics
		{

			private:

			public:

				// This is the max amount of rigid bodies that you can add to the physics system. If you try to add more you'll get an error.
				// Note: This value is low because this is a simple test. For a real project use something in the order of 65536.
				unsigned int MaxBodies;

				// This determines how many mutexes to allocate to protect rigid bodies from concurrent access. Set it to 0 for the default settings.
				unsigned int NumBodyMutexes;

				// This is the max amount of body pairs that can be queued at any time (the broad phase will detect overlapping
				// body pairs based on their bounding boxes and will insert them into a queue for the narrowphase). If you make this buffer
				// too small the queue will fill up and the broad phase jobs will start to do narrow phase work. This is slightly less efficient.
				// Note: This value is low because this is a simple test. For a real project use something in the order of 65536.
				unsigned int MaxBodyPairs;

				// This is the maximum size of the contact constraint buffer. If more contacts (collisions between bodies) are detected than this
				// number then these contacts will be ignored and bodies will start interpenetrating / fall through the world.
				// Note: This value is low because this is a simple test. For a real project use something in the order of 10240.
				unsigned int MaxContactConstraints;

				// maximum number of mb allocated for TempAllocar array
				unsigned int MaxMem;

				// We simulate the physics world in discrete time steps. 60 Hz is a good rate to update the physics system.
				float DeltaTime;

				// Create mapping table from object layer to broadphase layer
				// Note: As this is an interface, PhysicsSystem will take a reference to this so this instance needs to stay alive!
				// Also have a look at BroadPhaseLayerInterfaceTable or BroadPhaseLayerInterfaceMask for a simpler interface.
				JPH::BPLayerInterfaceImpl BroadPhaseLayerInterface;

				// Create class that filters object vs broadphase layers
				// Note: As this is an interface, PhysicsSystem will take a reference to this so this instance needs to stay alive!
				// Also have a look at ObjectVsBroadPhaseLayerFilterTable or ObjectVsBroadPhaseLayerFilterMask for a simpler interface.
				JPH::ObjectVsBroadPhaseLayerFilterImpl ObjectVsBroadphaseLayerFilter;

				// Create class that filters object vs object layers
				// Note: As this is an interface, PhysicsSystem will take a reference to this so this instance needs to stay alive!
				// Also have a look at ObjectLayerPairFilterTable or ObjectLayerPairFilterMask for a simpler interface.
				JPH::ObjectLayerPairFilterImpl ObjectVsObjectLayerFilter;

				// Now we can create the actual physics system.
				JPH::PhysicsSystem* PhysicsSystem;
				JPH::TempAllocatorImpl* TempAllocator;
				JPH::JobSystemThreadPool* JobSystem;

				JPH::MyBodyActivationListener BodyActivationListener;
				JPH::MyContactListener ContactListener;

			public:

				JPH::BodyInterface* BodyInterface;
				JPH::BodyID collisionMapId;

				JPH::RefConst<JPH::Shape>	  mesh_shape;
				JPH::RefConst<JPH::Shape>	  BoxShape;
				JPH::RefConst<JPH::Shape>	  SphereShape;
				JPH::ThreadSafeDebugRenderer* gDebugRenderer;

				// ---------------------------------------------------------------------------
				
				void CreateConcaveMesh(vml::meshes::Mesh3d* collisionmesh)
				{
					const std::vector<unsigned int>& indices = collisionmesh->GetSurfaceIndices();
					const std::vector<float>& vertices = collisionmesh->GetVertexArray();
					
					JPH::VertexList vertexList;
					JPH::IndexedTriangleList triangleList;

					for (size_t i = 0; i < vertices.size() / 4; ++i)
					{
						size_t idx = i * 4;
						vertexList.emplace_back(JPH::Float3(vertices[idx], vertices[idx + 1], vertices[idx + 2]));
					}

					for (size_t i = 0; i < indices.size() / 3; ++i)
					{
						size_t idx = i * 3;
						triangleList.emplace_back(JPH::IndexedTriangle((uint32_t)indices[idx], (uint32_t)indices[idx + 1], (uint32_t)indices[idx + 2]));
					}
					
					// Create mesh shape settings
					JPH::MeshShapeSettings settings(vertexList, triangleList);
					settings.Sanitize();
					settings.mBuildQuality = JPH::MeshShapeSettings::EBuildQuality::FavorRuntimePerformance;

					// Create shape
					JPH::Shape::ShapeResult result = settings.Create();
					if (result.IsValid())
						mesh_shape = result.Get();
					else
						vml::os::Message::Error("couldn't get resulting shape {}", result.GetError());

					JPH::BodyCreationSettings mesh_settings(mesh_shape, JPH::RVec3(0.0, 0.0, 0.0), JPH::Quat::sIdentity(), JPH::EMotionType::Static, JPH::Layers::NON_MOVING);
					JPH::Body* mesh_body = BodyInterface->CreateBody(mesh_settings); // Note that if we run out of bodies this can return nullptr
					BodyInterface->AddBody(mesh_body->GetID(), JPH::EActivation::DontActivate);
					collisionMapId = mesh_body->GetID();
				
				}
				
				// ---------------------------------------------------------------------------

				void Init()
				{
					// Register allocation hook. In this example we'll just let Jolt use malloc / free 
					// but you can override these if you want (see Memory.h).
					// This needs to be done before any other Jolt function is called.
					JPH::RegisterDefaultAllocator();

					// Install trace and assert callbacks
					JPH::Trace = JPH::TraceImpl;
					JPH_IF_ENABLE_ASSERTS(JPH::AssertFailed = JPH::AssertFailedImpl;)

					// Create a factory, this class is responsible for creating instances of classes based on their 
					// name or hash and is mainly used for deserialization of saved data.
					// It is not directly used in this example but still required.
					JPH::Factory::sInstance = new JPH::Factory();

					// Register all physics types with the factory and install their collision handlers 
					// with the CollisionDispatch class.
					// If you have your own custom shape types you probably need to register their handlers
					// with the CollisionDispatch before calling this function.
					// If you implement your own default material (PhysicsMaterial::sDefault) make sure to 
					// initialize it before this function or else this function will create one for you.
					JPH::RegisterTypes();

					// We need a temp allocator for temporary allocations during the physics update. We're
					// pre-allocating 10 MB to avoid having to do allocations during the physics update.
					// B.t.w. 10 MB is way too much for this example but it is a typical value you can use.
					// If you don't want to pre-allocate you can also use TempAllocatorMalloc to fall back to
					// malloc / free.
					TempAllocator = new JPH::TempAllocatorImpl(MaxMem);

					// We need a job system that will execute physics jobs on multiple threads. Typically
					// you would implement the JobSystem interface yourself and let Jolt Physics run on top
					// of your own job scheduler. JobSystemThreadPool is an example implementation.
					JobSystem = new JPH::JobSystemThreadPool(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1u);

					PhysicsSystem = new JPH::PhysicsSystem;

					PhysicsSystem->Init(MaxBodies,
										NumBodyMutexes,
										MaxBodyPairs,
										MaxContactConstraints,
										BroadPhaseLayerInterface,
										ObjectVsBroadphaseLayerFilter,
										ObjectVsObjectLayerFilter);

					// A body activation listener gets notified when bodies activate
					// and go to sleep Note that this is called from a job so 
					// whatever you do here needs to be thread safe.
					// Registering one is entirely optional.
					PhysicsSystem->SetBodyActivationListener(&BodyActivationListener);

					// A contact listener gets notified when bodies (are about to) 
					// collide, and when they separate again.
					// Note that this is called from a job so whatever you 
					// do here needs to be thread safe.
					// Registering one is entirely optional.
					PhysicsSystem->SetContactListener(&ContactListener);

					// The main way to interact with the bodies in the physics system
					// is through the body interface. There is a locking and a non-locking
					// variant of this. We're going to use the locking version
					// (even though we're not planning to access bodies from multiple threads)
					BodyInterface = &PhysicsSystem->GetBodyInterface();

					//	BodyManager::DrawSettings drawsettings;
					//	drawsettings.mDrawShapeWireframe = true;

					// create the sphere shape

					JPH::SphereShapeSettings spheresettings;
					spheresettings.mRadius = 1.0f;
					SphereShape = spheresettings.Create().Get();

					// create the box shape
					// box half extents

					JPH::BoxShapeSettings boxsettings;
					boxsettings.mHalfExtent = JPH::Vec3(1.0f, 1.0f, 1.0f);
					BoxShape = boxsettings.Create().Get();
			
					// allocate threaded debug renderer

					gDebugRenderer = new JPH::ThreadSafeDebugRenderer();
					
				}

				// ---------------------------------------------------------------------------
				// Optional step: Before starting the physics simulation you can optimize the broad phase. 
				// This improves collision detection performance (it's pointless here because we only have 2 bodies).
				// You should definitely not call this every frame or when e.g. streaming in a new level section as 
				// it is an expensive operation. Instead insert all new objects in batches instead of 1 at a time 
				// to keep the broad phase efficient.

				void OptimizeBroadPhase()
				{
					PhysicsSystem->OptimizeBroadPhase();
				}

				// ---------------------------------------------------------------------------

				void Close()
				{
					if (!collisionMapId.IsInvalid())
					{
						// remove collision mesh
						BodyInterface->RemoveBody(collisionMapId);
						BodyInterface->DestroyBody(collisionMapId);
					}

					// release mem
					vml::os::SafeDelete(gDebugRenderer);
					vml::os::SafeDelete(TempAllocator);
					vml::os::SafeDelete(JobSystem);
					vml::os::SafeDelete(PhysicsSystem);

					// Unregisters all types with the factory and cleans up the default material
					JPH::UnregisterTypes();

					// delete factory
					vml::os::SafeDelete(JPH::Factory::sInstance);
				}

				// ---------------------------------------------------------------------------
				// ctor / dtor

				JoltPhysics()
				{
					TempAllocator  = nullptr;
					JobSystem      = nullptr;
					PhysicsSystem  = nullptr;
					BodyInterface  = nullptr;
					gDebugRenderer = nullptr;

					MaxBodies			  = 1024u;
					MaxBodyPairs		  = 1024u;
					MaxContactConstraints = 1024u;
					MaxMem				  = 10u * 1024u * 1024u;
					NumBodyMutexes		  = 0u;
					DeltaTime			  = 1.0f / 60.0f;
				}

				~JoltPhysics()
				{
				}

		};

	}
}
