#pragma once

#include <chrono>
#include <memory>
#include <system_error>

#include <asio/deadline_timer.hpp>

#include "state.hpp"

#include "cocaine/detail/service/node/forwards.hpp"

namespace cocaine {
namespace detail {
namespace service {
namespace node {
namespace slave {
namespace state {

class spawn_t : public state_t, public std::enable_shared_from_this<spawn_t> {
    std::shared_ptr<machine_t> slave;

    asio::deadline_timer timer;
    std::unique_ptr<api::cancellation_t> handle;

public:
    explicit spawn_t(std::shared_ptr<machine_t> slave);

    auto name() const noexcept -> const char*;
    auto cancel() -> void;
    auto terminate(const std::error_code& ec) -> void;

    auto spawn(unsigned long timeout) -> void;

private:
    auto on_spawn(std::chrono::high_resolution_clock::time_point start) -> void;
    auto on_timeout(const std::error_code& ec) -> void;
    friend class slave::spawn_handle_t;
};

}  // namespace state
}  // namespace slave
}  // namespace node
}  // namespace service
}  // namespace detail
}  // namespace cocaine
