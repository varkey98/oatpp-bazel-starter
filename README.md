# oatpp-bazel-starter

Starter project of oat++ (AKA oatpp) (bazel) application. Based on oatpp Multithreaded (Simple) API.

See more:

- [Oat++ Website](https://oatpp.io/)
- [Oat++ Github Repository](https://github.com/oatpp/oatpp)
- [Get Started](https://oatpp.io/docs/start)

## Overview

### Project layout

```
|- CMakeLists.txt                        // projects CMakeLists.txt
|- app/
|    |
|    |- controller/                      // Folder containing MyController where all endpoints are declared
|    |- dto/                             // DTOs are declared here
|    |- AppComponent.hpp                 // Service config
|    |- App.cpp                          // main() is here
|
|- third-party/                          // third-party folder
|- utility/install-oatpp-modules.sh      // utility script to install required oatpp-modules.  
```

---

### Build and Run

#### Using Bazel

**Requires**

- `bazel` installed. choose one:
    - https://github.com/bazelbuild/bazel
    - https://github.com/bazelbuild/bazelisk

```shell
# build and run app
bazel run //app:app
```

check health

```shell
# use curl 
curl "http://localhost:8000/health/check"
```

show response :

```json
{
  "status": "OK",
  "code": 200,
  "message": "health",
  "data": null
}
```

# openssl windows build failure

Currently, the Windows version of `OpenSSL` is not supported. You need to replace it manually.

like：

```bazel 
new_local_repository(
    name = "openssl-windows",
    build_file_content = """
cc_library(
    name = "openssl",
    srcs = [
        "lib/libssl.lib",
        "lib/libcrypto.lib"
    ],
    hdrs = glob(["include/openssl/**"]),
    includes=["include"],
    visibility = ["//visibility:public"],
    target_compatible_with = select({
        "@platforms//os:windows": [],
        "//conditions:default": ["@platforms//:incompatible"],
    }),
)
""",
    path = "c:/openssl/",
)
```

update [third-party/openssl/BUILD.bazel](third-party/openssl/BUILD.bazel)中的`openssl`: 

```
cc_library(
    name = "openssl",
    visibility = ["//visibility:public"],
    deps = select({
        "@platforms//os:windows": [
            "@opensl-windows//:openssl",
        ],
        "//conditions:default": [
            ":openssl-build",
        ],
    }),
)
```

This allows you to skip the compilation of `openssl` and use the precompiled `libssl.lib` and `libcrypto.lib`.