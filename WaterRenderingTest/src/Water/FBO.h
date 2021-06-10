#pragma once
class FBO
{
public:
	FBO(float width, float height);
	~FBO();
	void BindFramebuffer();
	void UnbindFramebuffer();
	void CreateColorTextureAttachment();	//Creates a texture buffer for the color buffer
	void CreateDepthTextureAttachment();	//Creates a texture buffer for the depth buffer (to be sampled in fragment shader)
	void CreateDepthBufferAttachment();		//Creates a regular depth buffer attachment
	unsigned int m_ID, m_ColorBufferID, m_DepthBufferID;
	float m_Width, m_Height;
};

