#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////
// Debug material class

namespace vml
{

	namespace debugrendering
	{

		////////////////////////////////////////////////////////////////
		// simple material 

		class DebugMaterial
		{

			public:
				
				// ---------------------------------------------------------------
				// private data

				glm::vec4	Ambient;			// ambient component
				glm::vec4	Diffuse;			// diffuse component
				glm::vec4	Specular;			// specular component
				glm::vec4	Emission;			// emission component
				float				Shininess;			// shininess component

				// ---------------------------------------------------------------
				// ctor / dtor

				DebugMaterial(const glm::vec4& ambient,
							  const glm::vec4& diffuse,
							  const glm::vec4& specular,
							  const glm::vec4& emission,
							  const float shininess)
				{
					Ambient = ambient;
					Diffuse = diffuse;
					Specular = specular;
					Emission = emission;
					Shininess = shininess;
				}

				DebugMaterial()
				{
					Ambient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
					Diffuse = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
					Specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
					Emission = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
					Shininess = 15;
				}

				~DebugMaterial()
				{
				}

		};

	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Debug materials

namespace vml
{
	namespace debugrendering
	{

		static DebugMaterial Material0(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),
									   glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),
									   glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
									   glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
									   15);

		static DebugMaterial Material1(glm::vec4(0.5f, 0.0f, 0.0f, 1.0f),
									   glm::vec4(0.5f, 0.0f, 0.0f, 1.0f),
									   glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
									   glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
									   15);

		static DebugMaterial Material2(glm::vec4(0.0f, 0.5f, 0.0f, 1.0f),
									   glm::vec4(0.0f, 0.5f, 0.0f, 1.0f),
									   glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
									   glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
									   15);

		static DebugMaterial MaterialWhite(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
										   glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
										   glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
										   glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
										   10);

	}
}
