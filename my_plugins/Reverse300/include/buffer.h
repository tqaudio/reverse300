#pragma once

namespace Reverse300 {
enum PlaybackState { kNormal = 0, kRecord = 1, kReverse = 2 };

class Buffer {
public:
  Buffer(int sampleRate, double maxDuration /* ms */);
  ~Buffer();

  bool isPlaying();

  void rec();
  void stop();

  double read();
  void write(double value);

private:
  int mSampleRate; // e.g. 44100 (Hz)
  int mCapacity;   // e.g. 441000 (samples)
  PlaybackState mPlaybackState = PlaybackState::kNormal;
  int mReadHead = 0;
  int mWriteHead = 0;
  int mReturnPosition = 0;
  int mReadOffset = 0;
  double *mBuffer = nullptr;
};
} // namespace Reverse300
