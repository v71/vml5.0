#pragma once

namespace fa2040
{
	
	class Drone 
	{
		
		private:

			std::string				ScreenName;

			vml::models::Model3d_2* BodyModel;
			vml::geo2d::PathFinder* PathFinder;
			glm::vec3*				Path;
			glm::vec3*				PathDir;
			float*					PathInvSegmentLength;
			float*					PathSegmentLength;
			float					PathLenght;
			float					InvPathLenght;

			int						MaxPathCount;
			int						PathCount;
			int						PathIndex;

			int						State;
			int					    StopState;

			glm::ivec2				CurPathA;
			glm::ivec2				CurPathB;
			glm::ivec2				StartPathA;				// Start Origin, this will be the starting path wehen the drone will return after chase state
			glm::ivec2				StartPathB;				// End Origin, this will be the starting path wehen the drone will return after chase state
			glm::vec3			    Position;
			glm::vec3				CurrentOrigin;
			glm::vec3				StartOrigin;			// StartOriign is the current drone position which will be used when switching pathfinding to avoid jittering

			float					T;
			float					CruiseSpeed;			// cruise speed
			float				    Vmax;					// max velocity
			float					CurPath;
			float					Xl;						// accellerating distance vefore reaching max velocity
			float					V0;						// initial velocity
			float					M;						// accelleration factor
			float					PrevCruiseSpeed;
			float				    Xm;						// decellerating distance vefore stopping
			float					DeCell;					// decelleration factor when drone stops and resume motion

			float					SlowingRadius;
			float					ChaseSpeed;
			float					AvoidSpeed;
			int						NavMeshCellId;			// id of cell in the navmesh the drone is in.
															// Navmesh is segmented into 'islands'
															// this value indicates in which of these islands the
															// drone belongs to see pathfinder for more info
			int						Priority;

			vml::models::Model3d_2* Player;

			// ----------------------------------------------------------
			//

			static const unsigned int STOP_STATE_0 = 0;
			static const unsigned int STOP_STATE_1 = 1;
			static const unsigned int STOP_STATE_2 = 2;
			static const unsigned int STOP_STATE_3 = 3;
			static const unsigned int STOP_STATE_4 = 4;

			// -------------------------------------------------------------------
			// Get navmesh cell the drone is acutally nearst to
			// this is necessary becasue after the chas function the drone could be reset back
			// to its original route and it might happen the drone is outside
			// the walkable cell
			
			glm::ivec2 GetNavMeshCell()
			{
				if (!PathFinder)
					vml::os::Message::Error("Drone : ", "PathFinder is nnull");

				glm::vec3 p = BodyModel->GetPosition();
				
				// visual studio 2022 complains PathFinder pointer to be null

				if (PathFinder)
				{
					glm::ivec2 ci;
					
					if (PathFinder->GetCellIndicesFromPoint(BodyModel->GetPosition(), ci))
					{
						// NavMeshCellId its the isle in the navmesh bitmap where originally the drone had started

						if (PathFinder->GetCellIdFromIndices(ci) == -1) {
							return PathFinder->FindNearestCellToAnotherCell(ci.x, ci.y, NavMeshCellId);
						}

						return ci;
					}
				}

				return glm::ivec2(-1, -1);
			}
			
			// ----------------------------------------------------------
			//

			void ComputeSpeed2(const float d)
			{
				// cruise speed, accelleration / decelleration control

				if (StopState == STOP_STATE_0)
				{
					if (d < Xl) CruiseSpeed = V0 + d * M;
					else if (d > Xm)
						CruiseSpeed = Vmax - (d - Xm) * M;
					else CruiseSpeed = Vmax;
				}
				
				// if stop state is 1 , the decelleration is
				// taking place, this state saves the 
				// current speed used to resume the motion once
				// requested

				if (StopState == STOP_STATE_1)
				{
					PrevCruiseSpeed = CruiseSpeed;
					StopState = STOP_STATE_2;
				}

				// decelleration procedure
				// once hte drone is stopped , the 
				// state is set to a neutral state

				if (StopState == STOP_STATE_2)
				{
					CruiseSpeed -= DeCell;
					if (CruiseSpeed < 0)
					{
						CruiseSpeed = 0;
						StopState = STOP_STATE_3;
					}
				}

				// in case the motion is resumed
				// the drone is set in motion
				// until the previous speed is 
				// saved in the 'stop state 1' is achieved 

				if (StopState == STOP_STATE_4)
				{
					CruiseSpeed += DeCell;
					if (CruiseSpeed > PrevCruiseSpeed)
					{
						CruiseSpeed = PrevCruiseSpeed;
						StopState = STOP_STATE_0;
					}
				}
				
			}

