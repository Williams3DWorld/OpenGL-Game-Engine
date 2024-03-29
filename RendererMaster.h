#ifndef __RENDERER_MASTER_H__
#define __RENDERER_MASTER_H__

#include "Context.h"	// Include context for setting up OpenGL
#include "Deferred.h"	// Include the deferred passes for rendering in screenspce
#include "Editor.h"		// Include the editor compnents


Context	_opengl_context;	// Our OpenGL context class needs to be globally accessed


// This class will be for rendering the OpenGL world
class RendererMaster
{
public:
	// Initialise buffers and glsl shaders
	static inline void Initialise()
	{
		Mouse::SetCursorPosition(_pd_width / 2, _pd_height / 2);	// Center our mouse position);	// Set cursor to center

		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);	// Enable depth test
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);		// Enable seamless cubemap for hardware acceleration
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);	// Enable alpha blending

		Editor::Initialise();	// Initialise the editor

		glDisable(GL_BLEND);
		Deferred::Initialise(_pd_width, _pd_height);	// Initialise the deferred renderer

		// Load player content
		DataIO::Import::WavefrontObjI(Deferred::shaders[0]->GetProgram(), "DefaultPlayer.obj");
		Content::_map->GetPlayerController()->AssignAttachment(new AttachmentMesh(Deferred::shaders[0]->GetProgram(), Content::_meshes[0], glm::vec3(0.0f)));
	}

	// Destroy the render data
	static inline void Destroy()
	{
		_opengl_context.Destroy();	// Free our context data
	}

	// Update our object's logic with delta time
	static inline void Update(double& delta)
	{
		if (!UI::_controls[0]->active)	// If the console is NOT active...
			Deferred::Update(delta);	// Update the world through deferred passes
		
		Editor::Update(delta);	// Update the editor components
	}

	// Render our OpenGL context by clearing buffers and binding buffer objects to the gpu
	static inline void Render()
	{
		Deferred::Render();		// Render the world through deferred passes
		Editor::Render();	// Render the editor components

		_opengl_context.Swap();		// Swap our window context
	}
};

#endif