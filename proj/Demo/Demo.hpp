#ifndef _DEMO_HPP_
#define _DEMO_HPP_

#include <MemeAudio/Audio.hpp>
#include <MemeCore/Debug.hpp>
#include <MemeCore/Time.hpp>
#include <MemeCore/FileSystem.hpp> 
#include <MemeCore/Random.hpp>
#include <MemeCore/Thread.hpp>
#include <MemeEditor/Editor.hpp>
#include <MemeGraphics/Text.hpp>
#include <MemeGraphics/Canvas.hpp>
#include <MemeGraphics/Effect.hpp>
#include <MemeGraphics/RenderWindow.hpp>
#include <MemeGraphics/Shapes.hpp>
#include <MemeGraphics/VertexBuffer.hpp>
#include <MemeGraphics/IndexBuffer.hpp>
#include <MemeGraphics/FrameBuffer.hpp>
#include <MemeGraphics/RenderBuffer.hpp>
#include <MemeGraphics/BufferLayout.hpp>
#include <MemeGraphics/OpenGL.hpp>
#include <MemeGraphics/Model.hpp>
#include <MemeGraphics/Camera.hpp>
#include <MemeNet/Client.hpp>
#include <MemeNet/Server.hpp>
#include <MemeScript/Interpreter.hpp>
#include <MemeEditor/Editor.hpp>
#include <MemePhysics/Rigidbody.hpp>
#include <MemePhysics/Particle.hpp>

#include "DemoEvents.hpp"
#include "Settings.hpp"

// Demo
/* * * * * * * * * * * * * * * * * * * * */
namespace DEMO
{
	class Demo final : public ml::RenderWindow
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * */
		Demo();
		~Demo();

	public:
		void onEvent(const ml::Event * value) override;

	private:
		bool	loadInterpreter();
		bool	loadResources();
		bool	loadBuffers();
		bool	loadNetwork();

	private:
		void	onEnter(const EnterEvent & ev);
		void	onLoad(const LoadEvent & ev);
		void	onStart(const StartEvent & ev);
		void	onFixedUpdate(const FixedUpdateEvent & ev);
		void	onUpdate(const UpdateEvent & ev);
		void	onDraw(const DrawEvent & ev);
		void	onGui(const GuiEvent & ev);
		void	onExit(const ExitEvent & ev);

	private:
		/* * * * * * * * * * * * * * * * * * * * */

		ml::Transform	m_persp;
		ml::Transform	m_ortho;
		ml::Transform	m_camera;

		/* * * * * * * * * * * * * * * * * * * * */

		ml::VAO m_vao;
		ml::VBO m_vbo;

		using TextMap = ml::HashMap<ml::String, ml::Text>;
		TextMap m_text;

		using EffectMap = ml::HashMap<ml::String, ml::Effect>;
		EffectMap m_effects;

		/* * * * * * * * * * * * * * * * * * * * */

		ml::Material m_mat;

		bool		m_camAnimate	= true;
		ml::vec3f	m_camPos		= { 0, 1, 10 };
		float		m_camSpd		= 1.f;
		
		ml::vec3f	m_lightPos		= { 0, 1, 30 };
		ml::vec4f	m_lightCol		= ml::Color::LightYellow;
		float		m_ambient		= 0.01f;
		float		m_specular		= 0.5f;
		int32_t		m_shininess		= 8;

		ml::vec4f	m_clearColor	= ml::Color::Gray;
		int32_t		m_fboMode		= 0;

		int32_t		m_lineMode		= -1;
		ml::vec4f	m_lineColor		= ml::Color::Red;
		float		m_lineDelta		= 1.f;
		float		m_lineSize		= 0.5f;
		int32_t		m_lineSamples	= 16;
		bool		m_animate		= true;

		/* * * * * * * * * * * * * * * * * * * * */

		bool show_ml_editor		= false;
		bool show_ml_shader		= false;
		bool show_ml_network	= false;
		bool show_imgui_demo	= false;
		bool show_imgui_metrics	= false;
		bool show_imgui_style	= false;
		bool show_imgui_about	= false;

		ml::List<ml::Uniform> m_uniforms;
		size_t m_selected = 0;

		/* * * * * * * * * * * * * * * * * * * * */

		ml::Thread *	m_thread;

		/* * * * * * * * * * * * * * * * * * * * */

		ml::Rigidbody	m_rigidbody;
		ml::Particle	m_particle;

		/* * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_DEMO_HPP_