		public:

			// ----------------------------------------------------------
			//

			static const unsigned int NEUTRAL				= 0;
			static const unsigned int START_PATH			= 1;
			static const unsigned int START_FORWARD_PATROL  = 2;
			static const unsigned int START_BACKWARD_PATROL = 3;
			static const unsigned int CHASE_PLAYER			= 4;
			static const unsigned int STOP_CHASE			= 5;
			static const unsigned int GO_BACK_TO_ROUTE		= 6;

			// ----------------------------------------------------------
			//

			const std::string& GetScreeName() const { return ScreenName; }

			// ----------------------------------------------------------
			//

			void CreateObject(const std::string &screenname,vml::models::ModelHandler* modelhandler)
			{
				if (screenname.empty())
					vml::os::Message::Error("ScreenName is empty");

				// copy screenname

				float sc = 0.5f;

				ScreenName = screenname;

				std::string sourcepath = vml::utils::GlobalPaths::GetInstance()->GetFullDebugPath() + "/meshes/cube_imp/3dfs/";

				BodyModel = modelhandler->Add(ScreenName+"/Body",
											  sourcepath + "cube.3df",
											  glm::vec3(0, 0, 0), 
											  glm::vec3(0, 0, 0), 
											  glm::vec3(sc, sc, sc), 
											  { vml::models::EULER });

			}

			// ----------------------------------------------------------
			//

			void AttachPlayer(vml::models::Model3d_2 *player)
			{
				if (!player)
					vml::os::Message::Error("Player pointe is null");
				Player = player;
			}

			// ----------------------------------------------------------
			// attach pathfinder server to drone

			void AttachPathFinder(vml::geo2d::PathFinder* pathfinder)
			{
				if (pathfinder)
				{
					// initialise data memebrs

					PathFinder           = pathfinder;
					MaxPathCount         = PathFinder->GetSize();
					Path                 = new glm::vec3[MaxPathCount];
					PathDir              = new glm::vec3[MaxPathCount];;
					PathInvSegmentLength = new float[MaxPathCount];
					PathSegmentLength    = new float[MaxPathCount];
				}
				else
				{
					vml::os::Message::Error("Drone :", "Null PathFinder (2)");
				}
			}

			// ----------------------------------------------------------
			// this is used when the drone needs a fixed patrolling route
			// if you need to change the route in realtime use set route

			bool SetInitialRoute(const glm::ivec2& a, const glm::ivec2& b)
			{
				
				if (PathFinder)
				{
					// origin position is set at cell pointerd by 'a'
					// the position is needed becasue
					// the drone needs to go back to 
					// its original path, after any event 
					// which might breaks the set route
					// currently being travelled

					vml::geo2d::PathFinder::DebugCell cell;

					// visual studio 2022 complains about PathFinder being null

					if (PathFinder) {
						cell = PathFinder->GetCellAt(a.x, a.y);
					}

					glm::vec3 position = (cell.Pa + cell.Pb + cell.Pc + cell.Pd) * 0.25f;

					StartPathA    = a;
					StartPathB    = b;
					CurPathA      = a;
					CurPathB      = b;
					StartOrigin   = position;
					CurrentOrigin = position;
					Position      = position;

					PathIndex     = 0;
					PathCount     = 0;
					State         = NEUTRAL;
					PathLenght    = 0.0f;
					InvPathLenght = 0.0f;
					T             = 0.0f;
					CurPath       = 0.0f;
					CruiseSpeed   = 0.0f;

					// * Important *
					// get copter position to retrieve navmesh cell index
					// for the pathfiunder in the case due to navmesh segmentation
					// the player could intercept a non traversable cell
					// this interrupting the chase function for drones
					// each isolated region in the navmesh bitmap has a value
					// assigned, we need to know in which of these region

					if (PathFinder) {
						NavMeshCellId = PathFinder->GetCellIdFromIndices(a);
					}

					BodyModel->SetPosition(position);

				}
				else
				{
					vml::os::Message::Error("Drone :", "Null PathFinder (3)");
				}

				return false;
			}

