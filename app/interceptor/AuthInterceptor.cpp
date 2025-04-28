
#include "AuthInterceptor.hpp"

#include <iostream>

std::shared_ptr<AuthInterceptor::OutgoingResponse> AuthInterceptor::intercept(const std::shared_ptr<IncomingRequest>& request) {

    auto headers = request->getHeaders().getAll();
    for(auto itr = headers.begin(); itr != headers.end(); itr++) {
        std::cout<<itr->first.std_str()<<": "<<itr->second.std_str()<<"\n";
    }

    return nullptr;
//   throw oatpp::web::protocol::http::HttpError(oatpp::web::protocol::http::Status::CODE_401, "Unauthorized", {});

}

AuthInterceptor::AuthInterceptor() {
    traceable_libtraceable_config libtraceable_config = init_libtraceable_config();
    libtraceable_config.log_config.mode = TRACEABLE_LOG_STDOUT;
    libtraceable_config.log_config.level = TRACEABLE_LOG_LEVEL_TRACE;
    libtraceable_config.blocking_config.enabled = 1;
    libtraceable_config.blocking_config.modsecurity_config.enabled = 1;
    libtraceable_config.blocking_config.evaluate_body = 1;
    libtraceable_config.blocking_config.rb_config.enabled = 1;
    libtraceable_config.blocking_config.skip_internal_request = 0;
    libtraceable_config.remote_config.enabled = 1;
    libtraceable_config.remote_config.remote_endpoint =
        "host.docker.internal:5441";
    libtraceable_config.remote_config.poll_period_sec = 2;
    libtraceable_config.sampling_config.enabled = 1;

    TRACEABLE_RET ret = traceable_new_libtraceable(libtraceable_config, &libtraceable);
    if (ret != TRACEABLE_SUCCESS) {
        std::cout << "ERROR: new libtraceable" << std::endl;
        return;
    }

    ret = traceable_start_libtraceable(libtraceable);
    if (ret != TRACEABLE_SUCCESS) {
        std::cout << "ERROR: start libtraceable" << std::endl;
        return;
      }
}