#pragma once

#include <vml5.0/opengl/effects/overlay.h>
#include <vml5.0/handlers/overlayhandler.h>

namespace fa2040
{
	
	//////////////////////////////////////////////////////////////////
	// player helicopter

	class CleoCopter_2  
	{
		
		private:
			
			std::string ScreenName;

			// object meshes

			vml::models::Model3d_2* BodyModel;
			vml::models::Model3d_2* CanopyModel;
			vml::models::Model3d_2* JetsModel;
			vml::models::Model3d_2* RotorModel;
			vml::models::Model3d_2* RadarModel;
			vml::models::Model3d_2* TailRotorModel;
			vml::models::Model3d_2* BladesModel;
			vml::models::Model3d_2* LandingGearModel;
			vml::models::Model3d_2* GunRotorModel;
			vml::models::Model3d_2* GunMountModel;
			vml::models::Model3d_2* GunPivotModel;
			vml::models::Model3d_2* GunModel;
			vml::models::Model3d_2* MissileModel[16];

			// overlays

			vml::overlays::EffectOverlay* BladesOverlay;
			vml::overlays::EffectOverlay* GunMuzzleFlashOverlay;
			vml::overlays::EffectOverlay* JetFlame1Overlay;
			vml::overlays::EffectOverlay* JetFlame2Overlay;
			vml::overlays::EffectOverlay* BulletOverlays[16];
			vml::overlays::EffectOverlay* MissileOverlays[16];

			// keys

			unsigned char Keys[1024] = { 0 };		// keys to bind

			// main controller

			int		  State;						// animation state
			float	  Yaw;							// yaw rotation angle
			float	  Roll;							// roll rotation angle
			float	  Pitch;						// pitch rotation angle
			float	  TurnSpeed;					// yaw turn speed
			float	  RollAngleLimit;				// pitch angle limit when turining
			float	  ClimbAngleLimit;				// max inclination when levelling up or down
			float	  RollDampening;				// return to 0 pitch rate
			float	  RollSpeed;					// pitch decrease when reaching left or right turining limits
			float	  PrevYaw;						// store previous angular value used in bumping
			float	  PrevPitch;					// store previous angular value used in bumping
			bool	  DampingIsOver;				// check when harmonic damping is nearly finished
			float	  DampingEps;					// eps to check if damping is coming to a rest, the lower, the latter the sate will be set to true			
			
			float	  BumpSpeed;					// speed at wich the ccopter bumps into walls
			bool	  Bumping;

			float	  ClimbSpeed;					// Climb Speed
			
			glm::vec3 DirectionVector;				// Driection vector
			int		  LastHorzDirectionState;		// horizontal animation state 
			int		  LastVertDirectionState;		// vertical animation state
			int		  DirectionState;

			float	  ForceMagnitude;				// Resistive viscous force
			float	  RbForceMagnitude;				// Resistive viscous force
			glm::vec3 RbForceDirection;				// Resisitive force direction
			glm::vec3 Velocity;						// Velocity
			glm::vec3 Accelleration;				// Accelleration
			float	  Mass;							// Newtonian Mass
			float	  InvMass;						// Inverse of mass

			// landing gear controller

			int   LandingGearState;					// landing gear animation state
			float LandingGearSpeed;
			float LandingGearMinY;
			float LandingGearMaxY;
			
			// blade controller

			int   BladesState;						// Blades animation states
			float BladeAngularSpeed;
			float BladeAngularSpeedLimit;
			float BladeAcc;
			float BladeAngle;

			// gun turrest sates / animation parms

			int       GunTurretState;
			float     GunTurretSpeed;
			glm::vec3 GunTarget;

			// boosting controller

			float BoostTime;
			float BoostFactorA;
			
			// fire controller
			
			float FireRotationSpeed;
			float BulletSpacing;
			float BulletDt;
			float Elongazion;
			int   BulletOverlaysCount;
			
			// missile contorller
			
			int						MissileState;
			int						FiredMissilesCount;
			vml::models::Model3d_2* FiredMissileModel[16];
			int						MissileOverlaysCount;
			float					MissileRotationSpeed;
			float					MissileSpeed;


			float					Scale;

		//	vml::geo2d::PathFinder* PathFinder;
		//	int	NavMeshCellId;						// id of cell in the navmesh the navmesh is segmented into 'islands'
		//											// this value indicates in which of these islands the 
		//											// cleocopter belongs to see pathfinder for more info
										
			// -------------------------------------------------------------------------
			// state for copter animation

			static constexpr unsigned int NEUTRAL    = 0;
			static constexpr unsigned int LEFT_POSE  = 1;
			static constexpr unsigned int RIGHT_POSE = 2;
			static constexpr unsigned int BUMP_RIGHT = 3;
			static constexpr unsigned int BUMP_LEFT  = 4;
			static constexpr unsigned int BUMP_DOWN  = 5;
			static constexpr unsigned int BUMP_UP    = 6;

			// -------------------------------------------------------------------------
			// state for landing gear animation

			static constexpr unsigned int LANDING_GEAR_NEUTRAL    = 0;
			static constexpr unsigned int LANDING_GEAR_RETRACTED  = 1;
			static constexpr unsigned int LANDING_GEAR_GOING_UP   = 2;
			static constexpr unsigned int LANDING_GEAR_GOING_DOWN = 3;

			// -------------------------------------------------------------------------
			// state for blades animation

			static constexpr unsigned int BLADES_NEUTRAL = 0;
			static constexpr unsigned int BLADES_ON		 = 1;

			// -------------------------------------------------------------------------
			// Bump copter on the right

