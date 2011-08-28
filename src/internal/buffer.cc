#include "buffer.h"
#include <glog/logging.h>

namespace ber {
namespace internal {

Buffer::Buffer(int size)
    : size_(size),
      data_(new char[size]),
      ref_count_(1) {
  CHECK_GT(size, 0) << "Size must be positive";
}


Buffer::Buffer(char* data, int size)
  : size_(size),
    data_(CHECK_NOTNULL(data)),
    ref_count_(1) {
  CHECK_GT(size, 0) << "Size must be positive";
}

Buffer::~Buffer() {
  VLOG(1) << "Destroying buffer";
  delete [] data_;
}

void Buffer::Ref() {
  ++ref_count_;
}

void Buffer::Unref() {
  if (--ref_count_ <= 0) {
    delete this;
  }
}


}  // namespace internal
}  // namespace ber
