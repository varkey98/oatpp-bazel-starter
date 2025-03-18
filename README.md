# oatpp-bazel-starter

> on windows, openssl use 1.1.1w.

Starter project of oat++ (AKA oatpp) (bazel) application. Based on oatpp Multithreaded (Simple) API.

See more:

- [Oat++ Website](https://oatpp.io/)
- [Oat++ Github Repository](https://github.com/oatpp/oatpp)
- [Get Started](https://oatpp.io/docs/start)

## Overview

### Project layout

```
|- MODULE.bazel                          // Bazel module file 
|- app/
|    |
|    |- controller/                      // Folder containing MyController where all endpoints are declared
|    |- dto/                             // DTOs are declared here
|    |- AppComponent.hpp                 // Service config
|    |- App.cpp                          // main() is here
|    |- BUILD.bazel                      // Bazel target file for app
|
|- third-party/                          // third-party folder
|- patches                               // Patch files to apply to third-party
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
