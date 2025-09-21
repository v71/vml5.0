#pragma once

//////////////////////////////////////////////////////////////////////////////////////////////

//#include <vml2.0/content/fa2040/src/drone.h>
//#include <chrono>

namespace fa2040
{

	class DroneArbiter
	{
		
		private:
		
			vml::models::Model3d_2* Player;
			std::vector<fa2040::Drone*> Drones;
			/*
			int								      State;
			int								      TimerCounter;
			int								      TimerState;
			std::chrono::steady_clock::time_point TimerStart;
			std::chrono::steady_clock::time_point TimerEnd;
			
			// ----------------------------------------------------------
			//
			
			static const int DRONE_NO_HITS	   = 0;
			static const int DRONE_HITS_PLAYER = 1;
			static const int DRONE_HITS_WALL   = 2;
			  			
			float ChaseRange;
			int	  CurrentDroneId;
			int	  PreviousDroneId;
			float ZDepth;
			*/
			// ----------------------------------------------------------
			//
			/*
			int FindNearestDrone(vml::views::View* view, fa2040::CleoCopter* player)
			{
				float distmin;
				float denum;
				int minid;
				fa2040::Drone* dronei;
				glm::vec3 dir;

				distmin = FLT_MAX;
				minid = -1;

				glm::vec3 playerpos = player->GetRootModel()->GetPosition();

				for (size_t i = 0; i < Drones->size(); ++i)
				{
					dronei = Drones->at(i);
					dir = playerpos - dronei->GetRootModel()->GetPosition();
					denum = dir.x * dir.x + dir.y * dir.y + dir.z * dir.z;
					if (denum < distmin) { distmin = denum; minid = (int)i; }
				}

				if (minid != -1)
				{
					if (distmin < ChaseRange)
					{
						dronei = Drones->at(minid);
//						vml::DebugRender->DrawLine(view, playerpos, dronei->GetRootModel()->GetTransformedPosition(), vml::colors::Red, true);
						return minid;
					}
				}

				return -1;
			}
			*/

			// ----------------------------------------------------------
			//
			/*
			int CheckDroneVsPlayerVsWallCollision(vml::views::View* view, vml::scenes::Scene* scene, fa2040::Drone* drone,glm::vec3 &q )
			{
				glm::vec3 playerpos = Player->GetRootModel()->GetPosition();
				float playerradius  = Player->GetRadius();
				glm::vec3 dronepos  = drone->GetRootModel()->GetPosition();
				glm::vec3 dir		= playerpos - dronepos;
				float denum			= sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
				
				// if dir is nearly 0, exit , this means that the distance is very close
				// thus leading to a 0 division

				if (denum < vml::math::EPSILON)
					return DRONE_NO_HITS;

				glm::vec3 unitdir = glm::vec3(dir.x, dir.y, dir.z) / denum;

				// nvidia physx raycasting against level walls
				
				physx::PxRaycastBuffer hit;

				bool status = scene->RayCastClosest(dronepos, unitdir, hit);

				// status is always true since the ray starting from dron will always hit some wall

				if (status)
				{
					q = glm::vec3(hit.block.position.x, hit.block.position.y, hit.block.position.z);
					//	vml::DebugRender->DrawLine(view, dronepos, wallp, vml::colors::Red, true);
				}

				// hard coded sphere - ray intersection 
				// where the ray starting point is dronepos, end pos is playerpos
				// the sphre position is playerpos with playerradius
				// sphere and ray *always* do collide
				// 
				//	glm::vec3 p = dronepos + (1 - playerradius * invdenum) * dir;

				glm::vec3 dp0 = q - dronepos;
				float d0 = dp0.x * dp0.x + dp0.y * dp0.y + dp0.z * dp0.z;

				//  glm::vec3 p = dronepos + (1 - playerradius / denum) * dir;
				//	glm::vec3 dp1 = p - dronepos;
				//	float d1 = dp1.x * dp1.x + dp1.y * dp1.y + dp0.z * dp0.z;

				// hard coded result

				//	float d1 = denum* ( denum - playerradius );
				//	if (d0> d1)

				// drone ray hits player

				if (d0 > denum * (denum - playerradius))
				{
					q= dronepos + (1 - playerradius / denum) * dir;

					//	vml::DebugRender->DrawLine(view, dronepos, q, vml::colors::Yellow, true);
					//	vml::DebugRender->DrawPoint(view, q, vml::colors::Green);
				
					return DRONE_HITS_PLAYER;
				}

				// drone ray hits wall
				 
				//	vml::DebugRender->DrawLine(view, dronepos, q, vml::colors::Yellow, true);
				//	vml::DebugRender->DrawPoint(view, wallp, vml::colors::Green);
				
				return DRONE_HITS_WALL;
			}
			*/
		public:
			
