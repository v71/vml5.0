#pragma once

namespace vml
{
	namespace debugrendering
	{
		class FrameBuffer
		{
			private:

				unsigned int FBO;
				unsigned int TextureID;
				unsigned int RBO;
				unsigned int Width;
				unsigned int Height;
				
			public:

				unsigned int getFrameTexture() const
				{
					return TextureID;
				}

				void RescaleFrameBuffer(float width, float height) const
				{
					glBindTexture(GL_TEXTURE_2D, TextureID);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TextureID, 0);
					glBindRenderbuffer(GL_RENDERBUFFER, RBO);
					glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
				}

				void Bind() const
				{
					glBindFramebuffer(GL_FRAMEBUFFER, FBO);
				}

				void Unbind() const
				{
					glBindFramebuffer(GL_FRAMEBUFFER, 0);
				}

				// --------------------------------------------------------------
				// ctor / dtor

				FrameBuffer()
				{
					FBO       = 0;
					TextureID = 0;
					RBO       = 0;
					Width     = 512;
					Height    = 512;

					glGenFramebuffers(1, &FBO);
					glBindFramebuffer(GL_FRAMEBUFFER, FBO);

					glGenTextures(1, &TextureID);
					glBindTexture(GL_TEXTURE_2D, TextureID);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TextureID, 0);

					glGenRenderbuffers(1, &RBO);
					glBindRenderbuffer(GL_RENDERBUFFER, RBO);
					glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Width, Height);
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

					if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
						std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

					glBindFramebuffer(GL_FRAMEBUFFER, 0);
					glBindTexture(GL_TEXTURE_2D, 0);
					glBindRenderbuffer(GL_RENDERBUFFER, 0);
				}

				~FrameBuffer()
				{
					if (FBO)
					{
						glDeleteFramebuffers(1, &FBO);
						FBO = 0;
					}

					if (TextureID)
					{
						glDeleteTextures(1, &TextureID);
						TextureID = 0;
					}

					if (RBO)
					{
						glDeleteRenderbuffers(1, &RBO);
						RBO = 0;
					}
				}

		};

	}
}
