#pragma once
#include <appbase/application.hpp>

namespace eosio {
	using namespace appbase;

	class tcp_plugin : public appbase::plugin<tcp_plugin> {
	public:
		tcp_plugin();
		virtual ~tcp_plugin();
	 
		APPBASE_PLUGIN_REQUIRES()
		virtual void set_program_options(options_description&, options_description& cfg) override;
	 
		void plugin_initialize(const variables_map& options);
		void plugin_startup();
		void plugin_shutdown();
		void add_callback_msg(std::function<void(boost::asio::ip::tcp::socket* const, std::stringstream)> callback);
		void add_callback_disconnect(std::function<void(boost::asio::ip::tcp::socket* const)> callback);
		void send(boost::asio::ip::tcp::socket* const socket, std::string string);

	private:
		class tcp_plugin_impl* my;
	};
}