			// ----------------------------------------------------------
			// copy path rom path finder to current path
			// we do this becasue the pathfinder act just
			// like a server, we ask for a path it computes and
			// stays in its internal data , we need to copy
			// that data , if we do not , the next time
			// we ask for a path, it will delete the previous

			bool SetRoute(const glm::vec3 &origin,const glm::ivec2& a, const glm::ivec2& b)
			{
				if (!PathFinder)
					vml::os::Message::Error("Drone : ", "PathFinder is null");

				PathIndex       = 0;
				PathCount       = 0;
				State           = NEUTRAL;
				StopState	    = STOP_STATE_0;
				Position        = glm::vec3(0, 0, 0);
				PathLenght      = 0.0f;
				InvPathLenght   = 0.0f;
				T			    = 0.0f;
				CurPath		    = 0.0f;
				CruiseSpeed     = 0.0f;
				PrevCruiseSpeed = 0.0f;

				// compute path

				glm::ivec2 ia = b;
				glm::ivec2 ib = a;
				
				// if cell are coincident this is 
				// a corner case , yet it mighthappen
				
				if (ia.x == ib.x && ia.y == ib.y)
				{
					Path[0] = StartOrigin;
					PathDir[0] = glm::vec3(0, 0, 0);
					PathSegmentLength[0] = 0.0f;
					PathInvSegmentLength[0] = 1.0f;
					PathLenght = 0;
					InvPathLenght = 1.0f;
					PathCount = 1;
					return true;
				}

				if (PathFinder)
				{
					// it might happen that the drone is in a non valid cell, if this happens
					// then we need to find the nearest valid cell or the pasth will be null

					//	if (PathFinder->GetCellIdFromIndices(a.x, a.y) == -1) ia = PathFinder->FindNearestCellToAnotherCell(a.x, a.y, NavMeshCellId);
					//	if (PathFinder->GetCellIdFromIndices(b.x, b.y) == -1) ib = PathFinder->FindNearestCellToAnotherCell(b.x, b.y, NavMeshCellId);

					if (PathFinder->GetCellIdFromIndices(a) == -1) 
						vml::os::Message::Error("Drone PathFinder : a is in a non walkable cell");

					if (PathFinder->GetCellIdFromIndices(b) == -1) 
						vml::os::Message::Error("Drone PathFinder : b is in a non walkable cell");

					if (!PathFinder->FindPath(ia.x, ia.y, ib.x, ib.y, origin))
						vml::os::Message::Error("Drone PathFinder : cannot find path");

					if (PathFinder->GetPathCount() > 0)
					{

						PathLenght = PathFinder->GetPathLength();
						PathCount = PathFinder->GetPathCount();
						InvPathLenght = 1.0f / PathLenght;

						// copy path , the pathfinder acts as a server
						// so each drone has its own path

						for (size_t i = 0; i < PathCount; ++i)
						{
							Path[i] = PathFinder->GetPAvgrOut()[i];
							PathDir[i] = PathFinder->GetPAvgrDir()[i];
							PathInvSegmentLength[i] = PathFinder->GetPAvgrInvSegmentLength()[i];
							PathSegmentLength[i] = PathFinder->GetPAvgrSegmentLength()[i];
						}

						Position = Path[0];
						BodyModel->SetPosition(Path[0]);
						return true;
					}
				}

				return false;
			}

			// ----------------------------------------------------------
			//

