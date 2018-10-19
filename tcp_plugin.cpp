#include <eosio/tcp_plugin/tcp_plugin.hpp>
#include <eosio/chain_plugin/chain_plugin.hpp>
#include <eosio/chain/plugin_interface.hpp>
#include <fc/io/json.hpp>
#include <sstream>
#include "server.hpp"

namespace eosio {
	static appbase::abstract_plugin& _tcp_plugin = app().register_plugin<tcp_plugin>();

	class tcp_plugin_impl {
	private:
		chain_plugin& chain_plugin_ref;
		tcp_server server;
		std::vector<std::function<void(boost::asio::ip::tcp::socket* const, std::stringstream)>> callbacks_msg;
		std::vector<std::function<void(boost::asio::ip::tcp::socket* const)>> callbacks_disconnect;

	public:
		tcp_plugin_impl(uint16_t port) :
			chain_plugin_ref(app().get_plugin<chain_plugin>()),
			server(app().get_io_service(), port)
		{
			this->server.on_message([this](boost::asio::ip::tcp::socket* const socket, std::string string) {
				std::for_each(this->callbacks_msg.begin(), this->callbacks_msg.end(), [&](const auto& callback) {
					callback(socket, std::stringstream(string));
				});
			});
			this->server.on_disconnect([this](boost::asio::ip::tcp::socket* const socket) {
				std::for_each(this->callbacks_disconnect.begin(), this->callbacks_disconnect.end(), [&](const auto& callback) {
					callback(socket);
				});
			});
		}

		void add_callback_msg(std::function<void(boost::asio::ip::tcp::socket* const, std::stringstream)> callback) {
			this->callbacks_msg.push_back(callback);
		}

		void add_callback_disconnect(std::function<void(boost::asio::ip::tcp::socket* const)> callback) {
			this->callbacks_disconnect.push_back(callback);
		}

		void send(boost::asio::ip::tcp::socket* const socket, std::string string) {
			this->server.send(socket, string);
		}

		~tcp_plugin_impl() {
		}
	};

	tcp_plugin::tcp_plugin() {}

	tcp_plugin::~tcp_plugin() {}

	void tcp_plugin::set_program_options(options_description&, options_description& cfg) {
		cfg.add_options()
			("tcp-plugin-port", bpo::value<uint16_t>()->default_value(56732),
			"Port to listen to");
	}

	void tcp_plugin::plugin_initialize(const variables_map& options) {
		uint16_t port = options["tcp-plugin-port"].as<uint16_t>();
		ilog("starting tcp_plugin at port " + std::to_string(port));
		this->my = new tcp_plugin_impl(port);
	}

	void tcp_plugin::plugin_startup() {}

	void tcp_plugin::plugin_shutdown() {
		delete this->my;
	}

	void tcp_plugin::add_callback_msg(std::function<void(boost::asio::ip::tcp::socket* const, std::stringstream)> callback) {
		this->my->add_callback_msg(callback);
	}

	void tcp_plugin::add_callback_disconnect(std::function<void(boost::asio::ip::tcp::socket* const)> callback) {
		this->my->add_callback_disconnect(callback);
	}

	void tcp_plugin::send(boost::asio::ip::tcp::socket* const socket, std::string string) {
		this->my->send(socket, string);
	}
}
