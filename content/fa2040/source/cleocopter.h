#pragma once

namespace fa2040
{
	
	//////////////////////////////////////////////////////////////////
	// player helicopter

	class CleoCopter  : public vml::objects::Object3d_2
	{
		
		private:
			
			// object meshes

			vml::models::Model3d_2* BodyModel;
			vml::models::Model3d_2* CanopyModel;
			vml::models::Model3d_2* JetsModel;
			vml::models::Model3d_2* RotorModel;
			vml::models::Model3d_2* RadarModel;
			vml::models::Model3d_2* TailRotorModel;
			vml::models::Model3d_2* BladesModel;
			vml::models::Model3d_2* LandingGearModel;
			vml::models::Model3d_2* MissileModel[16];
			vml::models::Model3d_2* GunRotorModel;
			vml::models::Model3d_2* GunMountModel;
			vml::models::Model3d_2* GunPivotModel;
			vml::models::Model3d_2* GunModel;
			
			// keys

			unsigned char Keys[1024];							// keys to bind
			
			// animation states

			int   State;										// animation state
			
			int   LandingGearState;								// landing gear animation state
			float LandingGearSpeed;
			float LandingGearMinY;
			float LandingGearMaxY;

			// euler angles

			float Yaw;
			float Roll;
			float Pitch;

			// animation params

			float TurnSpeed;									// yaw turn speed
			float RollAngleLimit;								// pitch angle limit when turining
			float ClimbAngleLimit;								// max inclination when levelling up or down
			float RollDampening;								// return to 0 pitch rate
			float RollSpeed;									// pitch decrease when reaching left or right turining limits

			float PrevYaw;										// store previous angular value used in bumping
			float PrevPitch;									// store previous angular value used in bumping
			bool  DampingIsOver;								// check when harmonic damping is nearly finished
			float DampingEps;									// eps to check if damping is coming to a rest, the lower, the latter the sate will be set to true			
			float BoostFactorA;
			float BoostFactorB;
			float BumpSpeed;
			
			// gun turrest sates / animation parms

			int       GunTurretState;
			float     GunTurretSpeed;
			glm::vec3 GunTarget;

			// boosting animation parms

			bool Boost;

			float T;

			float BowLimit;
			float BowTau;
			float ClimbSpeed;

			glm::vec3 DirectionVector;		// driection vector
			float	  ForceMagnitude;		// resistive viscous force
			float	  RbForceMagnitude;		// resistive viscous force
			glm::vec3 RbForceDirection;		// resisitive force direction
			glm::vec3 Velocity;				// velocity
			glm::vec3 Accelleration;		// accelleration
			float	  Mass;					// Newtonian Mass
			float	  InvMass;				// inverse of mass

			int LastHorzDirectionState;
			int LastVertDirectionState;
			int DirectionState;
			/*
			vml::geo2d::PathFinder* PathFinder;
			int	NavMeshCellId;						// id of cell in the navmesh the navmesh is segmented into 'islands'
													// this value indicates in which of these islands the 
													// cleocopter belongs to see pathfinder for more info
				*/							
			// -------------------------------------------------------------------------
			// state for animation

			static constexpr unsigned int NEUTRAL    = 0;
			static constexpr unsigned int LEFT_POSE  = 1;
			static constexpr unsigned int RIGHT_POSE = 2;
			static constexpr unsigned int BUMP_RIGHT = 3;
			static constexpr unsigned int BUMP_LEFT  = 4;
			static constexpr unsigned int BUMP_DOWN  = 5;
			static constexpr unsigned int BUMP_UP    = 6;
			
			// -------------------------------------------------------------------------
			//

			void ComputeBumpRight()
			{
				static bool init = false;

				if (State == BUMP_RIGHT && !init)
				{
					PrevYaw = Yaw;
					init    = true;
				}

				if (init)
				{
					Yaw += TurnSpeed * BumpSpeed;
					
					if (Yaw > 180.0f)
					{
						Yaw   = 180.0f;
						State = LEFT_POSE;
						T     = 0.0f;
						init  = false;
					}
					else
					{
						if (fabs(Yaw - PrevYaw) > 1.0f)
						{
							State = NEUTRAL;
							T     = 0.0f;
							init  = false;
						}

						Roll += RollSpeed * BumpSpeed;

						//	if (Roll >  RollAngleLimit * 1.5f)
						//		Roll =  RollAngleLimit * 1.5f;

					}
				}
			}

			// -------------------------------------------------------------------------
			//

			void ComputeBumpLeft()
			{
				static bool init = false;

				if (State == BUMP_LEFT && !init)
				{
					PrevYaw = Yaw;
					init    = true;
				}

				if (init)
				{
					Yaw -= TurnSpeed * BumpSpeed;

					if (Yaw < 0)
					{
						Yaw   = 0.0f;
						State = RIGHT_POSE;
						T     = 0.0f;
						init  = false;
					}
					else
					{
						if (fabs(Yaw - PrevYaw) > 1.0f)
						{
							State = NEUTRAL;
							T     = 0.0f;
							init  = false;
						}

						Roll -= RollSpeed * BumpSpeed;

					//	if (Roll < -RollAngleLimit * 1.5f)
					//		Roll = -RollAngleLimit * 1.5f;
					}
				}
			}

