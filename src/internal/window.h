#ifndef __BER_INTERNAL_WINDOW__
#define __BER_INTERNAL_WINDOW__


#include <stdlib.h>

namespace ber {
namespace internal {

class Buffer;

// Window represents a view onto allocated Buffer.
// Window owns the buffer and will Unref it when done using it.
// Window makes bounds checks.
// Window is not thread safe.
class Window {
 public:
  // Creates a window with a given buffer from given offset with given width.
  // The buffer must be already Ref'ed.
  // Will fail on constraint violations.
  Window(Buffer* buf, size_t offset, size_t width);
  ~Window();

  // Writes single byte at the given window position.
  // CHECK's if offset is outside of window.
  void WriteByte(size_t pos, char value);

  // Writes the given array values in the window from position pos.
  // Checks for possible overruns.
  void WriteArray(size_t pos, const char* values, size_t len);

  // Reads a byte from the given position.
  // Checks for indices.
  char ReadByte(size_t pos) const;

  // Reads len bytes into values array from position pos.
  // Checks indices and that values is not null.
  // Caller must guarantee that values has enough size.
  void ReadArray(size_t pos, char* values, size_t len) const;

  // Creates a new window on the heap, which caller is responsible to delete.
  // Window is created from a given origin window with offset and width as
  // specified. Offset is the offset in regards to the origin window.
  // Will fail if parameters don't make sense.
  static Window* CreatePortWindow(const Window& origin,
                                  size_t offset, size_t width);

  // Returns the width of the window.
  int width() const { return end_ - offset_; }
private:
  Window* CreatePortWindow(size_t offset, size_t width) const;

 // "Owned" in the sense that Window will unref, when done.
 mutable Buffer* buffer_;
 const size_t offset_;
 const size_t end_;

 Window(const Window&);
 Window& operator=(const Window&);
};


}  // namespace internal
}  // namespace ber



#endif  // __BER_INTERNAL_WINDOW__
