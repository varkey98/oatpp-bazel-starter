FROM rockylinux:9.3

# RUN apt -y update && apt install -y curl gcc git libc6-dev build-essential

RUN dnf install -y gcc git glibc-devel perl

RUN dnf groupinstall -y "Development Tools"

RUN curl -L https://github.com/bazelbuild/bazel/releases/download/8.2.1/bazel-8.2.1-linux-arm64 -o /bazel

RUN chmod +x /bazel

COPY . .

RUN /bazel build --verbose_failures //app:app 

CMD [ "/bazel", "run", "app:app" ]