#include "FBO.h"
#include <glad/glad.h>

FBO::FBO(float width, float height) : m_Width(width), m_Height(height)
{
	//Create the frame buffer
	glGenFramebuffers(1, &m_ID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FBO::~FBO()
{
	glDeleteFramebuffers(1, &m_ID);
}

void FBO::BindFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
	//glViewport(0, 0, m_Width, m_Height);
}

void FBO::UnbindFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glViewport(0, 0, 1280, 720);
}

void FBO::CreateColorTextureAttachment()
{
	glGenTextures(1, &m_ColorBufferID);
	glBindTexture(GL_TEXTURE_2D, m_ColorBufferID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1280, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_ColorBufferID, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void FBO::CreateDepthTextureAttachment()
{
	glGenTextures(1, &m_DepthBufferID);
	glBindTexture(GL_TEXTURE_2D, m_DepthBufferID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, 1280, 720, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_DepthBufferID, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void FBO::CreateDepthBufferAttachment()
{
	glGenRenderbuffers(1, &m_DepthBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, m_DepthBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1280, 720);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthBufferID);
}
