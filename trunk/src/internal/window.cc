#include "window.h"

#include <glog/logging.h>

#include "buffer.h"

namespace ber {
namespace internal {

Window::Window(Buffer* buf, size_t offset, size_t width)
    : buffer_(CHECK_NOTNULL(buf)),
      offset_(offset),
      end_(offset + width) {
  CHECK_GT(width, 0) << "Window must be non-empty.";
  CHECK_LT(offset_, buf->size()) << "Window must start before the buffer end.";
  CHECK_LE(end_, buf->size())
      << "Window must not end beyond buffer.";

  VLOG(1) << "Created window from pos: " << offset_ << " with width: "
          << width << " ending at: "<< end_ << " buffer size: "
          << buffer_->size() << ".";
}

Window::~Window() {
  buffer_->Unref();
}

void Window::WriteByte(size_t pos, char value) {
  CHECK_LT(offset_ + pos, end_);
  buffer_->data()[offset_ + pos] = value;
}


void Window::WriteArray(size_t pos, const char* values, size_t len) {
  // TODO(vagababov): check overflows?
  CHECK_LT(offset_ + pos, end_);
  CHECK_LE(offset_ + pos + len, end_);
  CHECK_NOTNULL(values);

  if (len == 0) return;
  memmove(buffer_->data() + offset_ + pos, values, len);
}

char Window::ReadByte(size_t pos) const {
  CHECK_LT(offset_ + pos, end_);
  return buffer_->data()[offset_ + pos];
}

void Window::ReadArray(size_t pos, char* values, size_t len) const {
  CHECK_NOTNULL(values);
  CHECK_LT(offset_ + pos, end_);
  CHECK_LE(offset_ + pos + len, end_);
  memmove(values, buffer_->data() + offset_ + pos, len);
}

Window* Window::CreatePortWindow(size_t offset, size_t width) const {
  CHECK_GT(width, 0) << "Window must be non-empty.";
  const int this_width = this->width();
  CHECK_LT(width, this_width) << "Port window must be smalled than origin.";
  CHECK_LE(offset + width, this_width);
  buffer_->Ref();
  return new Window(buffer_, offset_ + offset, width);
}

Window* Window::Clone() const {
  buffer_->Ref();
  return new Window(buffer_, offset_, width());
}

}  // namespace internal
}  // namespace ber
