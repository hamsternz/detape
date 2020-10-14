# detape
Decode IBM PC cassette audio files back to data.

IBM PCs used to have tape drives.

The data format is 255 bytes of 0xFF, one byte of 0xFE (used to synchronize), then the data. 
Apprently data is written in 256 byte blocks, with a CRC, but I didn't get into that.

The ones are encoded as 500us high, 500us low. The zeros are 250us high, 250us low. This
gives between 1000 bits per second to 2000 bits per second, depending on the mix of ones and zeros.

This program assumes a 24kS/s stereo audio file, but only uses one channel.

It is also pretty primative, so will most likely only work on relatively 'clean' audio files.