			void Controller()
			{
				if (!PathFinder)
					vml::os::Message::Error("Drone : ", "PathFinder is null");
				if (!Player)
					vml::os::Message::Error("Drone : ", "Player pointer is null");

				//	std::cout << "called controller for object " << BodyModel->GetScreenName() << std::endl;

				// start path

				if (State == START_PATH)
				{
					//	std::cout << "Started Controller for object :" << BodyModel->GetScreenName() << std::endl;

					SetRoute(CurrentOrigin,CurPathA, CurPathB);
					
					// start path state

					State	        = START_FORWARD_PATROL;
					StopState       = STOP_STATE_0;
					T		        = 0.0f;						// T is initialize in the SetRoute function
					CurPath         = 0.0f;						// CurPath is initialize in the SetRoute function
					CruiseSpeed     = 0.0f;	
					PrevCruiseSpeed = 0.0f;
					PathIndex       = 0;
				}
				
				// path to end point

				if (State == START_FORWARD_PATROL)
				{
					
					Position = Path[PathIndex] + T * PathDir[PathIndex];

					BodyModel->SetPosition(Position);
					
					// step 'time' 

					T += CruiseSpeed * PathInvSegmentLength[PathIndex];
					
					// decelleration \ accellereation when reaching 
					// initial or final position 

					ComputeSpeed2(CurPath);

					if (T > 1.0f)
					{
						CurPath += PathSegmentLength[PathIndex] * InvPathLenght;
						PathIndex++;
						T = 0.0f;

						if (PathIndex > PathCount - 2)
						{
							PathIndex = PathCount - 1;
							Position = Path[PathIndex];
							BodyModel->SetPosition(Position);
							State = START_BACKWARD_PATROL;
							CruiseSpeed = 0.0f;
							CurPath = 1.0f;
						}
					}
				}
				
				// path to start point

				if (State == START_BACKWARD_PATROL)
				{
					int j = PathIndex - 1;

					Position = Path[PathIndex] - T * PathDir[j];

					BodyModel->SetPosition(Position);

					// step 'time' 

					T += CruiseSpeed * PathInvSegmentLength[j];

					// decelleration \ accellereation when reaching 
					// initial or final position 

					ComputeSpeed2(CurPath);

					if (T > 1.0f)
					{
						CurPath -= PathSegmentLength[j] * InvPathLenght;
						PathIndex--;
						T = 0.0f;

						if (PathIndex < 1)
						{
							PathIndex = 0;
							Position = Path[PathIndex];
							BodyModel->SetPosition(Position);
							State = START_FORWARD_PATROL;
							CruiseSpeed = 0.0f;
							CurPath = 0.0f;
						}
					}
				}
				
				// chase player 

				if (State == CHASE_PLAYER)
				{
					float sphereepsilon = 1.5f;
					
					Position = BodyModel->GetPosition();
					glm::vec3 dist(FLT_MAX,FLT_MAX,FLT_MAX);
					
					// viusal studio 2022 cokplains about Plyaer pointer being null

					if (Player) {
						dist = Player->GetPosition() - Position;
					}

					float distance = sqrtf(dist.x * dist.x + dist.y * dist.y + dist.z*dist.z);
					
					if (distance > SlowingRadius)
					{
						float dv = (distance - SlowingRadius) ;
						glm::vec3 newvelocity = dist * ChaseSpeed * dv / distance;
						newvelocity.z = 0.0f;
						BodyModel->Move(newvelocity);
					}
					else
					{
						
						BodyModel->MoveY(dist.y* AvoidSpeed / distance);
						
						float ri = BodyModel->GetRadius() * sphereepsilon;
						float rj = FLT_MAX;
						
						if (Player) {
							rj = Player->GetRadius() * sphereepsilon;
						}

						float disp = distance - ri - rj;
					
						if (disp < 0)
						{
							glm::vec3 d = dist * disp / distance;
							d.z = 0;
							BodyModel->Move(d);
						}
						
					}

				}

				// stops chase and go back to starting route

				if (State == STOP_CHASE)
				{
					// its possible that the path is null
					// this never happens , but its a possibility
					// in that case the drone will not move
					
					if (SetRoute(BodyModel->GetPosition(), GetNavMeshCell(), StartPathA))
					{
						State = GO_BACK_TO_ROUTE;
					}
				}

				// go back after stopping chase to starting route to replay initial route

				if (State == GO_BACK_TO_ROUTE)
				{

					Position.x = Path[PathIndex].x + T * PathDir[PathIndex].x;
					Position.y = Path[PathIndex].y + T * PathDir[PathIndex].y;
					Position.z = Path[PathIndex].z + T * PathDir[PathIndex].z;
					
					BodyModel->SetPosition(Position);

					T += CruiseSpeed * PathInvSegmentLength[PathIndex];
					
					// decelleration \ accellereation when reaching
					// initial position

					if (CruiseSpeed < Vmax) CruiseSpeed += DeCell;
						else
					if (CruiseSpeed > Vmax) CruiseSpeed -= DeCell;

					if (T > 1.0f)
					{
						CurPath += PathSegmentLength[PathIndex] * InvPathLenght;
						PathIndex++;
						T = 0.0f;
				
						if (PathIndex > PathCount - 2)
						{
							PathIndex = PathCount - 1;
							Position.x = Path[PathIndex].x;
							Position.y = Path[PathIndex].y;
							Position.z = Path[PathIndex].z;
				
							BodyModel->SetPosition(Position);
							CurrentOrigin = Position;
							State = START_PATH;
						}
					}

				}

				// recoil position

			//	glm::vec3 q = BodyModel->GetPosition();
			//	float recoilspeed = 0.05f;
			//	if (BodyModel->GetPosition().z < 0)
			//		BodyModel->SetPosition(glm::vec3(q.x, q.y, q.z + recoilspeed));

			}