			// -------------------------------------------------------------------------
			//

			void ComputeBumpDown()
			{
				static bool init = 0;

				if (State == BUMP_DOWN && !init)
				{
					PrevPitch = Pitch;
					init      = true;
				}

				if (init)
				{
					Pitch -= ClimbSpeed * BumpSpeed * 3.0f;

					if (Pitch < -ClimbAngleLimit)
					{
						Pitch = -ClimbAngleLimit;
						State = NEUTRAL;
						T	  = 0.0f;
						init  = false;
					}
					else
					{
						if (fabs(Pitch - PrevPitch) > 1.0f)
						{
							State = NEUTRAL;
							T	  = 0.0f;
							init  = false;
						}
					}
				}
			}

			// -------------------------------------------------------------------------
			//

			void ComputeBumpUp()
			{
				static bool init = false;

				if (State == BUMP_UP && !init)
				{
					PrevPitch = Pitch;
					init      = true;
				}

				if (init)
				{
					Pitch += ClimbSpeed * BumpSpeed * 3.0f;

					if (Pitch > ClimbAngleLimit)
					{
						Pitch = ClimbAngleLimit;
						State = NEUTRAL;
						T     = 0.0f;
						init  = false;
					}
					else
					{

						if (fabs(Pitch - PrevPitch) > ClimbAngleLimit)
						{
							State = NEUTRAL;
							T     = 0.0f;
							init  = false;
						}
					}
				}
			}

			// -------------------------------------------------------------------------
			// landing gear control, note that the key associated with 
			// the landing gear controller, must be a one time keypress or it won't work
			
			void LandingGearControl()
			{

				if (Keys[LANDING_GEAR_KEY_PRESSED])
				{
					if (LandingGearState == 0)
					{
						LandingGearState = 2;
					}
					else
					{
						if (LandingGearState == 1)
						{
							LandingGearState = 3;
						}
						else
						{
							if (LandingGearState == 2)
							{
								LandingGearState = 3;
							}
							else
							{
								if (LandingGearState == 3)
								{
									LandingGearState = 2;
								}
							}
						}
					}
				}

				if (LandingGearState == 2)
				{
					if (LandingGearModel->GetPosition().y < LandingGearMaxY)
						LandingGearModel->MoveY(LandingGearSpeed);
					else LandingGearState = 1;
				}

				if (LandingGearState == 3)
				{
					if (LandingGearModel->GetPosition().y > LandingGearMinY)
						LandingGearModel->MoveY(-LandingGearSpeed);
					else LandingGearState = 0;
				}
			}
						
			// ---------------------------------------------------------------
			//

			void BumpControl()
			{
				if (State == BUMP_RIGHT) ComputeBumpRight();
				if (State == BUMP_LEFT)  ComputeBumpLeft();
				if (State == BUMP_DOWN)  ComputeBumpDown();
				if (State == BUMP_UP)	 ComputeBumpUp();
			}
			
			// ---------------------------------------------------------------
			//
			
			void GunTurretControl()
			{
				// gun aims to target

				if (GunTurretState == 1)
				{

					// orient gunrotor towrads tagert

					float df = glm::dot(GunTarget - GunRotorModel->GetTransformedPosition(), GunRotorModel->GetRightVector());

					if (df < 0)
						GunRotorModel->AddAngleY(-GunTurretSpeed);
					else
						GunRotorModel->AddAngleY(GunTurretSpeed);

					// orient gin pivot to target

					glm::vec3 v = glm::normalize(GunPivotModel->GetTransformedPosition() - GunTarget);

					df = glm::dot(v, GunPivotModel->GetUpVector());

					if (df < 0)
						GunPivotModel->AddAngleX(-GunTurretSpeed);
					else
						GunPivotModel->AddAngleX(GunTurretSpeed);

					if (GunPivotModel->GetAngles().x < 0)
						GunPivotModel->SetAngleX(0);

				}

				// gun resets to default state

				if (GunTurretState == 0)
				{	
					float gy = GunRotorModel->GetAngles().y;
					float gx = GunPivotModel->GetAngles().x;
		
					if (gy > 0)
						GunRotorModel->AddAngleY(-GunTurretSpeed);
					else if (gy <0)
						GunRotorModel->AddAngleY(GunTurretSpeed);
					else
						GunRotorModel->SetAngleY(0);

					if (gx >0)
						GunPivotModel->AddAngleX(-GunTurretSpeed);
					else if (gx <0)
						GunPivotModel->AddAngleX(GunTurretSpeed);
					else
						GunPivotModel->SetAngleX(0);
				
				}
			}
			
			// -------------------------------------------------------------------------
			//

