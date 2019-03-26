#ifndef _ML_NETWORK_HUD_HPP_
#define _ML_NETWORK_HUD_HPP_

#include <MemeEditor/GUI_Window.hpp>

#define ML_NetworkHUD ml::NetworkHUD::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API NetworkHUD
		: public GUI_Window
		, public ISingleton<NetworkHUD>
	{
		friend class ISingleton<NetworkHUD>;

	private:
		NetworkHUD();
		~NetworkHUD();

	public:
		void onEvent(const IEvent * value) override;
		bool draw(bool * p_open) override;

	public:

	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_NETWORK_HUD_HPP_