			// ----------------------------------------------------------
			//

			void StartCourse()  { State     = START_PATH; }
			void StopCourse()   { StopState = STOP_STATE_1; }
			void ResumeCourse() { StopState = STOP_STATE_4; }
			void StopChase()    { State     = STOP_CHASE; }
			void ChasePlayer()  { State     = CHASE_PLAYER; }

			// ----------------------------------------------------------
			//
			
			int				 GetPathCount()		const { return PathCount; }
			const glm::vec3* GetPath()			const { return Path; }
			const glm::vec3& GetPosition()		const { return Position; }
			float			 GetSlowingRadius() const { return SlowingRadius; }
			int				 GetNavMeshCellId() const { return NavMeshCellId; }
			bool			 IsChasingPlayer()  const { return State == CHASE_PLAYER; }
			bool			 IsNeutral()	    const { return State == NEUTRAL; }
			int				 GetPriority()	    const { return Priority; }
			const glm::ivec2 GetStartPathA() const { return StartPathA; }
			const glm::ivec2 GetStartPathB() const { return StartPathB; }

			// ----------------------------------------------------------
			//

			bool IsFollowingPath()  const { return State == START_FORWARD_PATROL || State == START_BACKWARD_PATROL; }

			// ----------------------------------------------------------
			//

			void PrintPath() const
			{
				if (!PathFinder)
					vml::os::Message::Error("Drone : ", "PathFinder is nnull");
				for (int i = 0; i < PathCount; ++i)
					std::cout << "AddPoint ( " << Path[i].x << " , " << Path[i].y << " );" << std::endl;
				std::cout << std::endl;
			}

			// ----------------------------------------------------------
			//

			Drone(const std::string& screenname, vml::models::ModelHandler* modelhandler, int priority)
			{
				float f = 1.5f;

				BodyModel			 =  nullptr;
				PathFinder			 =  nullptr;
				Path				 =  nullptr;
				PathDir				 =  nullptr;
				PathInvSegmentLength =  nullptr;
				PathSegmentLength    =  nullptr;
				Player				 =  nullptr;
				MaxPathCount		 =  0;
				PathCount			 =  0;
				PathLenght			 =  0.0f;
				PathIndex			 =  0;
				State				 =  NEUTRAL;
				StopState			 =  STOP_STATE_0;
				T					 =  0.0f;
				CruiseSpeed			 =  0.0f;
				PrevCruiseSpeed		 =  0.0f;
				CurPath				 =  0.0f;
				StartPathA			 =  glm::ivec2(-1, -1);
				StartPathB			 =  glm::ivec2(-1, -1);
				StartOrigin			 =  glm::vec3(0, 0, 0);
				NavMeshCellId		 = -1;
				Priority			 =  priority;

				Vmax				 = 0.05f*f;
				Xl					 = 0.1f;
				Xm					 = 1 - Xl;
				V0					 = 0.01f * f;
				M					 = (Vmax - V0) / Xl;
				DeCell				 = 0.00125f*f;
				SlowingRadius		 = 5.0f;
				ChaseSpeed			 = Vmax * 1.5f;
				AvoidSpeed			 = ChaseSpeed * 1.5f;

				// create object

				CreateObject(screenname,modelhandler);
			}