			void MoveRight()
			{
				if ( Keys[LEFT_SHIFT_KEY_PRESSED] || Keys[RIGHT_SHIFT_KEY_PRESSED])
					Yaw -= TurnSpeed * BoostFactorA * 2.0f;
			
				if (!Keys[LEFT_SHIFT_KEY_PRESSED] || !Keys[RIGHT_SHIFT_KEY_PRESSED])
					Yaw -= TurnSpeed * BoostFactorA;

				if (Yaw < 0.0f)
				{
					Yaw = 0.0f;

					Roll += RollSpeed;

					if (Roll > 0.0f)
					{
						Roll = 0.0f;

						if (DampingIsOver)
							State = RIGHT_POSE;
					}
				}
				else
				{
					Roll -= RollSpeed*2;

				//	if (Roll < -RollAngleLimit)
				//		Roll = -RollAngleLimit;
				}
			}

			// -------------------------------------------------------------------------
			//

			void MoveLeft()
			{
				if (Keys[LEFT_SHIFT_KEY_PRESSED] || Keys[RIGHT_SHIFT_KEY_PRESSED])
					Yaw += TurnSpeed * 2.0f*BoostFactorA;

				if (!Keys[LEFT_SHIFT_KEY_PRESSED] || !Keys[RIGHT_SHIFT_KEY_PRESSED])
					Yaw += TurnSpeed * BoostFactorA;

				if (Yaw > 180.0f)
				{
					Yaw = 180.0f;

					Roll -= RollSpeed;

					if (Roll < 0.0f)
					{
						Roll = 0.0f;

						if (DampingIsOver)
							State = LEFT_POSE;
					}
				}
				else
				{
					Roll += RollSpeed*2;

				//	if (Roll > RollAngleLimit)
				//		Roll = RollAngleLimit;
				}
			}
			
		public:
			
			// -------------------------------------------------------------------------
			// Binding Keys

			static constexpr unsigned int UP_KEY_PRESSED           =  0;
			static constexpr unsigned int DOWN_KEY_PRESSED         =  1;
			static constexpr unsigned int RIGHT_KEY_PRESSED        =  2;
			static constexpr unsigned int LEFT_KEY_PRESSED         =  3;
			static constexpr unsigned int LANDING_GEAR_KEY_PRESSED =  4;
			static constexpr unsigned int FIRE_KEY_PRESSED         =  5;
			static constexpr unsigned int LEFT_SHIFT_KEY_PRESSED   =  6;
			static constexpr unsigned int RIGHT_SHIFT_KEY_PRESSED  =  7;
			static constexpr unsigned int TAB_KEY_PRESSED          =  8;
			static constexpr unsigned int LEFT_CTRL_KEY_PRESSED    =  9;
			static constexpr unsigned int RIGHT_CTRL_KEY_PRESSED   = 10;
			
			// -------------------------------------------------------------------------
			//
			
			const std::string GetStateString()
			{
				switch (State)
				{
					case NEUTRAL    : return "Neutral";			    break;
					case LEFT_POSE  : return "Left Limit Reached";	break;
					case RIGHT_POSE : return "Right Limit Reached"; break;
					case BUMP_RIGHT : return "Bump Right";			break;
					case BUMP_LEFT  : return "Bump Left";			break;
					case BUMP_DOWN  : return "Bump Down";			break;
					case BUMP_UP    : return "Bump Up";			    break;
				}
				return "Unknown State";
			}
			/*
			//------------------------------------------------------------------
			// physics getters

			float GetMass() const { return Mass; }
			float GetForceMagnitude() const { return ForceMagnitude; }
			float GetRbForceMagnitude() const { return RbForceMagnitude; }
			const glm::vec3& GetRbForceDirection() const { return RbForceDirection; }
			const glm::vec3& GetAccelleration() const { return Accelleration; }
			const glm::vec3& GetVelocity() const { return Velocity; }

			//------------------------------------------------------------------
			// physics setters

			void SetMass(float mass) { Mass = mass; }
			void SetVelocity(const glm::vec3& velocity) { Velocity = velocity; }
			void AddVelocity(const glm::vec3& dv) { Velocity += dv*BoostFactorB; }
			*/
			// -------------------------------------------------------------------------
			// clear binding keys 

			void ClearBindingKeys()
			{
				Keys[UP_KEY_PRESSED]		   = 0;
				Keys[DOWN_KEY_PRESSED]		   = 0;
				Keys[RIGHT_KEY_PRESSED]		   = 0;
				Keys[LEFT_KEY_PRESSED]		   = 0;
				Keys[LANDING_GEAR_KEY_PRESSED] = 0;
				Keys[FIRE_KEY_PRESSED]		   = 0;
				Keys[LEFT_SHIFT_KEY_PRESSED]   = 0;
				Keys[RIGHT_SHIFT_KEY_PRESSED]  = 0;
				Keys[TAB_KEY_PRESSED]	       = 0;
				Keys[LEFT_CTRL_KEY_PRESSED]    = 0;
				Keys[RIGHT_CTRL_KEY_PRESSED]   = 0;
			}

