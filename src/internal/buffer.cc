#include "buffer.h"

#include <algorithm>
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

Buffer* Buffer::Clone() const {
  Buffer* ret = new Buffer(size());
  memcpy(ret->data(), data_, size());
  return ret;
}

void Buffer::Initialize(char val) {
  memset(data_, val, size_);
}

void Buffer::CopyFrom(char* data, size_t num) {
  CHECK_NOTNULL(data);
  memcpy(data_, data, std::min<size_t>(num, size_));
}

}  // namespace internal
}  // namespace ber
