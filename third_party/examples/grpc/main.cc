#include "examples/grpc/hello.grpc.pb.h"
#include "grpcpp/grpcpp.h"
#include "hello_impl.h"

#include <iostream>
#include <thread>
#include <unistd.h>

using namespace vscode::examples::grpc;

void runServer() {
  // 注册 server
  HelloImpl service;
  grpc::ServerBuilder()
      .AddListeningPort("localhost:8081", grpc::InsecureServerCredentials())
      .RegisterService(&service)
      .BuildAndStart();
  std::cout << "run server...";
  while (1)
    ;
}

void runClient() {
  auto channel = grpc::CreateChannel("localhost:8081", grpc::InsecureChannelCredentials());
  std::unique_ptr<HelloService::Stub> stub = HelloService::NewStub(channel);
  Request request;
  request.set_context("hello, I am client...");
  ::grpc::ClientContext context;
  Response response;
  auto status = stub->function(&context, request, &response);
  std::cout << "response: " << response.result() << "err_msg: " << status.error_message();
}

int main() {
  std::thread t(runServer);

  runClient();

  sleep(3);

  return 0;
}