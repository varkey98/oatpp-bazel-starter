
#include "AuthInterceptor.hpp"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <unordered_map>
#include <vector>
#include <string>

std::shared_ptr<AuthInterceptor::OutgoingResponse> AuthInterceptor::intercept(const std::shared_ptr<IncomingRequest> &request)
{

    std::unordered_map<std::string, std::string> attributes;
    auto headers = request->getHeaders().getAll();
    for (auto itr = headers.begin(); itr != headers.end(); itr++)
    {
        std::string key = itr->first.std_str();
        std::transform(key.begin(), key.end(), key.begin(), [](unsigned char c)
                       { return std::tolower(c); });
        attributes.emplace("http.request.header." + key, itr->second.std_str());
    }

    std::string body = request->readBodyToString();
    attributes.emplace("http.request.body", body);

    std::vector<traceable_attribute> input_array;
    for (auto itr = attributes.begin(); itr != attributes.end(); ++itr)
    {
        traceable_attribute attr;
        attr.key = itr->first.c_str();
        attr.value = itr->second.c_str();
        input_array.push_back(attr);
    }

    traceable_attributes input_attributes;
    input_attributes.count = input_array.size();
    input_attributes.attribute_array = &input_array[0];
    traceable_process_request_result result;
    TRACEABLE_RET ret = traceable_process_request(libtraceable, input_attributes, &result);

    ret = traceable_delete_process_request_result_data(result);
    if (ret != TRACEABLE_SUCCESS)
    {
        std::cout << "ERROR: delete process request result" << std::endl;
        return nullptr;
    }

    if (result.block)
    {
        throw oatpp::web::protocol::http::HttpError(oatpp::web::protocol::http::Status::CODE_403, "Access Forbidden", {});
    }

    return nullptr;
}

AuthInterceptor::AuthInterceptor()
{
    std::string remote_endpoint = std::string(getenv("TA_REMOTE_CONFIG_ENDPOINT"));
    std::string service_name = std::string(getenv("TA_SERVICE_NAME"));

    std::string log_level = std::string(getenv("TA_LOG_LEVEL"));
    TRACEABLE_LOG_LEVEL level = TRACEABLE_LOG_LEVEL_INFO;
    if (log_level == "Trace")
    {
        level = TRACEABLE_LOG_LEVEL_TRACE;
    }
    if (log_level == "Debug")
    {
        level = TRACEABLE_LOG_LEVEL_DEBUG;
    }

    std::string metrics_enabled = std::string(getenv("TA_METRICS_CONFIG_ENABLED"));
    int metrics_config_enabled = 0;
    if (metrics_enabled == "true")
    {
        metrics_config_enabled = 1;
    }

    std::string metrics_exporter_enabled = std::string(getenv("TA_METRICS_CONFIG_EXPORTER_ENABLED"));
    int metrics_exporter_enabled_val = 0;
    if (metrics_exporter_enabled == "true")
    {
      metrics_exporter_enabled_val = 1;
    }

    std::string reporting_endpoint = std::string(getenv("TA_REPORTING_ENDPOINT"));

    traceable_libtraceable_config libtraceable_config = init_libtraceable_config();
    libtraceable_config.log_config.mode = TRACEABLE_LOG_STDOUT;
    libtraceable_config.log_config.level = level;
    libtraceable_config.blocking_config.enabled = 1;
    libtraceable_config.blocking_config.modsecurity_config.enabled = 1;
    libtraceable_config.blocking_config.evaluate_body = 1;
    libtraceable_config.blocking_config.rb_config.enabled = 1;
    libtraceable_config.blocking_config.skip_internal_request = 0;
    libtraceable_config.remote_config.enabled = 1;
    libtraceable_config.remote_config.poll_period_sec = 30;
    libtraceable_config.remote_config.remote_endpoint = remote_endpoint.c_str();
    libtraceable_config.sampling_config.enabled = 1;
    libtraceable_config.agent_config.service_name = service_name.c_str();
    libtraceable_config.metrics_config.enabled = metrics_config_enabled;
    libtraceable_config.metrics_config.exporter.enabled = metrics_exporter_enabled_val;
    libtraceable_config.metrics_config.exporter.server.endpoint = reporting_endpoint.c_str();
    libtraceable_config.metrics_config.exporter.server.export_interval_ms = 60000;
    libtraceable_config.metrics_config.exporter.server.export_timeout_ms = 30000;

    TRACEABLE_RET ret = traceable_new_libtraceable(libtraceable_config, &libtraceable);
    if (ret != TRACEABLE_SUCCESS)
    {
        std::cout << "ERROR: new libtraceable" << std::endl;
        return;
    }

    ret = traceable_start_libtraceable(libtraceable);
    if (ret != TRACEABLE_SUCCESS)
    {
        std::cout << "ERROR: start libtraceable" << std::endl;
        return;
    }
}