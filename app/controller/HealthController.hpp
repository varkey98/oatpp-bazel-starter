//
// Created by Caesar on 2025/3/15.
//

#ifndef HEALTHCONTROLLER_HPP
#define HEALTHCONTROLLER_HPP


#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/macro/codegen.hpp"
#include "oatpp/macro/component.hpp"
#include "dto/ResultDataDto.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen

/**
 * Sample Api Controller.
 */
class HealthController : public oatpp::web::server::api::ApiController
{
public:
  /**
   * Constructor with object mapper.
   * @param apiContentMappers - mappers used to serialize/deserialize DTOs.
   */
  HealthController(OATPP_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>, apiContentMappers))
    : ApiController(apiContentMappers)
  {
  }

public:
  ENDPOINT("GET", "health/check", healthCheck)
  {
    const auto result = ResultDataDto<oatpp::Any>::OK("health", nullptr);
    return createDtoResponse(Status::CODE_200, result);
  }

  // TODO Insert Your endpoints here !!!
};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen


#endif //HEALTHCONTROLLER_HPP
