#pragma once
#include <glad/glad.h>
#include "WindowManager.h"

unsigned int createFrameBuffer();

unsigned int createTextureAttachment(int width, int height);

unsigned int createDepthTextureAttachment(int width, int height);

unsigned int createDepthBufferAttachment(int width, int height);

unsigned int createRenderBufferAttachment(int width, int height);

unsigned int createRenderBufferAttachmentDepth(int width, int height);

class FrameBufferObject {
public:
	FrameBufferObject(int W, int H);
	unsigned int FBO, renderBuffer, depthTex;
	unsigned int tex;
	int getWidth();
	int getHeight();
private:
	int W, H;
	int nColorAttachments;
	unsigned int * colorAttachments;
};