			~Drone()
			{
				vml::os::SafeDeleteArray(Path);
				vml::os::SafeDeleteArray(PathDir);
				vml::os::SafeDeleteArray(PathInvSegmentLength);
				vml::os::SafeDeleteArray(PathSegmentLength);
			}
		
	};
	
}

/*
	class Drone : public vml::objects::Object3d_2
	{

		private:

			int					    PathFinderStartingCellId;
			int						AvoidPlayer;
			glm::vec3				NearestPathDestPoint;
			glm::vec3				NearestPathDir;


	//		bool					Firing;
	//		int						TurnState;
			int						AvoidState;
	//		float					Yaw;					// yaw angle
	//		float					Roll;					// roll angle
	//		float					Pitch;					// pich angle
	//		float					PrevYaw;				// store previous angular value used in bumping
	//		float					PrevPitch;				// store previous angular value used in bumping
	//		float					TurnSpeed;				// yaw turn speed
	//		float					RollAngleLimit;			// pitch angle limit when turining
	//		float				    ClimbAngleLimit;		// max inclination when levelling up or down
	//		float					RollDampening;			// return to 0 pitch rate
	//		float					RollSpeed;				// pitch decrease when reaching left or right turining limits
	//		float					DampingEps;				// eps to check if damping is coming to a rest, the lower, the latter the sate will be set to true
	//		float					BumpSpeed;
	//		float					BowLimit;
	//		float					BowTau;
	//		float					ClimbSpeed;
	//		bool					Boost;
	//		bool					DampingIsOver;			// check when harmonic damping is nearly finished
	//		float					PreviousSpeed;

			// animation control data members

	//		float aT;
	//		float Theta0;
	//		float Theta1;
	//		float ThetaLimit;
	//		float BladesSpeed;
	//		float RotorRotationSpeed;

	//		bool UpKeyPressed;
	//		bool DownKeyPressed;
	//		bool LeftKeyPressed;
	//		bool RightKeyPressed;

			// ----------------------------------------------------------

			static const unsigned int NEUTRAL_POSE = 0;
			static const unsigned int LEFT_POSE = 1;
			static const unsigned int RIGHT_POSE = 2;
			static const unsigned int BUMP_RIGHT = 3;
			static const unsigned int BUMP_LEFT = 4;
			static const unsigned int BUMP_DOWN = 5;
			static const unsigned int BUMP_UP = 6;

		public:

			// ----------------------------------------------------------
			//

	//		ObjectFrustm			 Frustum;

			static const unsigned int AVOID_STATE_0 = 0;
			static const unsigned int AVOID_STATE_1 = 1;

			// ----------------------------------------------------------

	//		JPH::Hit WallHit;
	//		bool CastRay;

			
			// ----------------------------------------------------------
			//

			void SetAvoidPlayerToTrue() { AvoidPlayer = 1; }
			void SetAvoidPlayerToFalse() { AvoidPlayer = 0; }
			void SetAvoidStateToFalse() { AvoidState = AVOID_STATE_0; }
			void SetAvoidStateToTrue() { AvoidState = AVOID_STATE_1; }

			// ----------------------------------------------------------
			//

			bool		 IsGoingBack()	    const { return State == STOP_CHASE || State == GO_BACK_TO_ROUTE; }
			bool		 IsAvoiding()		const { return AvoidState == AVOID_STATE_1; }

			
		// ----------------------------------------------------------
		//

		void Controller(vml::views::View* view)
		{

			// go back after stopping chase to starting route to replay initial route

			if (State == GO_BACK_TO_ROUTE)
			{
		
			//	Position.x = Path[PathIndex].x + T * PathDir[PathIndex].x;
			//	Position.y = Path[PathIndex].y + T * PathDir[PathIndex].y;
			//	Position.z = Path[PathIndex].z + T * PathDir[PathIndex].z;
			//
			//	GetRootModel()->SetPosition(Position);
			//
			//	T += CruiseSpeed * PathInvSegmentLength[PathIndex];
			//
			//	// decelleration \ accellereation when reaching
			//	// initial position
			//
			//	std::cout << CurPath << std::endl;
			//
			//	if (CurPath < T0) CruiseSpeed += Acc;
			//	if (CurPath > 1 - T0) CruiseSpeed -= Acc;
		//
		//		if (T > 1.0f)
		//		{
		//			CurPath += PathSegmentLength[PathIndex] / PathLenght;
		//			PathIndex++;
		//			T = 0.0f;
		//
		//			if (PathIndex > PathCount - 2)
		//			{
		//				PathIndex = PathCount - 1;
		//				Position.x = Path[PathIndex].x;
		//				Position.y = Path[PathIndex].y;
		//
		//				Position.z = Path[PathIndex].z;
		//
		//				GetRootModel()->SetPosition(Position);
		//				CurOrigin = Position;
		//				State = START_PATH;
		//			}
		//		}
		
			}

			// recoil position

		//	glm::vec3 q = GetRootModel()->GetPosition();
		//	float recoilspeed = 0.05f;
		//	if (GetRootModel()->GetPosition().z < 0)
		//		GetRootModel()->SetPosition(glm::vec3(q.x, q.y, q.z + recoilspeed));

		}

		// ----------------------------------------------------------
		//

		Drone(const std::string& screenname, const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scaling, int priority) : vml::objects::Object3d_2(screenname)
		{
			//	Player					 =  nullptr;
	
			PathIndex = -1;
			PathFinderStartingCellId = -1;
			Priority = priority;
			AvoidPlayer = 0;
			CurPathA = glm::ivec2(-1, -1);
			CurPathB = glm::ivec2(-1, -1);
			Position = glm::vec3(0, 0, 0);
			NearestPathDestPoint = glm::vec3(0, 0, 0);
			NearestPathDir = glm::vec3(0, 0, 0);

			PrevCruiseSpeed = 0.0f;
			//	ChaseSpeed				 =  0.05f;
			//	PreviousSpeed			 =  0.0f;



			//	
		//	T0						 =  0.2f;
		//	Acc						 =  1.0f / 1000.0f;
			//	Firing					 =  false;

			CastRay = false;
			WallHit = {};

			//

		//	UpKeyPressed    = false;
		//	DownKeyPressed  = false;
		//	LeftKeyPressed  = false;
		//	RightKeyPressed = false;

		//	Boost = false;

		//	Yaw       = 0.0f;
		//	Roll      = 0.0f;
		//	Pitch     = 0.0f;
		//	PrevYaw   = 0.0f;
		//	PrevPitch = 0.0f;

		//	DampingIsOver = 0;

		//	TurnSpeed	  = 0.2f * 16.0f;
		//	RollDampening = 0.9f;

		//	ClimbSpeed = 0.1f * 16.0f;
		//	RollSpeed  = 0.2f * 16.0f;

		//	BumpSpeed = 0.05f;

		//	RollAngleLimit	   = 45.0f;			// maximum pitch angle
		//	ClimbAngleLimit	   = 25.0f;		// Angle limit when climbing up or down
		//	BowLimit		   = -15.0f;
		//	BowTau			   = 100.0f;
		//	DampingEps		   = 0.01f;
		//	RotorRotationSpeed = 0.05f;

		//	aT = 0.0f;

		//	TurnState = NEUTRAL_POSE;
			AvoidState = AVOID_STATE_1;

			//	Theta0 = 0.0f;
			//	Theta1 = 0.0f;
			//	ThetaLimit = 45.0f;
			//	BladesSpeed = 5.0f;

			//	BodyModel		  = nullptr;
			//	LowerBodyModel    = nullptr;
			//	SupportShaftModel = nullptr;
			//	Rotor1Model       = nullptr;
			//	Rotor2Model       = nullptr;
			//	GunPivotModel     = nullptr;
			//	GunModel          = nullptr;
			//	Blade1Model       = nullptr;
			//	Blade2Model       = nullptr;
			//	ObjectFrustm			 Frustum;

			//

			// create object

			CreateObject(pos, rot, scaling);

			// add ellipsoid collider

		//	GetRootModel()->AddEllipsoidCollider(glm::vec3(2, 1.5f, 1));

			// attach player

		//	if (player)
		//	{
		//		Player = player;
		//	}
		//	else
		//	{
		//		vml::os::Message::Error("Drone : ", "Null Player");
		//	}
		}


	};

*/