			void ComputeBumpRight()
			{
				Bumping = false;
				
				if (State == BUMP_RIGHT && !Bumping)
				{
					PrevYaw = Yaw;
					Bumping = true;
				}

				if (Bumping)
				{
					Yaw += TurnSpeed * BumpSpeed * 2;
					
					if (Yaw > 180.0f)
					{
						Yaw     = 180.0f;
						State   = LEFT_POSE;
						Bumping = false;
					}
					else
					{
						if (fabs(Yaw - PrevYaw) > 90.0f)
						{
							State   = NEUTRAL;
							Bumping = false;
						}

						Roll += RollSpeed * BumpSpeed;
					}
				}
			}

			// -------------------------------------------------------------------------
			//

			void ComputeBumpLeft()
			{
			
				if (State == BUMP_LEFT && !Bumping)
				{
					PrevYaw = Yaw;
					Bumping = true;
				}

				if (Bumping)
				{
					Yaw -= TurnSpeed * BumpSpeed*2;

					if (Yaw < 0)
					{
						Yaw     = 0.0f;
						State   = RIGHT_POSE;
						Bumping = false;
					}
					else
					{
						if (fabs(Yaw - PrevYaw) > 90.0f)
						{
							State   = NEUTRAL;
							Bumping = false;
						}

						Roll -= RollSpeed * BumpSpeed;
					}
				}
			}

			// -------------------------------------------------------------------------
			//

			void ComputeBumpDown()
			{

				if (State == BUMP_DOWN && !Bumping)
				{
					PrevPitch = Pitch;
					Bumping   = true;
				}

				if (Bumping)
				{
					Pitch -= ClimbSpeed * BumpSpeed * 4.0f;

					if (Pitch < -ClimbAngleLimit)
					{
						Pitch   = -ClimbAngleLimit;
						State   = NEUTRAL;
						Bumping = false;
					}
					else
					{

						if (fabs(Pitch - PrevPitch) > ClimbAngleLimit)
						{
							State   = NEUTRAL;
							Bumping = false;
						}
					}
				}
			}

			// -------------------------------------------------------------------------
			//

			void ComputeBumpUp()
			{
				if (State == BUMP_UP && !Bumping)
				{
					PrevPitch = Pitch;
					Bumping   = true;
				}

				if (Bumping)
				{
					Pitch += ClimbSpeed * BumpSpeed * 4.0f;

					if (Pitch > ClimbAngleLimit)
					{
						Pitch   = ClimbAngleLimit;
						State   = NEUTRAL;
						Bumping = false;
					}
					else
					{

						if (fabs(Pitch - PrevPitch) > ClimbAngleLimit)
						{
							State   = NEUTRAL;
							Bumping = false;
						}
					}
				}
			}

			// ---------------------------------------------------------------
			//

			void BumpController()
			{
				if (State == BUMP_RIGHT) ComputeBumpRight();
				if (State == BUMP_LEFT) ComputeBumpLeft();
				if (State == BUMP_DOWN) ComputeBumpDown();
				if (State == BUMP_UP) ComputeBumpUp();
			}

			// -------------------------------------------------------------------------
			// landing gear control, note that the key associated with 
			// the landing gear controller, must be a one time keypress or it won't work
			
			void LandingGearController()
			{
				if (Keys[LANDING_GEAR_KEY_PRESSED])
				{
					if (LandingGearState == LANDING_GEAR_NEUTRAL)
					{
						LandingGearState = LANDING_GEAR_GOING_UP;
					}
					else
					{
						if (LandingGearState == LANDING_GEAR_RETRACTED)
						{
							LandingGearState = LANDING_GEAR_GOING_DOWN;
						}
						else
						{
							if (LandingGearState == LANDING_GEAR_GOING_UP)
							{
								LandingGearState = LANDING_GEAR_GOING_DOWN;
							}
							else
							{
								if (LandingGearState == LANDING_GEAR_GOING_DOWN)
								{
									LandingGearState = LANDING_GEAR_GOING_UP;
								}
							}
						}
					}
				}

				if (LandingGearState == LANDING_GEAR_GOING_UP)
				{
					if (LandingGearModel->GetPosition().y < LandingGearMaxY)
						LandingGearModel->MoveY(LandingGearSpeed);
					else LandingGearState = LANDING_GEAR_RETRACTED;
				}

				if (LandingGearState == LANDING_GEAR_GOING_DOWN)
				{
					if (LandingGearModel->GetPosition().y > LandingGearMinY)
						LandingGearModel->MoveY(-LandingGearSpeed);
					else LandingGearState = LANDING_GEAR_NEUTRAL;
				}
			}

			// -------------------------------------------------------------------------
			// Boost controller

			void BoostController()
			{
				float dt = 0.25f;

				if (Keys[TAB_KEY_PRESSED])
				{
					BoostTime += dt;
					if (BoostTime > 1)
						BoostTime = 1.0f;
				}
				else
				{
					BoostTime -= dt;
					if (BoostTime < 0)
						BoostTime = 0.0f;
				}

				// position jet flames
				// -8 is the position when the flame is zero
				// -4 is the flame position at maximum intensity
				
				float fx = -4.0f + BoostTime * (-8.5f + 4.0f);

				JetFlame1Overlay->SetScaling(glm::vec3(9, 3.5f, 3) * BoostTime*Scale);
				JetFlame2Overlay->SetScaling(glm::vec3(9, 3.5f, 3) * BoostTime*Scale);
				JetFlame1Overlay->SetPosition(glm::vec3( 2.0f, -0.15f, fx)*Scale);
				JetFlame2Overlay->SetPosition(glm::vec3(-2.0f, -0.15f, fx)*Scale);
				JetFlame1Overlay->AddAngleY(BoostTime * 30.0f);
				JetFlame2Overlay->AddAngleY(BoostTime * 30.0f);

				// if jet flame overlay timer is greater than zero
				// visualize , otherwise, set them to invisible

				if (BoostTime > 0)
				{
					JetFlame1Overlay->SetVisible();
					JetFlame2Overlay->SetVisible();
				}
				else
				{
					JetFlame1Overlay->SetInVisible();
					JetFlame2Overlay->SetInVisible();
				}
			}