			// ----------------------------------------------------------
			//
			/*
			void Mutex(vml::views::View* view, vml::handlers::Scene *scene)
			{
				
				// vml::DebugRender->DrawSphere(view, dronei, vml::colors::Red);
				//	vml::DebugRender->DrawSphere(view, Player, vml::colors::Red);

				fa2040::Drone* dronei=nullptr;
				fa2040::Drone* dronej=nullptr;
				float spehreepsilon = 1.5f;
				size_t n = Drones->size();
				
				// check if playter is near a drone , if it is so
				// compute which drone has greater priortiy and sei it
				// in chase mode, release from chase mode the lower id drone
				
				if (State == 0)
				{
					
					int minid = FindNearestDrone(view, Player);

					if (minid != -1)
					{
						dronei=Drones->at(minid);

						int dronestate = dronei->GetState();

						if ( dronestate == fa2040::Drone::START_FORWARD_PATROL ||
							 dronestate == fa2040::Drone::START_BACKWARD_PATROL ||
							 dronestate == fa2040::Drone::GO_BACK_TO_ROUTE )
						{
							CurrentDroneId = minid;
							PreviousDroneId = -1;

							// nearest drone has higher priority over others

							int tp = Drones->at(0)->Priority;
							Drones->at(0)->Priority = Drones->at(CurrentDroneId)->Priority;
							Drones->at(CurrentDroneId)->Priority = tp;

							State = 1;
						}
					}
					
				}
				
				if (State == 1)
				{
					
					// find nearest drone to player and
					// raise its level of priority

					dronei= Drones->at(CurrentDroneId);
					dronei->ChasePlayer();

					int minid = FindNearestDrone(view, Player);

					if (minid != -1)
					{
						if (minid != CurrentDroneId)
						{

							fa2040::Drone* dronej = Drones->at(minid);

							int dronestate = dronej->GetState();

							if (dronestate == fa2040::Drone::START_FORWARD_PATROL ||
								dronestate == fa2040::Drone::START_BACKWARD_PATROL ||
								dronestate == fa2040::Drone::GO_BACK_TO_ROUTE )
							{
								
								PreviousDroneId = CurrentDroneId;
								CurrentDroneId = minid;
								Drones->at(PreviousDroneId)->StopChase();
								State = 0;

							//	vml::DebugRender->DrawLine(view, playerpos, dronej->GetRootModel()->GetTransformedPosition(), vml::colors::Yellow, true);

							}
						}

					}
					
					// if drone is chasing player, then check if the line of sight
					// hits the player of a level wall

					if (dronei->IsChasingPlayer())
					{

						glm::vec3 dronepos = dronei->GetRootModel()->GetPosition();
						glm::vec3 p;

						int result = CheckDroneVsPlayerVsWallCollision(view, scene, dronei, p);

						if (result == DRONE_HITS_WALL)
						{
							vml::DebugRender->DrawLine(view, dronepos, p, vml::colors::Yellow, true);
							vml::DebugRender->DrawPoint(view, p, vml::colors::Green);
						
							if (TimerState == 0)
							{
								TimerCounter = 0;
								TimerState = 1;
								TimerStart = std::chrono::high_resolution_clock::now();
							}

							if (TimerState == 1)
							{
								TimerCounter++;
								TimerEnd = std::chrono::high_resolution_clock::now();
								
								//	std::cout << std::chrono::duration_cast<std::chrono::seconds>(TimerEnd - TimerStart).count() << '\n';
									
								if (std::chrono::duration_cast<std::chrono::seconds>(TimerEnd - TimerStart).count() > 5)
								{
									//	std::cout << "Do the theng" << std::endl;

									PreviousDroneId = CurrentDroneId;
									CurrentDroneId = minid;
									Drones->at(PreviousDroneId)->StopChase();
									State = 0;
									TimerState = 0;
								}
							}
							
						}

						if (result == DRONE_HITS_PLAYER)
						{
							vml::DebugRender->DrawLine(view, dronepos, p, vml::colors::Red, true);
							vml::DebugRender->DrawPoint(view, p, vml::colors::Green);
						}
					
					}
					
			//		std::cout << TimerCounter << std::endl;

				}
					
				// too many glitches
				
				// drone avoidnace handling the drone with higher priority
				// displaces the otehr
			
				for (size_t i = 0; i < n; ++i)
					Drones->at(i)->SetAvoidStateToFalse();

				for (size_t i = 0; i < n; ++i)
				{
					
					// cache drone params

					dronei = Drones->at(i);

					int pi		   = dronei->GetPriority();
					glm::vec3 posi = dronei->GetRootModel()->GetPosition();
					float ri	   = dronei->GetRadius() * spehreepsilon;
					
			//		std::cout << dronei->GetRootModel()->GetScreenName() << " , Priority : " << pi << std::endl;

					for (size_t j = 0; j < n; ++j)
					{

						if (i != j)
						{
							
							// cache secondo dorne params

							dronej = Drones->at(j);
			
							int pj		   = dronej->GetPriority();
							glm::vec3 posj = dronej->GetRootModel()->GetPosition();
							float rj	   = dronej->GetRadius() * spehreepsilon;
														
							// check drone bounding sphere collision

							glm::vec3 dir = posj - posi;
							float denum = sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
							float disp = denum - ri - rj;

							// if they collided

							if (disp < 0)
							{
							
								//	vml::DebugRender->DrawSphere(view, dronei, vml::colors::Red);
								//	vml::DebugRender->DrawSphere(view, dronej, vml::colors::Red);

								// check drone priority

								if (pi < pj)
								{
									dronej->SetAvoidStateToTrue();
								//	dronei->GetRootModel()->SetPosition(glm::vec3(posi.x, posi.y, posi.z - disp));
									dronej->GetRootModel()->SetPosition(glm::vec3(posj.x, posj.y, posj.z + disp));
								}

							}
							
						}
						
					}
										
				}
			
				// reposition drone to origina z position if needed
				// due to the fact that a small error might accumulate
				// during the avaoiding stage

				for (size_t i = 0; i < n; ++i)
				{
					dronei = Drones->at(i);

					if (!dronei->IsAvoiding())
					{ 
						vml::models::Model3d_2* model = dronei->GetRootModel();

						float z0 = model->GetTransformedPosition().z-ZDepth;
				
					//	std::cout << model->GetScreenName() << " " << z0 << " " << ZDepth << " " << ZDepth - z0 << std::endl;

						if (z0>0)
						{
							model->MoveZ(z0);
						}
					}
				}

			}
			
			// ----------------------------------------------------------
			//
			
			void Init(fa2040::CleoCopter* player, const std::vector<fa2040::Drone*> *drones ,const float zdepth)
			{
			//	if (!player)
			//		vml::os::Message::Error("DroneArbiter :","cannot attach null player");

			//	Player = player;
				Drones = drones;
				ZDepth = zdepth;
				std::cout << "Started Drone Arbiter" << std::endl;

			}
			*/
			// ----------------------------------------------------------
			//
			/*
			void RestartRoutes()
			{
				CurrentDroneId = -1;
				PreviousDroneId = -1;
				State = 0;
				for (size_t i = 0; i < Drones->size(); ++i)
					Drones->at(i)->StartCourse();
			}
			*/

