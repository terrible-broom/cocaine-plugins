#pragma once

#include "cocaine/vicodyn/queue/invocation.hpp"
#include "cocaine/vicodyn/stream.hpp"

#include <asio/ip/tcp.hpp>

#include <future>

namespace cocaine {
namespace vicodyn {

class peer_t : public std::enable_shared_from_this<peer_t> {
public:
    typedef std::function<void(std::future<void>)> error_callback_t;
    typedef std::function<void()> connect_callback_t;

    ~peer_t();

    peer_t(context_t& context, asio::io_service& loop, std::vector<asio::ip::tcp::endpoint>, std::string uuid, bool local);

    auto invoke(const io::aux::decoded_message_t& incoming_message,
                const io::graph_node_t& protocol,
                stream_ptr_t backward_stream) -> stream_ptr_t;

    auto connect() -> void;

    auto connected() -> bool;

    auto on_error(error_callback_t) -> void;

    auto on_connect(connect_callback_t) -> void;

    auto step_down() -> std::unique_ptr<queue::invocation_t>;

private:
    context_t& context;
    asio::io_service& loop;

    std::string uuid;
    bool local;
    std::chrono::system_clock::time_point freezed_till;
    std::chrono::system_clock::time_point last_used;

    std::vector<asio::ip::tcp::endpoint> endpoints;
    std::unique_ptr<logging::logger_t> logger;
    std::unique_ptr<queue::invocation_t> queue;

    connect_callback_t connect_cb;
    error_callback_t error_cb;
};

} // namespace vicodyn
} // namespace cocaine