			// -------------------------------------------------------------------------
			//

			void SetBindingKeyValue(unsigned int key)
			{
				Keys[key] = true;
			}
			
			// -------------------------------------------------------------------------
			//

			void Controller(vml::views::View* view)
			{
				TurnSpeed        = 3.2f;
				ClimbSpeed       = 1.6f;
				RollDampening    = 0.9f;
				RollSpeed        = 3.2f;
				BowTau           = 100.0f;
				LandingGearSpeed = 0.02f;
				GunTurretSpeed   = 0.02f;
								
				// check if damping is over

				DampingIsOver = Pitch > -DampingEps && Pitch < DampingEps;

				// get direction vector

				float dx = GetForwardVector().x;

				// turn left or right even if both keys are pressed
				// store previous state for both keys

				if (Keys[RIGHT_KEY_PRESSED] && !Keys[LEFT_KEY_PRESSED]) LastHorzDirectionState = 0;
				if (!Keys[RIGHT_KEY_PRESSED] && Keys[LEFT_KEY_PRESSED]) LastHorzDirectionState = 1;

				if (Keys[RIGHT_KEY_PRESSED] && Keys[LEFT_KEY_PRESSED])
				{
					if (LastHorzDirectionState == 0)
					{
						Keys[RIGHT_KEY_PRESSED] = false;
						Keys[LEFT_KEY_PRESSED] = true;
					}

					if (LastHorzDirectionState == 1)
					{
						Keys[RIGHT_KEY_PRESSED] = true;
						Keys[LEFT_KEY_PRESSED] = false;
					}
				}

				// turn up or down even if both keys are pressed
				// store previous state for both keys

				if (Keys[UP_KEY_PRESSED] && !Keys[DOWN_KEY_PRESSED]) LastVertDirectionState = 0;
				if (!Keys[UP_KEY_PRESSED] && Keys[DOWN_KEY_PRESSED]) LastVertDirectionState = 1;

				if (Keys[UP_KEY_PRESSED] && Keys[DOWN_KEY_PRESSED])
				{
					if (LastVertDirectionState == 0)
					{
						Keys[UP_KEY_PRESSED] = false;
						Keys[DOWN_KEY_PRESSED] = true;
					}

					if (LastVertDirectionState == 1)
					{
						Keys[UP_KEY_PRESSED] = true;
						Keys[DOWN_KEY_PRESSED] = false;
					}
				}

				// if tab is pressed and shift is pressed as well
				// then the boost procedure is executed
				// regardless the shiftkey being pressed

				if (Keys[TAB_KEY_PRESSED] && Keys[LEFT_SHIFT_KEY_PRESSED] )
				{
					Keys[LEFT_SHIFT_KEY_PRESSED] = false;
				}

				// boost control 

				if (Keys[TAB_KEY_PRESSED] && !Keys[LEFT_SHIFT_KEY_PRESSED])
				{
					BoostFactorA = 2.0f;
					BoostFactorB = 4.0f;

					if (dx > 0)
					{
						Keys[RIGHT_KEY_PRESSED] = true;
						Keys[LEFT_KEY_PRESSED] = false;
					}
					else
					{
						Keys[RIGHT_KEY_PRESSED] = false;
						Keys[LEFT_KEY_PRESSED] = true;
					}
				}
				else
				{
					BoostFactorA = 1.0f;
					BoostFactorB = 1.0f;
				}

				// set initial direction 

				DirectionVector.x = 0;
				DirectionVector.y = 0;
				DirectionVector.z = 0;

				if (Keys[LEFT_KEY_PRESSED])  DirectionVector.x -= 1;
				if (Keys[RIGHT_KEY_PRESSED]) DirectionVector.x += 1;
				if (Keys[DOWN_KEY_PRESSED])  DirectionVector.y -= 1;
				if (Keys[UP_KEY_PRESSED])    DirectionVector.y += 1;

				// if state is 0 then control 
				// for right and left turn

				if (State == NEUTRAL)
				{
				
					// right rudder control

					if (!Keys[LEFT_KEY_PRESSED] && Keys[RIGHT_KEY_PRESSED])
					{
						MoveRight();
					}
					
					// left rudder control

					if (!Keys[RIGHT_KEY_PRESSED] && Keys[LEFT_KEY_PRESSED])
					{
						MoveLeft();
					}

					// Pitch control , Climbing Up

					if (Keys[UP_KEY_PRESSED] && !Keys[DOWN_KEY_PRESSED])
					{
						Pitch += ClimbSpeed;

						if (Pitch > ClimbAngleLimit)
							Pitch = ClimbAngleLimit;
					}

					// Pitch control , Climbing Down

					if (!Keys[UP_KEY_PRESSED] && Keys[DOWN_KEY_PRESSED])
					{
						Pitch -= ClimbSpeed;

						if (Pitch < -ClimbAngleLimit)
							Pitch = -ClimbAngleLimit;
					}
					
				}
				
				// right direction

				if (State == RIGHT_POSE)
				{
					// if right key is pressed , then perform a forward bow

					if (Keys[RIGHT_KEY_PRESSED])
					{
						Pitch = BowLimit - BowLimit * exp(-T * BoostFactorB / BowTau);

						T += 1.0f;
					}

					// if right key is released, reset forward bow timer

					if (!Keys[RIGHT_KEY_PRESSED])
					{
						State = NEUTRAL;
						T = 0.0f;
					}

					// if these keys are pressed, go back to neutral state

					if (Keys[RIGHT_KEY_PRESSED] && Keys[UP_KEY_PRESSED])
					{
						State = NEUTRAL;
						T = 0.0f;
					}

					if (Keys[RIGHT_KEY_PRESSED] && Keys[DOWN_KEY_PRESSED])
					{
						State = NEUTRAL;
						T = 0.0f;
					}

				}

				// left direction

				if (State == LEFT_POSE)
				{

					// if left key is pressed , then perform a forward bow

					if (Keys[LEFT_KEY_PRESSED])
					{
						Pitch = BowLimit - BowLimit * exp(-T * BoostFactorB / BowTau);
						T += 1.0f;
					}

					// if left key is relese, reset forward bow timer

					if (!Keys[LEFT_KEY_PRESSED])
					{
						State = NEUTRAL;
						T = 0.0f;
					}

					// if these keys are pressed, go back to neutral state

					if (Keys[LEFT_KEY_PRESSED] && Keys[UP_KEY_PRESSED])
					{
						State = NEUTRAL;
						T = 0.0f;
					}

					if (Keys[LEFT_KEY_PRESSED] && Keys[DOWN_KEY_PRESSED])
					{
						State = NEUTRAL;
						T = 0.0f;
					}

				}

				// damp accumulated rool and pitch angles

				Roll  *= RollDampening;
				Pitch *= RollDampening;
				
				// bump controller

				BumpControl();

				// landing gear controller

				LandingGearControl();

				// gun turret conroller

				GunTurretControl();
				
				// compute forces

				ComputeForces();
				
			}
			