			// ----------------------------------------------------------
			//
			
			fa2040::Drone* GetDroneAt(size_t pos)
			{
				return Drones[pos];
			}

			// ----------------------------------------------------------
			//

			void Clear()
			{
				for (auto it = Drones.begin(); it != Drones.end(); ++it)
					vml::os::SafeDelete(*it);
				Drones.clear();
			}

			// ----------------------------------------------------------
			//

			void AttachPlayer(vml::models::Model3d_2* player)
			{
				if (!player)
					vml::os::Message::Error("Player pointer is null");
				Player = player;
			}

			// ----------------------------------------------------------
			//
			
			void AttachDrone(fa2040::Drone* drone)
			{
				if (!drone)
					vml::os::Message::Error("Drone pointer is null");
				Drones.emplace_back(drone);
			}

			// ----------------------------------------------------------
			// ctor / dtor

			DroneArbiter()
			{
				
				Player          = nullptr;
			/*
				Drones          = nullptr;
				State           = 0;
				TimerCounter    = 0;
				TimerState      = 0;
				ChaseRange      =  100.0f;
				State	        =  0;
				CurrentDroneId  = -1;
				PreviousDroneId = -1;
				ZDepth			=  0.0f;
			*/
			}

			~DroneArbiter()
			{
				for (auto it = Drones.begin(); it != Drones.end(); ++it)
					vml::os::SafeDelete(*it);
			}

	};
}

