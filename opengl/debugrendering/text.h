#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

namespace vml
{
	namespace freetype
	{
		class Text
		{
			private:

				// Holds all state information relevant to a character as loaded using FreeType

				struct Character
				{
					unsigned int TextureID; // ID handle of the glyph texture
					glm::ivec2   Size;      // Size of glyph
					glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
					unsigned int Advance;   // Horizontal offset to advance to next glyph
				};

				Character					   Characters[1024];
				unsigned int				   VAO;
				unsigned int				   VBO;
				vml::shaders::GlShaderProgram* TextShader;
				GLuint						   ProjectionLocation;
				GLuint						   ColorLocation;

			public:

				// --------------------------------------------------

				int Init(const std::string& path)
				{

					std::string fullpath = path + "text.shd";

					// compile and setup the shader

					TextShader = new vml::shaders::GlShaderProgram(fullpath, {});

					// FreeType

					FT_Library ft;

					// All functions return a value different than 0 whenever an error occurred

					if (FT_Init_FreeType(&ft))
					{
						std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
						return -1;
					}

					// find path to font

					std::string font_name = "Roboto-Regular.ttf";
					if (font_name.empty())
					{
						std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
						return -1;
					}

					// load font as face

					FT_Face face;

					if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
						std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
						return -1;
					}
					else
					{
						// set size to load glyphs as

						FT_Set_Pixel_Sizes(face, 0, 48);

						// disable byte-alignment restriction

						glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

						// load first 128 characters of ASCII set

						for (unsigned char c = 0; c < 128; c++)
						{

							// Load character glyph 

							if (FT_Load_Char(face, c, FT_LOAD_RENDER))
							{
								std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
								continue;
							}

							// generate texture

							unsigned int texture;
							glGenTextures(1, &texture);
							glBindTexture(GL_TEXTURE_2D, texture);
							glTexImage2D(
								GL_TEXTURE_2D,
								0,
								GL_RED,
								face->glyph->bitmap.width,
								face->glyph->bitmap.rows,
								0,
								GL_RED,
								GL_UNSIGNED_BYTE,
								face->glyph->bitmap.buffer
							);

							// set texture options

							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

							// now store character for later use

							Characters[c] = Character{
														texture,
														glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
														glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
														static_cast<unsigned int>(face->glyph->advance.x)
							};

						}

						glBindTexture(GL_TEXTURE_2D, 0);
					}

					// destroy FreeType once we're finished

					FT_Done_Face(face);
					FT_Done_FreeType(ft);

					// configure VAO/VBO for texture quads

					glGenVertexArrays(1, &VAO);
					glGenBuffers(1, &VBO);
					glBindVertexArray(VAO);
					glBindBuffer(GL_ARRAY_BUFFER, VBO);
					glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
					glEnableVertexAttribArray(0);
					glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindVertexArray(0);

					TextShader->Use();
					ProjectionLocation = glGetUniformLocation(TextShader->GetID(), "projection");
					ColorLocation = glGetUniformLocation(TextShader->GetID(), "textColor");
					TextShader->UnUse();

					return 0;
				}

				// --------------------------------------------------
				// render line of text

				void RenderText(int SCR_WIDTH, int SCR_HEIGHT, const std::string& text, float x, float y, float scale, glm::vec3 color)
				{

					// activate corresponding render state	

					glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));

					TextShader->Use();
					glUniformMatrix4fv(ProjectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
					glUniform3f(ColorLocation, color.x, color.y, color.z);
					glActiveTexture(GL_TEXTURE0);
					glBindVertexArray(VAO);

					float vertices2[6][4] = { 0 };

					// iterate through all characters

					std::string::const_iterator c;

					for (c = text.begin(); c != text.end(); c++)
					{
						Character ch = Characters[*c];

						float xpos = x + ch.Bearing.x * scale;
						float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

						float w = ch.Size.x * scale;
						float h = ch.Size.y * scale;

						// update VBO for each character

						vertices2[0][0] = xpos;     vertices2[0][1] = ypos + h; vertices2[0][2] = 0; vertices2[0][3] = 0;
						vertices2[1][0] = xpos;     vertices2[1][1] = ypos;     vertices2[1][2] = 0; vertices2[1][3] = 1;
						vertices2[2][0] = xpos + w; vertices2[2][1] = ypos;     vertices2[2][2] = 1; vertices2[2][3] = 1;

						vertices2[3][0] = xpos;     vertices2[3][1] = ypos + h; vertices2[3][2] = 0; vertices2[3][3] = 0;
						vertices2[4][0] = xpos + w; vertices2[4][1] = ypos;     vertices2[4][2] = 1; vertices2[4][3] = 1;
						vertices2[5][0] = xpos + w; vertices2[5][1] = ypos + h; vertices2[5][2] = 1; vertices2[5][3] = 0;

						// render glyph texture over quad
						glBindTexture(GL_TEXTURE_2D, ch.TextureID);

						// update content of VBO memory
						glBindBuffer(GL_ARRAY_BUFFER, VBO);
						glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices2), vertices2); // be sure to use glBufferSubData and not glBufferData

						glBindBuffer(GL_ARRAY_BUFFER, 0);

						// render quad
						glDrawArrays(GL_TRIANGLES, 0, 6);

						// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
						x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
					}

					glBindVertexArray(0);
					glBindTexture(GL_TEXTURE_2D, 0);
					TextShader->UnUse();

				}

				// --------------------------------------------------
				// ctor / dtor

				Text()
				{
					TextShader = nullptr;
					VAO = 0;
					VBO = 0;
					ProjectionLocation = 0;
					ColorLocation = 0;
					memset(Characters, 0, 1024 * sizeof(Character));
				}

				~Text()
				{
					if (TextShader) { delete TextShader; TextShader = 0; }
					if (VBO) { glDeleteBuffers(1, &VBO); VBO = 0; }
					if (VAO) { glDeleteVertexArrays(1, &VAO); VAO = 0; }
				}

		};
	}
}