			// -------------------------------------------------------------------
			//

	//		glm::ivec2 GetNavMeshCell()
	//		{
	//			if (PathFinder) 
	//			{
	//				glm::ivec2 ci = PathFinder->GetCellIndicesFromPoint(GetBodyModel()->GetPosition());
	//				if (PathFinder->GetCellIdFromIndices(ci.x, ci.y) == -1)
	//				{
	//					// NavMeshCellId its the isle in the navmesh bitmap where originally the cleocopter started
	//					return PathFinder->FindNearestCellToAnotherCell(ci.x, ci.y, NavMeshCellId);
	//				}
	//			}
	//			return glm::ivec2(-1,-1);
	//		}
			
			// -------------------------------------------------------------------
			//
			
			void Bump(const glm::vec3 &mtvNormal) 
			{
				if (mtvNormal.x > 0) State = BUMP_LEFT;
				if (mtvNormal.x < 0) State = BUMP_RIGHT;
				if (mtvNormal.y > 0) State = BUMP_UP;
				if (mtvNormal.y < 0) State = BUMP_DOWN;
			}
			
			// -------------------------------------------------------------------
			//

			void ComputeForces()
			{
				
	//			ForceMagnitude	   = 25;
	//			RbForceMagnitude   = 2.5f;
				
	//			glm::vec3 totalacc = glm::vec3(0, 0, 0);
							
				// viscous direction is the opposite of velocity

	//			RbForceDirection = -Velocity;
				
				// if left shoft key is not pressed then compute accelleration due to forces
				// if leftkey is pressed perform a rapid in place turn 

//				if (!LeftShiftKeyPressed)
//				{

			//		bool keypressed = UpKeyPressed || DownKeyPressed || LeftKeyPressed || RightKeyPressed;

					// normalize direction vector this is due to the fact that direction vector 
					// is the result of different sum of base vectors (1,0,0), (0,1,0), (0,0,1) 
					// and negative directions as well

		//			float denum = DirectionVector.x * DirectionVector.x + DirectionVector.y * DirectionVector.y + DirectionVector.z * DirectionVector.z;

					// this never happens
					
				//	if (denum > -vml::math::EPSILON && denum < vml::math::EPSILON)
				//	{
				//		DirectionVector.x = 0;
				//		DirectionVector.y = 0;
				//		DirectionVector.z = 0;
				//	}
				//	else
		//			{
//						denum = 1.0f / denum;

	//					DirectionVector.x *= denum;
	//					DirectionVector.y *= denum;
	//					DirectionVector.z *= denum;
	//				}

					// detect keypress

	//				if (keypressed)
	//				{
						// compute accelleration due to propulsion force and viscous force

	//					totalacc = (DirectionVector * ForceMagnitude * BoostFactorB + RbForceMagnitude * RbForceDirection) * InvMass;
	//				}
	//				else
	//				{
						// comptue decelleration due to viscous force
//
	//					totalacc = RbForceMagnitude * RbForceDirection * InvMass;
	//				}
//
	//			}
	//			else
	//			{
	//				// comptue decelleration due to viscous force
	//			
	//				totalacc = RbForceMagnitude * RbForceDirection * InvMass;
	//			}
				//
				// integrate
//
	//			float dt = 1.0f / 60.0f;
//
		//		Velocity += totalacc * dt ;
				//
				// apply changes to root model
//
	//			GetBodyModel()->Move(Velocity * dt);
				
				BodyModel->SetAngles(glm::vec3(Roll, Pitch, Yaw));
			}
			
