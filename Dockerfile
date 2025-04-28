FROM ubuntu:22.04

RUN apt -y update && apt install -y curl gcc git libc6-dev build-essential

RUN curl -L https://github.com/bazelbuild/bazel/releases/download/8.2.1/bazel-8.2.1-linux-arm64 -o /bazel

RUN chmod +x /bazel

COPY . .

RUN /bazel build //app:app

CMD [ "/bazel", "run", "app:app" ]