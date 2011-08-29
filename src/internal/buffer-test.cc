#include "buffer.h"

#include <algorithm>
#include <glog/logging.h>
#include <gflags/gflags.h>
#include <new>
#include <stdlib.h>
#include <time.h>

#include "window.h"
#include "gtest/gtest.h"

using ber::internal::Buffer;

namespace {
const int kSize = 10;

// Tests regular creation of buffers.
TEST(BufferTest, Creation) {
  Buffer* b = new Buffer(kSize);
  EXPECT_EQ(kSize, b->size());
  b->Unref();
}

TEST(BufferTest, RefUnfref) {
  Buffer* b = new Buffer(kSize);
  b->Ref();
  b->Unref();
  b->Unref();
}

// Tests that cloning of buffers works properly.
TEST(BufferTest, Clone) {
  Buffer* b = new Buffer(kSize);
  srand(time(0));
  std::generate_n(b->data(), kSize, &rand);
  Buffer* c = b->Clone();
  EXPECT_EQ(kSize, c->size());
  EXPECT_NE(c->data(), b->data());
  EXPECT_EQ(0, memcmp(b->data(), c->data(), kSize));
  b->Unref();
  c->Unref();
}

TEST(BufferTest, Initialize) {
  Buffer* b = new Buffer(kSize);
  b->Initialize(11);
  for (int i = 0; i < kSize; ++i) {
    EXPECT_EQ(11, b->data()[i]);
  }
  b->Unref();
}

TEST(BufferTest, NegativeOrZeroSize) {
  try {
    new Buffer(0);
    EXPECT_TRUE(false);
  } catch (std::bad_alloc& b) {}
  try {
    new Buffer(-1);
    EXPECT_TRUE(false);
  } catch (std::bad_alloc& b) {}
}

}  // namespace


int main(int argc, char** argv) {
  google::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
