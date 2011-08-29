#include "buffer.h"
#include "window.h"
#include <glog/logging.h>
#include <gflags/gflags.h>
#include "gtest/gtest.h"

using ber::internal::Buffer;
using ber::internal::Window;
namespace {
 // Tests that window can be created.
 TEST(WindowTest, NormalCreation) {
  Buffer* b = new Buffer(15);
  Window w(b, 5, 5);
  ASSERT_EQ(5, w.width());
 }

 TEST(WindowTest, EmptyWindowDeath) {
  Buffer* b = new Buffer(1);
  EXPECT_DEATH(Window(b, 0, 0), ".*");
 }

 // Tests death, if buffer is NULL.
 TEST(WindowTest, NullBufferDeath) {
  EXPECT_DEATH(Window(0, 0, 0), ".*");
 }

 // Tests that window can occupy the whole buffer.
 TEST(WindowTest, FullBufferWindow) {
  Buffer* b = new Buffer(10);
  Window w(b, 0, 10);
  ASSERT_EQ(b->size(), w.width());
 }

 // Tests that window cannot be bigger than buffer.
 TEST(WindowTest, WindowOutOfBufferBounds) {
  Buffer * b = new Buffer(10);
  EXPECT_DEATH(Window(b, 0, 11), ".*");
 }

 // Tests that window cannot start after buffer end.
 TEST(WindowTest, WindowCreatedAfterBufferEnd) {
  Buffer* b = new Buffer(10);
  EXPECT_DEATH(Window(b, 10, 1), ".*");
  EXPECT_DEATH(Window(b, 11, 1), ".*");
  // -1 converted to size_t is huge.
  EXPECT_DEATH(Window(b, (size_t)-1, 1), ".*");
 }

 // Normal read/write test
 TEST(WindowTest, ReadWrite) {
   Buffer* b = new Buffer(10);
   Window w(b, 1, 6);
   for (int i = 0; i < w.width(); ++i) {
    w.WriteByte(i, i);
   }
   for (int i = 0; i < w.width(); ++i) {
    EXPECT_EQ(i, w.ReadByte(i));
   }
 }

 // Tests that arrays are properly written.
 TEST(WindowTest, ReadWriteArray) {
  const char data[] = {1, 2, 3, 4};
  Buffer* b = new Buffer(10);
  Window w(b, 1, 4);
  w.WriteArray(0, data, 4);
  char data_out[4];
  w.ReadArray(0, data_out, 4);
  for (size_t i = 0; i < sizeof(data) / sizeof(data[0]); ++i) {
    EXPECT_EQ(data[i], data_out[i]);
  }
 }

 // Tests that death on writing out of bounds happens.
 TEST(WindowTest, DeathOnWriteOutOfBounds) {
  Buffer* b = new Buffer(10);
  Window w(b, 5, 5);
  EXPECT_DEATH(w.WriteByte(5, 1), ".*");
 }

 // Test death on read byte out of bounds.
 TEST(WindowTest, DeathOnReadOutOfBounds) {
  Buffer* b = new Buffer(10);
  Window w(b, 5, 5);
  EXPECT_DEATH(w.ReadByte(7), ".*");
 }

 TEST(WindowTest, CreatePortWindow) {
  Buffer* b = new Buffer(10);
  Window w(b, 0, 10);
  Window* port = w.CreatePortWindow(1, 9);
  EXPECT_NE(static_cast<Window*>(NULL), port);
  EXPECT_EQ(9, port->width());
  delete port;

  port = w.CreatePortWindow(5, 3);
  EXPECT_NE(static_cast<Window*>(NULL), port);
  EXPECT_EQ(3, port->width());
  EXPECT_EQ(w.buffer(), port->buffer());
  delete port;
 }

TEST(WindowTest, DeathOnPortWindowSameSize) {
  Buffer* b = new Buffer(10);
  Window w(b, 0, 10);
  EXPECT_DEATH(w.CreatePortWindow(0, 10), ".*");
}

TEST(WindowTest, BadPortWindowParams) {
  Buffer* b = new Buffer(10);
  Window w(b, 0, 10);
  // Longer than possible
  EXPECT_DEATH(w.CreatePortWindow(5, 6), ".*");
  EXPECT_DEATH(w.CreatePortWindow(5, 10), ".*");
  // 0 length.
  EXPECT_DEATH(w.CreatePortWindow(1, 0), ".*");
}

TEST(WindowTest, TestClone) {
  Buffer* b = new Buffer(10);
  Window w1(b, 1, 9);
  Window* w2 = w1.Clone();
  EXPECT_NE(static_cast<Window*>(NULL), w2);
  EXPECT_EQ(w1.width(), w2->width());
  EXPECT_EQ(w1.buffer(), w2->buffer());
}

}  // namespace

int main(int argc, char** argv) {
  google::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
