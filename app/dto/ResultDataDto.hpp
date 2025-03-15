#ifndef RESULTDATADTO_hpp
#define RESULTDATADTO_hpp

#include "oatpp/macro/codegen.hpp"
#include "oatpp/Types.hpp"
#include <oatpp/web/protocol/http/Http.hpp>
#include <oatpp/web/protocol/http/outgoing/Response.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 *  Data Transfer Object. Object containing fields only.
 *  Used in API for serialization/deserialization and validation
 */
template <class T>
class ResultDataDto final : public oatpp::DTO
{
private:
  typedef oatpp::web::protocol::http::outgoing::Response OutgoingResponse;
  typedef oatpp::web::protocol::http::Status Status;

public:
  DTO_INIT(ResultDataDto, DTO)

  DTO_FIELD_INFO(status)
  {
    info->description = "Short status text";
  }

  DTO_FIELD(String, status);

  DTO_FIELD_INFO(code)
  {
    info->description = "Status code";
  }

  DTO_FIELD(Int32, code);

  DTO_FIELD_INFO(message)
  {
    info->description = "Verbose message";
  }

  DTO_FIELD(String, message);

  DTO_FIELD_INFO(data)
  {
    info->description = "Response data";
  }

  DTO_FIELD(T, data);

private:
  static Object<ResultDataDto> create(Status code, const String& message, T data)
  {
    auto resp = ResultDataDto<T>::createShared();
    resp->code = code.code;
    resp->status = code.description;
    resp->message = message;
    resp->data = data;
    return resp;
  }

public:
  bool isSuccess() const
  {
    return code == Status::CODE_200.code;
  }

  bool isFail() const
  {
    return !isSuccess();
  }

public:
  static Object<ResultDataDto<T>> OK()
  {
    return OK(nullptr);
  }

  static Object<ResultDataDto<T>> OK(T data)
  {
    return OK("success", data);
  }

  static Object<ResultDataDto<T>> OK(const String& message, T data)
  {
    return OK(Status::CODE_200, message, data);
  }

  static Object<ResultDataDto<T>> OK(Status code, const String& message, T data)
  {
    return create(code, message, data);
  }

  static Object<ResultDataDto<T>> FAIL()
  {
    return FAIL("fail");
  }

  static Object<ResultDataDto<T>> FAIL(const String& message)
  {
    return FAIL(message, nullptr);
  }

  static Object<ResultDataDto<T>> FAIL(const String& message, T data)
  {
    return FAIL(Status::CODE_500, message, data);
  }

  static Object<ResultDataDto<T>> FAIL(Status code, const String& message)
  {
    return create(code, message, nullptr);
  }

  static Object<ResultDataDto<T>> FAIL(Status code, const String& message, T data)
  {
    return create(code, message, data);
  }
};

#include OATPP_CODEGEN_END(DTO)

#endif /* RESULTDATADTO_hpp */