			// -------------------------------------------------------------------
			//
			
	//		void ComputeGunHitPoint(glm::vec3 &guntip, glm::vec3 &gundir)
	//		{
	//			gundir = GunModel->GetForwardVector();
	//			guntip = GunModel->GetTransformedPosition() + 0.2f * GunModel->GetForwardVector() + 0.02f * GunModel->GetUpVector();
	//		}
			
			// -------------------------------------------------------------------
			//
		
			void ActivateGunTurret()
			{
				switch(GunTurretState)
				{
					case  0: GunTurretState = 1; return; break;
					case  1: GunTurretState = 0; return; break;
				}
			}

			// -------------------------------------------------------------------

			void SetGunTurretTarget(const glm::vec3& tgt)
			{
				GunTarget = tgt;
			}
					
			// -------------------------------------------------------------------
			//

			vml::models::Model3d_2* GetBodyModel() const { return BodyModel; }
			vml::models::Model3d_2* GetBladesModel() const { return BladesModel; }
			vml::models::Model3d_2* GetLandingGearModel() const { return LandingGearModel; }
			vml::models::Model3d_2* GetGunRotorModel() const { return GunRotorModel; }

	//		int GetNavMeshCellId() const { return NavMeshCellId; }

			// -------------------------------------------------------------------
			// Object creation

