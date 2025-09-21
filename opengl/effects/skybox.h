#pragma once

////////////////////////////////////////////////////////////////////////////////////
//	This source file is part of v71's engine
//
//	Copyright (c) 2011-2050 v71 
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.

////////////////////////////////////////////////////////////
// skybox class

namespace vml
{
    namespace opengl
    {
        class SkyBox
        {
            private:

                // ---------------------------------------------------------
                // cube positions          

                inline static float skyboxVertices[] =
                {
                    -1.0f,  1.0f, -1.0f,
                    -1.0f, -1.0f, -1.0f,
                     1.0f, -1.0f, -1.0f,
                     1.0f, -1.0f, -1.0f,
                     1.0f,  1.0f, -1.0f,
                    -1.0f,  1.0f, -1.0f,

                    -1.0f, -1.0f,  1.0f,
                    -1.0f, -1.0f, -1.0f,
                    -1.0f,  1.0f, -1.0f,
                    -1.0f,  1.0f, -1.0f,
                    -1.0f,  1.0f,  1.0f,
                    -1.0f, -1.0f,  1.0f,

                     1.0f, -1.0f, -1.0f,
                     1.0f, -1.0f,  1.0f,
                     1.0f,  1.0f,  1.0f,
                     1.0f,  1.0f,  1.0f,
                     1.0f,  1.0f, -1.0f,
                     1.0f, -1.0f, -1.0f,

                    -1.0f, -1.0f,  1.0f,
                    -1.0f,  1.0f,  1.0f,
                     1.0f,  1.0f,  1.0f,
                     1.0f,  1.0f,  1.0f,
                     1.0f, -1.0f,  1.0f,
                    -1.0f, -1.0f,  1.0f,

                    -1.0f,  1.0f, -1.0f,
                     1.0f,  1.0f, -1.0f,
                     1.0f,  1.0f,  1.0f,
                     1.0f,  1.0f,  1.0f,
                    -1.0f,  1.0f,  1.0f,
                    -1.0f,  1.0f, -1.0f,

                    -1.0f, -1.0f, -1.0f,
                    -1.0f, -1.0f,  1.0f,
                     1.0f, -1.0f, -1.0f,
                     1.0f, -1.0f, -1.0f,
                    -1.0f, -1.0f,  1.0f,
                     1.0f, -1.0f,  1.0f
                };

                // ---------------------------------------------------------

                GLuint  VAO;
                GLuint  ID;
                GLuint  skyboxVBO;
                GLuint  cubemapTexture;
                GLuint	SkyBoxSamplerLocation;
                GLuint	ViewLocation;
                GLuint	ProjectionLocation;
                vml::shaders::GlShaderProgram* Shader;

                // ---------------------------------------------------------

                bool UploadTexture(const std::string& filename, uint32_t i)
                {
                    int width, height, nrChannels;
                    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
                    if (data) {
                        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                        stbi_image_free(data);
                        return true;
                    }
                    std::cout << "Cubemap texture failed to load at path: " << filename << std::endl;
                    stbi_image_free(data);
                    return false;
                }

            public:

                // ---------------------------------------------------------

                void Init(const std::string& rightimage,
                    const std::string& leftimage,
                    const std::string& topimage,
                    const std::string& bottomimage,
                    const std::string& frontimage,
                    const std::string& backimage)
                {
                    // load shader
                    const std::string fulldebugpath = vml::utils::GlobalPaths::GetInstance()->GetFullDebugPath();
                    const std::string filename = fulldebugpath + "/shaders/skybox.shd";
                    Shader = new vml::shaders::GlShaderProgram(filename, {});

                    // skybox VAO
                    glGenVertexArrays(1, &VAO);
                    glGenBuffers(1, &skyboxVBO);
                    glBindVertexArray(VAO);
                    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
                    glEnableVertexAttribArray(0);
                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

                    // load textures
                    glGenTextures(1, &cubemapTexture);
                    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

                    // flip texture upside down 
                    stbi_set_flip_vertically_on_load(false);

                    // loads a cubemap texture from 6 individual texture faces
                    // order:
                    // +X (right)
                    // -X (left)
                    // +Y (top)
                    // -Y (bottom)
                    // +Z (front) 
                    // -Z (back)

                    UploadTexture(rightimage, 0);
                    UploadTexture(leftimage, 1);
                    UploadTexture(topimage, 2);
                    UploadTexture(bottomimage, 3);
                    UploadTexture(frontimage, 4);
                    UploadTexture(backimage, 5);

                    // restore stbi flag
                    stbi_set_flip_vertically_on_load(true);

                    // set opengl parameters for cube map
                    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

                    // shader configuration
                    ID = Shader->GetID();
                    glUseProgram(ID);
                    SkyBoxSamplerLocation = glGetUniformLocation(ID, "skybox");
                    ViewLocation = glGetUniformLocation(ID, "view");
                    ProjectionLocation = glGetUniformLocation(ID, "projection");
                }

                // ---------------------------------------------------------
                // draw skybox 

                void Draw(vml::views::View* view) const
                {
                    // change depth function so depth test passes when values are equal to depth buffer's content
                    glDepthFunc(GL_LEQUAL);

                    glUseProgram(ID);

                    // remove translation from the view matrix
                    glm::mat4 view_matrix = view->GetView();
                    view_matrix[3][0] = 0.0f;
                    view_matrix[3][1] = 0.0f;
                    view_matrix[3][2] = 0.0f;

                    glUniform1i(SkyBoxSamplerLocation, 0);
                    glUniformMatrix4fv(ViewLocation, 1, GL_FALSE, &view_matrix[0][0]);
                    glUniformMatrix4fv(ProjectionLocation, 1, GL_FALSE, &view->GetProjection()[0][0]);

                    // skybox cube
                    glBindVertexArray(VAO);
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                    glBindVertexArray(0);

                    // set depth function back to default
                    glDepthFunc(GL_LESS);
                }

                // ---------------------------------------------------------
                // ctor / dtor

                SkyBox()
                {
                    Shader = nullptr;
                    ID = 0;
                    VAO = 0;
                    skyboxVBO = 0;
                    cubemapTexture = 0;
                    SkyBoxSamplerLocation = 0;
                    ViewLocation = 0;
                    ProjectionLocation = 0;
                }

                ~SkyBox()
                {
                    glDeleteVertexArrays(1, &VAO);
                    glDeleteBuffers(1, &skyboxVBO);
                    vml::os::SafeDelete(Shader);
                }

        };

    }
}
