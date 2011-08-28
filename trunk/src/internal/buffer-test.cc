#include "buffer.h"
#include <glog/logging.h>
#include <gflags/gflags.h>

DEFINE_int32(tst, 11, "just a test");


using ber::internal::Buffer;
int main(int argc, char** argv) {
  google::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();
  LOG(INFO) << "What a nice day";
  Buffer* b = new Buffer(15);
  b->Unref();
  return 0;
}