			void CreateObject(const std::string& sourcepath, const glm::vec3 &pos, const glm::vec3& rot, const glm::vec3& scaling,bool landinggearup=true)
			{

				AddModel("Body",
						 sourcepath + "body.3df",
						 glm::vec3(0, 0, 0),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 { vml::models::POLAR });
				
				AddModel("Canopy",
						 sourcepath + "canopy.3df",
						 glm::vec3(0.051f, 2.274f, 9.42f),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 { vml::models::EULER });
				
				AddModel("Jets",
						 sourcepath + "jets.3df",
						 glm::vec3(0.082f, 3.046f, 1.713f),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 { vml::models::EULER });
				
				AddModel("Rotor",
						 sourcepath + "main_rotor_shaft.3df",
						 glm::vec3(0.1f, 4.72001f, 4.7f),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 { vml::models::EULER });
				
				AddModel("Radar",
						 sourcepath + "radar.3df",
						 glm::vec3(0.1f, 5.74003f, 4.7f),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 { vml::models::EULER });
				
				AddModel("TailRotor",
						 sourcepath + "tail_rotor.3df",
						 glm::vec3(0.1, 0.88f, -12.4f),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 { vml::models::EULER });
				
				AddModel("Blades",
						 sourcepath + "mainrotor.3df",
						 glm::vec3(0.05f, 0.19f, -0.56f),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 { vml::models::EULER });
				
				AddModel("Stand",
						 sourcepath + "stand.3df",
						 glm::vec3(0.05f, -2.5f, 1.72f),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 { vml::models::EULER });
				
				AddModel("GunRotor",
						 sourcepath + "gunrotor.3df",
						 glm::vec3(0.12f, -0.5f, 8.2301f),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 { vml::models::EULER });
				
				AddModel("GunPivot",
						 sourcepath + "gunpivot.3df",
						 glm::vec3(-0.02f, -0.22f, -0.12f),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 { vml::models::EULER });
				
				AddModel("Gun",
						 sourcepath + "gun.3df",
						 glm::vec3(0.04f, -0.24f, 1.52f),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 { vml::models::EULER });
				
				// first missle group

				AddModel("Missile0",
						 sourcepath + "missile.3df",
						 glm::vec3(4.44f, -1.02f, 0.8f),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 { vml::models::EULER });
				
				AddModel("Missile1",
						 sourcepath + "missile.3df",
						 glm::vec3(4.44f, -0.5f, 0.8f),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 { vml::models::EULER });
				
				AddModel("Missile2",
						 sourcepath + "missile.3df",
						 glm::vec3(3.94f, -0.5f, 0.8f),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 { vml::models::EULER });
				
				AddModel("Missile3",
						 sourcepath + "missile.3df",
						 glm::vec3(3.94f, -1.02f, 0.8f),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 { vml::models::EULER });
				
				// second missile group

				AddModel("Missile4",
						 sourcepath + "missile.3df",
						 glm::vec3(2.85f, -0.58f, 2.5f),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 { vml::models::EULER });

				AddModel("Missile5",
						 sourcepath + "missile.3df",
						 glm::vec3(2.85f, -0.05f, 2.5f),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
				 		 { vml::models::EULER });

				AddModel("Missile6",
						 sourcepath + "missile.3df",
						 glm::vec3(2.31f, -0.05f, 2.5f),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 { vml::models::EULER });

				AddModel("Missile7",
						 sourcepath + "missile.3df",
						 glm::vec3(2.31f, -0.58f, 2.5f),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 { vml::models::EULER });
				
				// third missile group
				
				AddModel("Missile8",
						 sourcepath + "missile.3df",
						 glm::vec3(-2.31f, -0.58f, 2.5f),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 { vml::models::EULER });

				AddModel("Missile9",
						 sourcepath + "missile.3df",
						 glm::vec3(-2.31f, -0.05f, 2.5),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 { vml::models::EULER });

				AddModel("Missile10",
						 sourcepath + "missile.3df",
						 glm::vec3(-2.85f, -0.05f, 2.5f),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 { vml::models::EULER });

				AddModel("Missile11",
						 sourcepath + "missile.3df",
						 glm::vec3(-2.85f, -0.58f, 2.5f),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 { vml::models::EULER });
				
				// forth missile group

				AddModel("Missile12",
						 sourcepath + "missile.3df",
						 glm::vec3(-3.94f, -1.02f, 0.8f),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 { vml::models::EULER });

				AddModel("Missile13",
						 sourcepath + "missile.3df",
						 glm::vec3(-3.94f, -0.5f, 0.8f),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 { vml::models::EULER });

				AddModel("Missile14",
						 sourcepath + "missile.3df",
						 glm::vec3(-4.44f, -0.5f, 0.8f),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 { vml::models::EULER });

				AddModel("Missile15",
						 sourcepath + "missile.3df",
						 glm::vec3(-4.44f, -1.02f, 0.8f),
						 glm::vec3(0, 0, 0),
						 glm::vec3(1, 1, 1),
						 { vml::models::EULER });
				
				// link models
				
				Link("Body", "Canopy");
				Link("Body", "Jets");
				Link("Body", "Rotor");
				Link("Body", "Radar");
				Link("Body", "TailRotor");
				Link("Body", "Stand");
				Link("Rotor", "Blades");
				Link("Body", "GunRotor");
				Link("GunRotor" , "GunPivot");
				Link("GunPivot" , "Gun");
				Link("Body" , "Missile0");
				Link("Body" , "Missile1");
				Link("Body" , "Missile2");
				Link("Body" , "Missile3");
				Link("Body" , "Missile4");
				Link("Body" , "Missile5");
				Link("Body" , "Missile6");
				Link("Body" , "Missile7");
				Link("Body" , "Missile8");
				Link("Body" , "Missile9");
				Link("Body" , "Missile10");
				Link("Body" , "Missile11");
				Link("Body" , "Missile12");
				Link("Body" , "Missile13");
				Link("Body" , "Missile14");
				Link("Body" , "Missile15");
				
				// finalize object

				Finalize(pos, rot, scaling);
				
				// get models pointers
				
				BodyModel        = GetModelAt("cleocopter/body");
				CanopyModel      = GetModelAt("cleocopter/canopy");
				JetsModel        = GetModelAt("cleocopter/jets");
				RotorModel		 = GetModelAt("cleocopter/rotor");
				RadarModel		 = GetModelAt("cleocopter/radar");
				TailRotorModel   = GetModelAt("cleocopter/tailrotor");
				BladesModel      = GetModelAt("cleocopter/blades");
				LandingGearModel = GetModelAt("cleocopter/stand");
				GunRotorModel	 = GetModelAt("cleocopter/gunrotor");
				GunPivotModel	 = GetModelAt("cleocopter/gunpivot");
				GunModel		 = GetModelAt("cleocopter/gun");
				MissileModel[ 0] = GetModelAt("cleocopter/missile0");
				MissileModel[ 1] = GetModelAt("cleocopter/missile1");
				MissileModel[ 2] = GetModelAt("cleocopter/missile2");
				MissileModel[ 3] = GetModelAt("cleocopter/missile3");
				MissileModel[ 4] = GetModelAt("cleocopter/missile4");
				MissileModel[ 5] = GetModelAt("cleocopter/missile5");
				MissileModel[ 6] = GetModelAt("cleocopter/missile6");
				MissileModel[ 7] = GetModelAt("cleocopter/missile7");
				MissileModel[ 8] = GetModelAt("cleocopter/missile8");
				MissileModel[ 9] = GetModelAt("cleocopter/missile9");
				MissileModel[10] = GetModelAt("cleocopter/missile10");
				MissileModel[11] = GetModelAt("cleocopter/missile11");
				MissileModel[12] = GetModelAt("cleocopter/missile12");
				MissileModel[13] = GetModelAt("cleocopter/missile13");
				MissileModel[14] = GetModelAt("cleocopter/missile14");
				MissileModel[15] = GetModelAt("cleocopter/missile15");
								
				// copter starts with landing gear retracted

				LandingGearMinY = LandingGearModel->GetPosition().y;
				LandingGearMaxY = 0.93f*scaling.y;

				// landing gear up
				
				if (landinggearup)
				{
					LandingGearState = 1;
					LandingGearModel->MoveY(LandingGearMaxY - LandingGearMinY);
				}
				else
				{
					LandingGearState = 0;
				}
			}
			
