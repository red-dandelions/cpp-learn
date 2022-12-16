#include "examples/grpc/hello.grpc.pb.h"

using namespace vscode::examples::grpc;

class HelloImpl : public HelloService::Service {
 public:
  explicit HelloImpl(){};
  ::grpc::Status function(::grpc::ServerContext* context,
                          const ::vscode::examples::grpc::Request* request,
                          ::vscode::examples::grpc::Response* response) override {
    response->set_result("hello, I am impl...");
    return ::grpc::Status();
  }
};