			// ---------------------------------------------------------------
			//
			
			void GunTurretController()
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
				if (Keys[LEFT_SHIFT_KEY_PRESSED] || Keys[RIGHT_SHIFT_KEY_PRESSED])
				{
					Yaw -= TurnSpeed * BoostFactorA * 2.0f;
					Keys[TAB_KEY_PRESSED] = true;	
				}
				
				if (!Keys[LEFT_SHIFT_KEY_PRESSED] || !Keys[RIGHT_SHIFT_KEY_PRESSED])
				{
					Yaw -= TurnSpeed * BoostFactorA;
				}

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

					if (Roll < -RollAngleLimit)
						Roll = -RollAngleLimit;
				}
			}

			// -------------------------------------------------------------------------
			//

			void MoveLeft()
			{
				if (Keys[LEFT_SHIFT_KEY_PRESSED] || Keys[RIGHT_SHIFT_KEY_PRESSED])
				{
					Yaw += TurnSpeed * 2.0f * BoostFactorA;
					Keys[TAB_KEY_PRESSED] = true;
				}

				if (!Keys[LEFT_SHIFT_KEY_PRESSED] || !Keys[RIGHT_SHIFT_KEY_PRESSED])
				{
					Yaw += TurnSpeed * BoostFactorA;
				}

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

					if (Roll > RollAngleLimit)
						Roll = RollAngleLimit;
				}
			}

			// ------------------------------------------------------------------------------------
			// blade animation
			// * Most American helicopter rotors turn counter-clockwise; French and Russian helicopters turn clockwise *

			void BladesController()
			{
				if (BladesState == BLADES_ON)
				{
					// accellerate blades to terminal angular velocity

					BladeAngularSpeed += BladeAcc;
					
					if (BladeAngularSpeed > BladeAngularSpeedLimit)
					{
						// if angular speed is greate the terminal velocity
						// then switch off blades model and visualize
						// the blades overlays

						BladeAngularSpeed = BladeAngularSpeedLimit;
						BladesOverlay->SetVisible();
						BladesModel->SetInVisible();
					}
				}

				if (BladesState == 2)
				{
					// decellerate to null angular velocity

					BladeAngularSpeed -= BladeAcc;
				
					if (BladeAngularSpeed < BladeAngularSpeedLimit)
					{
						// if angular speed is less the terminal velocity
						// then switch off blades overlay model and visualize
						// the blades model

						BladesOverlay->SetInVisible();
						BladesModel->SetVisible();
					}
				}

				// blades are stopped and the state
				// is resetted

				if (BladeAngularSpeed < 0.0f)
				{
					BladesState = BLADES_NEUTRAL;
					BladeAngularSpeed = 0.0f;
				}

				if (BladesState == 3)
				{
					BladeAngularSpeed = BladeAngularSpeedLimit + BladeAcc;
					BladesOverlay->SetVisible();
					BladesModel->SetInVisible();
				}

				// integrate angualr bvelocity
				BladeAngle += BladeAngularSpeed;
				RotorModel->SetAngles(glm::vec3(0, BladeAngle, 0));
				TailRotorModel->SetAngles(glm::vec3(BladeAngle, 0, 0));
			}

			// -------------------------------------------------------------------
			// Fire controller

			void FireController()
			{
				if (Keys[SPACE_KEY_PRESSED])
				{
					const glm::vec3 dir = GunModel->GetForwardVector();
					GunMuzzleFlashOverlay->AddAngleY(FireRotationSpeed);
					GunMuzzleFlashOverlay->SetVisible();

					for (size_t i = 0; i < BulletOverlaysCount; ++i)
					{
						vml::overlays::EffectOverlay* bullet = BulletOverlays[i];
						bullet->SetVisible();
						bullet->MoveZ(BulletDt);
						glm::vec3 bulletpos = bullet->GetPosition();
						glm::vec3 startbulletpos = bullet->GetOriginalPosition();
						glm::vec3 deltapos = bulletpos - startbulletpos + glm::vec3(0, 0, BulletSpacing * (float)i);
						float t = (deltapos.x * deltapos.x + deltapos.y * deltapos.y + deltapos.z * deltapos.z) / Elongazion / Elongazion;
						if (t > 1.0f)
							bullet->SetPosition(startbulletpos - glm::vec3(0, 0, BulletSpacing * (float)i));
					}
				}
				else
				{
					GunMuzzleFlashOverlay->SetInVisible();
					for (size_t i = 0; i < BulletOverlaysCount; ++i)
						BulletOverlays[i]->SetInVisible();
				}
			}

			// -------------------------------------------------------------------------------------
			// Missile controller

			void MissileController()
			{
				if (MissileState == 1)
				{
					if (FiredMissilesCount < 16)
					{
						vml::models::Model3d_2* model = MissileModel[FiredMissilesCount];
						vml::models::Model3d_2* parent = model->GetParent();

						if (parent)
						{
							parent->UnLink(model);
							FiredMissileModel[FiredMissilesCount] = model;
							MissileOverlays[FiredMissilesCount]->SetVisible();
							FiredMissilesCount++;
						}
					}

					MissileState = 2;
				}

				if (MissileState == 2)
				{
					for (size_t i = 0; i < FiredMissilesCount; ++i)
					{
						vml::models::Model3d_2* model = FiredMissileModel[i];
						model->Move(model->GetForwardVector() * MissileSpeed);
						if (MissileOverlays[i]->IsVisible())
							MissileOverlays[i]->AddAngleY(MissileRotationSpeed);
					}
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
			static constexpr unsigned int SPACE_KEY_PRESSED		   = 11;

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

			// -------------------------------------------------------------------------
			// Key handling

			void BindKey(unsigned int key,bool value) { Keys[key] = value; }
			bool GetKey(unsigned int key) const { return Keys[key]; }

			// -------------------------------------------------------------------------
			// main controller

			void Controller(vml::views::View* view)
			{
				
				TurnSpeed        = 3.2f;
				ClimbSpeed       = 1.6f;
				RollDampening    = 0.9f;
				RollSpeed        = 3.2f;
				LandingGearSpeed = 0.02f;
				GunTurretSpeed   = 0.02f;
								
				// check if damping is over

				DampingIsOver = Pitch > -DampingEps && Pitch < DampingEps;

				// get direction vector

				float dx = BodyModel->GetForwardVector().x;

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

					if (dx > 0)
					{
						Keys[RIGHT_KEY_PRESSED] = true;
						if (Keys[LEFT_KEY_PRESSED])
						{
							Keys[RIGHT_KEY_PRESSED] = false;
							Keys[LEFT_KEY_PRESSED] = true;
						}
					}
					else
					{
						Keys[LEFT_KEY_PRESSED] = true;
						if (Keys[RIGHT_KEY_PRESSED])
						{
							Keys[RIGHT_KEY_PRESSED] = true;
							Keys[LEFT_KEY_PRESSED] = false;
						}
					}
				}
				else
				{
					BoostFactorA = 1.0f;
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
						Pitch -= ClimbSpeed;
						if (Pitch < -10)
							Pitch = -10;

					}

					// if right key is released, reset forward bow timer

					if (!Keys[RIGHT_KEY_PRESSED])
					{
						State = NEUTRAL;
					}

					// if these keys are pressed, go back to neutral state

					if (Keys[RIGHT_KEY_PRESSED] && Keys[UP_KEY_PRESSED])
					{
						State = NEUTRAL;
					}

					if (Keys[RIGHT_KEY_PRESSED] && Keys[DOWN_KEY_PRESSED])
					{
						State = NEUTRAL;
					}

				}

				// left direction

				if (State == LEFT_POSE)
				{

					// if left key is pressed , then perform a forward bow

					if (Keys[LEFT_KEY_PRESSED])
					{
						Pitch -= ClimbSpeed;
						if (Pitch < -10)
							Pitch = -10;

					}

					// if left key is relese, reset forward bow timer

					if (!Keys[LEFT_KEY_PRESSED])
					{
						State = NEUTRAL;
					}

					// if these keys are pressed, go back to neutral state

					if (Keys[LEFT_KEY_PRESSED] && Keys[UP_KEY_PRESSED])
					{
						State = NEUTRAL;
					}

					if (Keys[LEFT_KEY_PRESSED] && Keys[DOWN_KEY_PRESSED])
					{
						State = NEUTRAL;
					}

				}

				// damp accumulated rool and pitch angles

				Roll  *= RollDampening;
				Pitch *= RollDampening;

				//blades controller
				BladesController();

				// bump controller
				BumpController();

				// boost controller
				BoostController();

				// landing gear controller
				LandingGearController();

				// gun turret conroller
				GunTurretController();
		
				// fire controller
				FireController();

				// missile controller
				MissileController();

				// compute forces
				ComputeForces();
			}
		
			// ---------------------------------------------------
			// 

			void TurnOnBlades()
			{
				if (BladesState == 0 || BladesState==3)
					BladesState = 1;
			}

			// -------------------------------------------------------------------
			//

			void TurnOffBlades()
			{
				if (BladesState == 1 || BladesState == 3)
					BladesState = 2;
			}

			// -------------------------------------------------------------------
			//

			void FullBlades()
			{
				BladesState = 3;
			}

			// -------------------------------------------------------------------
			//
			
			void FireMissile()
			{
				MissileState = 1;
			}

			// -------------------------------------------------------------------
			//
			
			void BumpUp()    
			{
				State = BUMP_UP; 
			}
			
			void BumpDown()  
			{
				State = BUMP_DOWN; 
			}
			
			void BumpRight() 
			{
				State = BUMP_RIGHT; 
			}
			
			void BumpLeft()
			{
				State = BUMP_LEFT; 
			}
			
			bool IsBumping() const 
			{ 
				return Bumping; 
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
			/*
			// -------------------------------------------------------------------
			//
			
			void Bump(const glm::vec3 &mtvNormal) 
			{
				if (mtvNormal.x > 0) State = BUMP_LEFT;
				if (mtvNormal.x < 0) State = BUMP_RIGHT;
				if (mtvNormal.y > 0) State = BUMP_UP;
				if (mtvNormal.y < 0) State = BUMP_DOWN;
			}
			*/
			// -------------------------------------------------------------------
			//

			void ComputeForces()
			{
				bool UpKeyPressed   = Keys[UP_KEY_PRESSED];
				bool DownKeyPressed = Keys[DOWN_KEY_PRESSED];
				bool LeftKeyPressed = Keys[LEFT_KEY_PRESSED];
				bool RightKeyPressed = Keys[RIGHT_KEY_PRESSED];
				bool keyPressed = UpKeyPressed || DownKeyPressed || LeftKeyPressed || RightKeyPressed;

				/*
				if (!keyPressed) std::cout << "Neutral" << std::endl;
				if (LeftKeyPressed) std::cout << "Left" << std::endl;
				if (UpKeyPressed)   std::cout << "Up" << std::endl;
				if (DownKeyPressed) std::cout << "Down" << std::endl;
				if (RightKeyPressed)std::cout << "Right" << std::endl;
				*/
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

			void ResetMissiles()
			{
				for ( size_t i=0; i<FiredMissilesCount; ++i)
				{ 
					vml::models::Model3d_2* model = FiredMissileModel[i];
					model->ResetTransformations();
					BodyModel->Link(model);
					MissileOverlays[i]->SetInVisible();
				}
				MissileState = 0;
				FiredMissilesCount = 0;
			}

			// -------------------------------------------------------------------
			//
			
	//		void ComputeGunHitPoint(glm::vec3 &guntip, glm::vec3 &gundir)
	//		{
	//			gundir = GunModel->GetForwardVector();
	//			guntip = GunModel->GetTransformedPosition() + 0.2f * GunModel->GetForwardVector() + 0.02f * GunModel->GetUpVector();
	//		}
			/*
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
			*/		
			
			// -------------------------------------------------------------------
			//

			[[nodiscard]] vml::models::Model3d_2* GetBodyModel() const { return BodyModel; }
			[[nodiscard]] vml::models::Model3d_2* GetBladesModel() const { return BladesModel; }
			[[nodiscard]] vml::models::Model3d_2* GetLandingGearModel() const { return LandingGearModel; }
			[[nodiscard]] vml::models::Model3d_2* GetGunRotorModel() const { return GunRotorModel; }
			[[nodiscard]] vml::models::Model3d_2* GetRotorModel() const { return RotorModel; }
			[[nodiscard]] vml::models::Model3d_2* GetTailRotorModel() const { return TailRotorModel; }
			[[nodiscard]] vml::models::Model3d_2* GetGunModel() const { return GunModel; }
			[[nodiscard]] vml::models::Model3d_2* GetJetsModel() const { return JetsModel; }
			[[nodiscard]] vml::models::Model3d_2* GetGunPivotModel() const { return GunPivotModel; }
			[[nodiscard]] vml::models::Model3d_2* GetMissileModel(const size_t id) const { return MissileModel[id]; }
			[[nodiscard]] float GetElongazion() const { return Elongazion; }
			[[nodiscard]] const std::string& GetScreenName() const { return ScreenName; }

	//		int GetNavMeshCellId() const { return NavMeshCellId; }
			
			// -------------------------------------------------------------------
			// Object creation
			
			void CreateObject(vml::models::ModelHandler *modelhandler, vml::overlays::OverlayHandler* overlayhandler)
			{
				
				// create cleocopter

				std::string sourcepath= vml::utils::GlobalPaths::GetInstance()->GetFullProjectPath() + "/meshes/cleocopter2/newcleocopter16_imp/3dfs/";

				BodyModel = modelhandler->Add(ScreenName+"/Body",
											  sourcepath + "body.3df",
											  glm::vec3(0, 0, 0), 
											  glm::vec3(0, 0, 0), 
											  glm::vec3(1,1,1)*Scale, 
											  { vml::models::SPHERICAL_QUATERNION });

				CanopyModel = modelhandler->Add(ScreenName + "/Canopy",
											    sourcepath + "canopy.3df",
											    glm::vec3(0.051f, 2.274f, 9.42f)*Scale,
											    glm::vec3(0, 0, 0),
												glm::vec3(1,1,1)*Scale,
											    { vml::models::EULER });
				
				JetsModel = modelhandler->Add(ScreenName + "/Jets",
											  sourcepath + "jets.3df",
											  glm::vec3(0.082f, 3.046f, 1.713f)*Scale,
											  glm::vec3(0, 0, 0),
											  glm::vec3(1,1,1)*Scale,
											  { vml::models::EULER });

				RotorModel = modelhandler->Add(ScreenName + "/Rotor",
											   sourcepath + "main_rotor_shaft.3df",
											   glm::vec3(0.1f, 4.72001f, 4.7f)*Scale,
											   glm::vec3(0, 0, 0),
											   glm::vec3(1,1,1)*Scale,
											   { vml::models::EULER });

				RadarModel = modelhandler->Add(ScreenName + "/Radar",
											   sourcepath + "radar.3df",
											   glm::vec3(0.1f, 5.74003f, 4.7f)*Scale,
											   glm::vec3(0, 0, 0),
											   glm::vec3(1,1,1)*Scale,
											   { vml::models::EULER });

				TailRotorModel = modelhandler->Add(ScreenName + "/TailRotor",
												   sourcepath + "tail_rotor.3df",
												   glm::vec3(0.1, 0.88f, -12.4f)*Scale,
												   glm::vec3(0, 0, 0),
												   glm::vec3(1,1,1)*Scale,
												   { vml::models::EULER });

				BladesModel = modelhandler->Add(ScreenName + "/Blades",
												sourcepath + "mainrotor.3df",
												glm::vec3(0.05f, 0.19f, -0.56f)*Scale,
												glm::vec3(0, 0, 0),
												glm::vec3(1,1,1)*Scale,
												{ vml::models::EULER });

				LandingGearModel = modelhandler->Add(ScreenName + "/Stand",
												     sourcepath + "stand.3df",
												     glm::vec3(0.05f, -2.5f, 1.72f)*Scale,
												     glm::vec3(0, 0, 0),
													 glm::vec3(1,1,1)*Scale,
												     { vml::models::EULER });

				GunRotorModel = modelhandler->Add(ScreenName + "/GunRotor",
												  sourcepath + "gunrotor.3df",
												  glm::vec3(0.12f, -0.5f, 8.2301f)* Scale,
												  glm::vec3(0, 0, 0),
												  glm::vec3(1,1,1)*Scale,
												  { vml::models::EULER });

				GunPivotModel = modelhandler->Add(ScreenName + "/GunPivot",
												  sourcepath + "gunpivot.3df",
												  glm::vec3(-0.02f, -0.22f, -0.12f)* Scale,
												  glm::vec3(0, 0, 0),
												  glm::vec3(1,1,1)*Scale,
												  { vml::models::EULER });

				GunModel = modelhandler->Add(ScreenName + "/Gun",
											 sourcepath + "gun.3df",
											 glm::vec3(0.04f, -0.24f, 1.52f)* Scale,
											 glm::vec3(0, 0, 0),
											 glm::vec3(1,1,1)*Scale,
											 { vml::models::EULER });

				// first missle group

				MissileModel[0] = modelhandler->Add(ScreenName + "/Missile0",
													sourcepath + "missile.3df",
													glm::vec3(4.44f, -1.02f, 0.8f)* Scale,
													glm::vec3(0, 0, 0),
													glm::vec3(1,1,1)*Scale,
													{ vml::models::EULER });

				MissileModel[1] = modelhandler->Add(ScreenName + "/Missile1",
													sourcepath + "missile.3df",
													glm::vec3(4.44f, -0.5f, 0.8f)* Scale,
													glm::vec3(0, 0, 0),
													glm::vec3(1,1,1)*Scale,
													{ vml::models::EULER });

				MissileModel[2] = modelhandler->Add(ScreenName + "/Missile2",
													sourcepath + "missile.3df",
													glm::vec3(3.94f, -0.5f, 0.8f)* Scale,
													glm::vec3(0, 0, 0),
													glm::vec3(1,1,1)*Scale,
													{ vml::models::EULER });

				MissileModel[3] = modelhandler->Add(ScreenName + "/Missile3",
													sourcepath + "missile.3df",
													glm::vec3(3.94f, -1.02f, 0.8f)* Scale,
													glm::vec3(0, 0, 0),
													glm::vec3(1,1,1)*Scale,
													{ vml::models::EULER });

				// second missile group

				MissileModel[4] = modelhandler->Add(ScreenName + "/Missile4",
													sourcepath + "missile.3df",
													glm::vec3(2.85f, -0.58f, 2.5f)* Scale,
													glm::vec3(0, 0, 0),
													glm::vec3(1,1,1)*Scale,
													{ vml::models::EULER });
 
				MissileModel[5] = modelhandler->Add(ScreenName + "/Missile5",
													sourcepath + "missile.3df",
													glm::vec3(2.85f, -0.05f, 2.5f)* Scale,
													glm::vec3(0, 0, 0),
													glm::vec3(1,1,1)*Scale,
													{ vml::models::EULER });

				MissileModel[6] = modelhandler->Add(ScreenName + "/Missile6",
													sourcepath + "missile.3df",
													glm::vec3(2.31f, -0.05f, 2.5f)* Scale,
													glm::vec3(0, 0, 0),
													glm::vec3(1,1,1)*Scale,
													{ vml::models::EULER });

				MissileModel[7] = modelhandler->Add(ScreenName + "/Missile7",
												    sourcepath + "missile.3df",
												    glm::vec3(2.31f, -0.58f, 2.5f)* Scale,
												    glm::vec3(0, 0, 0),
													glm::vec3(1,1,1)*Scale,
												    { vml::models::EULER });

				// third missile group

				MissileModel[8] = modelhandler->Add(ScreenName + "/Missile8",
													 sourcepath + "missile.3df",
													 glm::vec3(-2.31f, -0.58f, 2.5f)* Scale,
													 glm::vec3(0, 0, 0),
													 glm::vec3(1,1,1)*Scale,
													 { vml::models::EULER });

				MissileModel[9] = modelhandler->Add(ScreenName + "/Missile9",
													 sourcepath + "missile.3df",
													 glm::vec3(-2.31f, -0.05f, 2.5)* Scale,
													 glm::vec3(0, 0, 0),
													 glm::vec3(1,1,1)*Scale,
													 { vml::models::EULER });

				MissileModel[10] = modelhandler->Add(ScreenName + "/Missile10",
													 sourcepath + "missile.3df",
													 glm::vec3(-2.85f, -0.05f, 2.5f)* Scale,
													 glm::vec3(0, 0, 0),
													 glm::vec3(1,1,1)*Scale,
													 { vml::models::EULER });

				MissileModel[11] = modelhandler->Add(ScreenName + "/Missile11",
													 sourcepath + "missile.3df",
													 glm::vec3(-2.85f, -0.58f, 2.5f)* Scale,
													 glm::vec3(0, 0, 0),
													 glm::vec3(1,1,1)*Scale,
													 { vml::models::EULER });

				// forth missile group

				MissileModel[12] = modelhandler->Add(ScreenName + "/Missile12",
													 sourcepath + "missile.3df",
													 glm::vec3(-3.94f, -1.02f, 0.8f)* Scale,
													 glm::vec3(0, 0, 0),
													 glm::vec3(1,1,1)*Scale,
													 { vml::models::EULER });

				MissileModel[13] = modelhandler->Add(ScreenName + "/Missile13",
													 sourcepath + "missile.3df",
													 glm::vec3(-3.94f, -0.5f, 0.8f)* Scale,
													 glm::vec3(0, 0, 0),
													 glm::vec3(1,1,1)*Scale,
													 { vml::models::EULER });

				MissileModel[14] = modelhandler->Add(ScreenName + "/Missile14",
													 sourcepath + "missile.3df",
													 glm::vec3(-4.44f, -0.5f, 0.8f)* Scale,
													 glm::vec3(0, 0, 0),
													 glm::vec3(1,1,1)*Scale,
													 { vml::models::EULER });

				MissileModel[15] = modelhandler->Add(ScreenName + "/Missile15",
													 sourcepath + "missile.3df",
													 glm::vec3(-4.44f, -1.02f, 0.8f)* Scale,
													 glm::vec3(0, 0, 0),
													 glm::vec3(1,1,1)*Scale,
													 { vml::models::EULER });

				FiredMissileModel[ 0] = nullptr;
				FiredMissileModel[ 1] = nullptr;
				FiredMissileModel[ 2] = nullptr;
				FiredMissileModel[ 3] = nullptr;
				FiredMissileModel[ 4] = nullptr;
				FiredMissileModel[ 5] = nullptr;
				FiredMissileModel[ 6] = nullptr;
				FiredMissileModel[ 7] = nullptr;
				FiredMissileModel[ 8] = nullptr;
				FiredMissileModel[ 9] = nullptr;
				FiredMissileModel[10] = nullptr;
				FiredMissileModel[11] = nullptr;
				FiredMissileModel[12] = nullptr;
				FiredMissileModel[13] = nullptr;
				FiredMissileModel[14] = nullptr;
				FiredMissileModel[15] = nullptr;
				
				// Link models

				BodyModel->Link(CanopyModel);
				BodyModel->Link(JetsModel);
				BodyModel->Link(RotorModel);
				BodyModel->Link(RadarModel);
				BodyModel->Link(TailRotorModel);
				RotorModel->Link(BladesModel);
				BodyModel->Link(LandingGearModel);
				BodyModel->Link(GunRotorModel);
				GunRotorModel->Link(GunPivotModel);
				GunPivotModel->Link(GunModel);
				BodyModel->Link(MissileModel[0]);
				BodyModel->Link(MissileModel[1]);
				BodyModel->Link(MissileModel[2]);
				BodyModel->Link(MissileModel[3]);
				BodyModel->Link(MissileModel[4]);
				BodyModel->Link(MissileModel[5]);
				BodyModel->Link(MissileModel[6]);
				BodyModel->Link(MissileModel[7]);
				BodyModel->Link(MissileModel[8]);
				BodyModel->Link(MissileModel[9]);
				BodyModel->Link(MissileModel[10]);
				BodyModel->Link(MissileModel[11]);
				BodyModel->Link(MissileModel[12]);
				BodyModel->Link(MissileModel[13]);
				BodyModel->Link(MissileModel[14]);
				BodyModel->Link(MissileModel[15]);

				// copter starts with landing gear retracted
		
				LandingGearMinY = LandingGearModel->GetPosition().y;
				LandingGearMaxY = 0.93f*Scale;

				// landing gear up

				LandingGearState = 1;
				LandingGearModel->MoveY(LandingGearMaxY - LandingGearMinY);

				// overlays
				
				// blades overlay

				BladesOverlay = overlayhandler->Add(vml::overlays::EffectOverlay::QUAD,
												   ScreenName + "/Blades",
												   vml::utils::GlobalPaths::GetInstance()->GetFullProjectPath() + "/textures/bladesblurred5.png",
												   glm::vec3(0, 0, 0)* Scale, glm::vec3(90, 0, 0), glm::vec3(30, 30, 30)* Scale);
				BladesOverlay->AttachModel(BladesModel);
				BladesOverlay->SetInVisible();

				// gun muzzle flash overlay

				GunMuzzleFlashOverlay = overlayhandler->Add(vml::overlays::EffectOverlay::FAN,
															ScreenName + "/MuzzleFalsh",
															vml::utils::GlobalPaths::GetInstance()->GetFullProjectPath() + "/textures/muzzleflash4.png",
															glm::vec3(0, 0.2f, 5.7f)* Scale, glm::vec3(270, 0, 90), glm::vec3(7.5f, 7.5f, 7.5f)* Scale);
				GunMuzzleFlashOverlay->AttachModel(GunModel);
				GunMuzzleFlashOverlay->SetInVisible();

				// jet flames overlays

				JetFlame1Overlay = overlayhandler->Add(vml::overlays::EffectOverlay::FAN,
													   ScreenName + "/JetFlame1",
													   vml::utils::GlobalPaths::GetInstance()->GetFullProjectPath() + "/textures/jetflame1_1.png",
													   glm::vec3(2.0f, -0.15f, -8.5f)* Scale, glm::vec3(270, 0, 90), glm::vec3(9, 3.5f, 3)* Scale);

				JetFlame2Overlay = overlayhandler->Add(vml::overlays::EffectOverlay::FAN,
													   ScreenName + "/JetFlame2",
													   vml::utils::GlobalPaths::GetInstance()->GetFullProjectPath() + "/textures/jetflame1_1.png",
													   glm::vec3(-2.0f, -0.15f, -8.5f)* Scale, glm::vec3(270, 0, 90), glm::vec3(9, 3.5f, 3)* Scale);

				JetFlame1Overlay->AttachModel(JetsModel);
				JetFlame2Overlay->AttachModel(JetsModel);
				JetFlame1Overlay->SetInVisible();
				JetFlame2Overlay->SetInVisible();

				// bullets overlays
				
				glm::vec3 startpos = glm::vec3(0, 0.4f/2, 11.5f/2);

				for ( size_t i=0; i< BulletOverlaysCount; ++i)
				{
					BulletOverlays[i] = overlayhandler->Add(vml::overlays::EffectOverlay::FAN,
															ScreenName + "/Bullet" + std::to_string(i),
															vml::utils::GlobalPaths::GetInstance()->GetFullProjectPath() + "/textures/bullet1.png",
															startpos*Scale + glm::vec3(0, 0, BulletSpacing * (float)i), glm::vec3(270, 0, 90), glm::vec3(20/2, 10/2, 10/2)* Scale);
					BulletOverlays[i]->AttachModel(GunModel);
					BulletOverlays[i]->SetInVisible();
				}

				// missiles overlays

				for (size_t i = 0; i < MissileOverlaysCount; ++i)
				{
					MissileOverlays[i] = overlayhandler->Add(vml::overlays::EffectOverlay::FAN,
															 ScreenName + "/MissileOverlay" + std::to_string(i),
															 vml::utils::GlobalPaths::GetInstance()->GetFullProjectPath() + "/textures/jetflame1_1.png",
															 glm::vec3(0, 0, -3.5f)* Scale, glm::vec3(270, 0, 90), glm::vec3(9, 3.5f, 3) * 0.5f* Scale);
					MissileOverlays[i]->AttachModel(MissileModel[i]);
					MissileOverlays[i]->SetInVisible();
				}
				
			}
			
			// ---------------------------------------------------
			// ctor / dtor

			CleoCopter_2(const std::string& screenname, 
						 const float scale,
						 vml::models::ModelHandler* modelhandler,
						 vml::overlays::OverlayHandler* overlayhandler
						 /* , vml::geo2d::PathFinder* pathfinder*/
						 /*  ,vml::handlers::Scene* scene*/ )
			{
				if (screenname.empty())
					vml::os::Message::Error("ScreenName is empty");

				// models

				ScreenName = screenname;

				BodyModel                = nullptr;
				CanopyModel              = nullptr;
				JetsModel                = nullptr;
				RotorModel               = nullptr;
				RadarModel               = nullptr;
				TailRotorModel           = nullptr;
				BladesModel              = nullptr;
				LandingGearModel         = nullptr;
				GunRotorModel            = nullptr;
				GunMountModel            = nullptr;
				GunPivotModel            = nullptr;
				GunModel                 = nullptr;
				MissileModel[ 0]         = nullptr;
				MissileModel[ 1]         = nullptr;
				MissileModel[ 2]         = nullptr;
				MissileModel[ 3]         = nullptr;
				MissileModel[ 4]         = nullptr;
				MissileModel[ 5]         = nullptr;
				MissileModel[ 6]         = nullptr;
				MissileModel[ 7]         = nullptr;
				MissileModel[ 8]         = nullptr;
				MissileModel[ 9]         = nullptr;
				MissileModel[10]         = nullptr;
				MissileModel[11]         = nullptr;
				MissileModel[12]         = nullptr;
				MissileModel[13]         = nullptr;
				MissileModel[14]         = nullptr;
				MissileModel[15]         = nullptr;

				// overlays

				BladesOverlay	      = nullptr;
				GunMuzzleFlashOverlay = nullptr;
				JetFlame1Overlay	  = nullptr;
				JetFlame2Overlay	  = nullptr;
				BulletOverlays[ 0]	  = nullptr;
				BulletOverlays[ 1]	  = nullptr;
				BulletOverlays[ 2]	  = nullptr;
				BulletOverlays[ 3]	  = nullptr;
				BulletOverlays[ 4]	  = nullptr;
				BulletOverlays[ 5]	  = nullptr;
				BulletOverlays[ 6]	  = nullptr;
				BulletOverlays[ 7]	  = nullptr;
				BulletOverlays[ 8]	  = nullptr;
				BulletOverlays[ 9]	  = nullptr;
				BulletOverlays[10]	  = nullptr;
				BulletOverlays[11]	  = nullptr;
				BulletOverlays[12]	  = nullptr;
				BulletOverlays[13]	  = nullptr;
				BulletOverlays[14]	  = nullptr;
				BulletOverlays[15]	  = nullptr;
				MissileOverlays[0]    = nullptr;
				MissileOverlays[1]    = nullptr;
				MissileOverlays[2]    = nullptr;
				MissileOverlays[3]    = nullptr;
				MissileOverlays[4]    = nullptr;
				MissileOverlays[5]    = nullptr;
				MissileOverlays[6]    = nullptr;
				MissileOverlays[7]    = nullptr;
				MissileOverlays[8]    = nullptr;
				MissileOverlays[9]    = nullptr;
				MissileOverlays[10]   = nullptr;
				MissileOverlays[11]   = nullptr;
				MissileOverlays[12]   = nullptr;
				MissileOverlays[13]   = nullptr;
				MissileOverlays[14]   = nullptr;
				MissileOverlays[15]   = nullptr;

				// Cleocopter scale
				
				Scale=scale;
				 
				// flags
				
				BoostTime = 0.0f;

				// animation

				State				   = NEUTRAL;
				DirectionState		   = 0;
				DampingIsOver		   = false;
				DampingEps			   = 0.01f;
				Yaw				       = 0.0f;
				Roll			       = 0.0f;
				Pitch			       = 0.0f;
				PrevYaw			       = 0.0f;
				PrevPitch  		       = 0.0f;
				TurnSpeed	           = 0.25f;
				ClimbSpeed	           = 0.09f;
				RollDampening          = 0.995f;					// return to 0 roll speed rate
				RollSpeed	           = 0.1f;
				BumpSpeed	           = 2.0f;
				Bumping				   = false;
				RollAngleLimit	       = 70.0f;					// maximum pitch angle
				ClimbAngleLimit        = 45.0f;					// Angle limit when climbing up or down
				LastHorzDirectionState = 0;
				LastVertDirectionState = 0;
				DirectionVector		   = glm::vec3(1, 0, 0);

				LandingGearState	   = LANDING_GEAR_NEUTRAL;
				LandingGearSpeed	   = 0.005f;
				LandingGearMinY		   = 0.0f;
				LandingGearMaxY		   = 0.0f;

				BladesState			   = 0;
				BladeAngle			   = 0.0f;
				BladeAngularSpeed	   = 0.0f;
				BladeAcc = 0.05f;
				BladeAngularSpeedLimit = 12.0f;

				GunTurretState	       = 0;
				GunTurretSpeed		   = 0.00125f;
				BoostFactorA		   = 1.0f;

				FireRotationSpeed		= 10.0f;
				BulletDt				= 2.5f;
				BulletSpacing			= 12.5f;
				Elongazion				= 150.0f;

				BulletOverlaysCount		= 12;

				MissileState			= 0;
				FiredMissilesCount		= 0;
				MissileOverlaysCount	= 16;
				MissileRotationSpeed	= 10.0f;
				MissileSpeed			= 0.025f;

				/*
				ForceMagnitude		   = 1.5f;								
				RbForceMagnitude	   = 1.0f;								
				Mass				   = 1.0f;
				InvMass				   = 1.0f / Mass;
				RbForceDirection	   = glm::vec3(0, 0, 0);
				Accelleration		   = glm::vec3(0, 0, 0);
				Velocity			   = glm::vec3(0, 0, 0);

		//		NavMeshCellId		   = -1;
		//		PathFinder			   =  nullptr;
				*/

				// clear keys
				
				memset(Keys, 0, sizeof(unsigned char) * 1024);

				BulletSpacing *= Scale;
				BulletDt *= Scale;
				Elongazion *= Scale;

				// create object
			
				CreateObject(modelhandler,overlayhandler);


				/*
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
				*/
			}

			~CleoCopter_2()
			{
			}
			
	};
	
}