			// ---------------------------------------------------
			// ctor / dtor

			CleoCopter(/*vml::geo2d::PathFinder* pathfinder,*/ 
				       const std::string& sourcepath, 
				       const std::string& screenname, 
				       vml::handlers::Scene* scene, 
				       const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scaling) : vml::objects::Object3d_2(screenname)
			{
				BodyModel        = nullptr;
				CanopyModel      = nullptr;
				JetsModel        = nullptr;
				RotorModel       = nullptr;
				RadarModel       = nullptr;
				TailRotorModel   = nullptr;
				BladesModel      = nullptr;
				LandingGearModel = nullptr;
				GunRotorModel    = nullptr;
				GunMountModel    = nullptr;
				GunPivotModel    = nullptr;
				GunModel         = nullptr;
				MissileModel[ 0] = nullptr;
				MissileModel[ 1] = nullptr;
				MissileModel[ 2] = nullptr;
				MissileModel[ 3] = nullptr;
				MissileModel[ 4] = nullptr;
				MissileModel[ 5] = nullptr;
				MissileModel[ 6] = nullptr;
				MissileModel[ 7] = nullptr;
				MissileModel[ 8] = nullptr;
				MissileModel[ 9] = nullptr;
				MissileModel[10] = nullptr;
				MissileModel[11] = nullptr;
				MissileModel[12] = nullptr;
				MissileModel[13] = nullptr;
				MissileModel[14] = nullptr;
				MissileModel[15] = nullptr;

				Boost = false;

				Yaw       = 0.0f;
				Roll      = 0.0f;
				Pitch     = 0.0f;
				PrevYaw   = 0.0f;
				PrevPitch = 0.0f;

				TurnSpeed	  =  0.25f;
				ClimbSpeed	  =  0.09f;
				RollDampening =  0.995f;					// return to 0 roll speed rate
				RollSpeed	  =  0.1f;
				BumpSpeed	  =  2.0f;

				RollAngleLimit	=  45.0f;					// maximum pitch angle
				ClimbAngleLimit =  25.0f;					// Angle limit when climbing up or down

				BowLimit = -15.0f;
				BowTau	 =  1.0f;
				T		 =  0.0f;

				State                  = NEUTRAL;
				LastHorzDirectionState = 0;
				LastVertDirectionState = 0;
				LandingGearState       = 0;
				GunTurretState	       = 0;
				DirectionState		   = 0;
				DampingIsOver          = 0;
			
				DampingEps			   = 0.01f;
				BoostFactorA		   = 1.0f;
				BoostFactorB		   = 1.0f;
				LandingGearSpeed	   = 0.005f;
				GunTurretSpeed		   = 0.00125f;
				LandingGearMinY		   = 0.0f;
				LandingGearMaxY		   = 0.0f;

				ForceMagnitude		   = 1.5f;								
				RbForceMagnitude	   = 1.0f;								
				Mass				   = 1.0f;
				InvMass				   = 1.0f / Mass;
				RbForceDirection	   = glm::vec3(0, 0, 0);
				Accelleration		   = glm::vec3(0, 0, 0);
				Velocity			   = glm::vec3(0, 0, 0);
				DirectionVector		   = glm::vec3(1, 0, 0);

		//		NavMeshCellId		   = -1;
		//		PathFinder			   =  nullptr;

				// clear keys

				memset(Keys, 0, sizeof(unsigned char) * 1024);
				
				// create object
				
				CreateObject(sourcepath, pos, rot, scaling);
				
				// * Important * 
				// get copter position to retrieve navmesh cell index 
				// for the pathfiunder in the case due to navmesh segmentation
				// the player could intercept a non traversable cell 
				// this interrupting the chase function for drones
				// each isolated region in the navmesh bitmap has a value
				// assigned, we need to know in which of these region 
				// the cleocopter is
		//		
		//		if (pathfinder)
		//		{
		//			PathFinder = pathfinder;
		//			glm::ivec2 ci = PathFinder->GetCellIndicesFromPoint(pos);
		//			NavMeshCellId = PathFinder->GetCellIdFromIndices(ci.x, ci.y);
		//		}
		//		else
		//		{
		//			vml::os::Message::Error("CleoCopter :" ,"pathfinder pointer is null");
		//		}
				
			}

			~CleoCopter()
			{
			}
			
	};
	
}
