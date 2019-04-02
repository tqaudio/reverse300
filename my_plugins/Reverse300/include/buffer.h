#pragma once

#include <math.h>

namespace Reverse300 {
enum PlaybackState { kNormal = 0, kRecord = 1, kReverse = 2 };

class Buffer {
public:
  Buffer(int sampleRate, double capacity /* ms */);
  ~Buffer();

  bool isPlaying();

  void rec();
  void stop();

  double read();
  void write(double value);

protected:
  int mCapacity;   // e.g. 48000
  int mSampleRate; // e.g. 48000
  int mReadHead;
  int mWriteHead;
  int mReturnPosition;
  int mReadOffset;
  PlaybackState mPlaybackState;
  double *mBuffer;
};
} // namespace Reverse300
