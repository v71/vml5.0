#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Debug light

namespace vml
{

	namespace debugrendering
	{

		/////////////////////////////////////////////////////////////////
		// directional light

		class DebugDirectionalLight
		{

			public:
				
				//------------------------------------------------
				// light properties

				glm::vec4 Ambient;
				glm::vec4 Diffuse;
				glm::vec4 Specular;
				glm::vec4 Direction;
				glm::vec4 CameraSpaceDirection;
				float	  Power;

				//------------------------------------------------
				// ctor / dtor

				DebugDirectionalLight()
				{
					Ambient = glm::vec4(1, 1, 1, 1);
					Diffuse = glm::vec4(1, 1, 1, 1);
					Specular = glm::vec4(1, 1, 1, 1);
					Direction = glm::vec4(0, 0, 0, 1);
					CameraSpaceDirection = glm::vec4(0, 0, 0, 1);
					Power = 1.0f;
				}

				DebugDirectionalLight(const glm::vec4 ambient,
									  const glm::vec4 diffuse,
									  const glm::vec4 specular,
									  const glm::vec4 direction,
									  float power)
				{
					Ambient = ambient;
					Diffuse = diffuse;
					Specular = specular;
					Direction = direction;
					Power = power;
					CameraSpaceDirection = glm::vec4(0, 0, 0, 0);
				}

				~DebugDirectionalLight()
				{}

		};

	}

}
