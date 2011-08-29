#ifndef __BER_INTERNAL_BUFFER_H__
#define __BER_INTERNAL_BUFFER_H__

#include <stdlib.h>

namespace ber {
namespace internal {

// Buffer allocates (or takes ownership) of memory and holds on to it.
// Each object using buffer should ref it. When releasing the object -- unref.
// When ref count is 0 -- object destroys itself. On creation ref count is 0.
// Buffer is supposed to be created on the heap.
// Buffer is not thread safe.
// Buffer does not make any bounds checks
class Buffer {
 public:
  Buffer(int size);
  Buffer(char* data, int size);

  // Increments ref counter.
  void Ref();

  // Decrements ref counter. If it's 0 or negative -- deletes 'this'.
  void Unref();

  // Returns identical buffer with identical contents.
  // The caller takes ownership of the Buffer object.
  Buffer* Clone() const;

  // Copies the contents from the given data buffer.
  // Buffer will copy min(size, num) bytes.
  void CopyFrom(char* data, size_t num);

  // Initializes the data buffer with given value.
  void Initialize(char val = 0);

  char* data() { return data_; }
  const char* data() const { return data_; }
  int size() const { return size_; }

 private:
  const int size_;
  char* const data_;
  int ref_count_;


  // Make object not possible to create on stack.
  ~Buffer();

  // Prohibit stuff we don't care about.
  Buffer(const Buffer&);
  Buffer& operator=(const Buffer&);
};

}  // namespace internal
}  // namespace ber

#endif  // __BER_INTERNAL_BUFFER_H__
