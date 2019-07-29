#include "../include/buffer.h"

namespace Reverse300 {
Buffer::Buffer(int sampleRate, double maxDuration /* ms */)
    : mSampleRate(sampleRate), mCapacity(sampleRate * maxDuration / 1000.0),
      mBuffer(new double[mCapacity]{}) {}

Buffer::~Buffer() { delete[] mBuffer; }

bool Buffer::isPlaying() { return mPlaybackState == PlaybackState::kReverse; }

void Buffer::rec() {
  mPlaybackState = kRecord;
  mReadOffset = 1;
  mReadHead = 0;
  mWriteHead = 0;
}

void Buffer::stop() {
  mPlaybackState = kReverse;
  mReadOffset = -1;
}

double Buffer::read() {
  double result = mBuffer[mReadHead];

  if (mPlaybackState == kNormal) {
    return result;
  }

  mReadHead += mReadOffset;

  if (mPlaybackState == kReverse && mReadHead < 0) {
    mPlaybackState = kNormal;
    mReadOffset = 0;
    mReadHead = 0;
    mWriteHead = 0;
  }

  return result;
}

void Buffer::write(double value) {
  mBuffer[mWriteHead] = value;

  if (mPlaybackState == kNormal) {
    return;
  }

  mWriteHead += 1;

  if (mWriteHead > mCapacity - 1) {
    mPlaybackState = kReverse;
    mReadOffset = -1;
  }
}
} // namespace Reverse300
