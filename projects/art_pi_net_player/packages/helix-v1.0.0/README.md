### The Helix MP3 Decoder

The Helix MP3 decoder provides MPEG-compliant decoding of MP3 content. Both floating-point and fixed-point decoder implementations are available. The fixed-point decoder is optimized especially for ARM processors but can run on any 32-bit fixed-point processor which can perform a long multiply operation (two 32-bit inputs generating a 64-bit result) and long multiply-accumulate (long multiply with 64-bit accumulator).

### Key Features

- Pure 32-bit fixed-point implementation
- High-quality C reference code for porting to new platforms
- Optimized for ARM processors
- Fully reentrant and statically linkable
- Optional C++ API for compatibility with Helix clients
- Designed for high performance and low power consumption in handheld and mobile devices
- Full layer 3 support for
  - MPEG1 layer 3 - sampling frequencies: 48 KHz, 44.1 KHz, 32 KHz
  - MPEG2 layer 3 - sampling frequencies: 24 KHz, 22.05 KHz, 16 KHz
  - MPEG2.5 layer 3 - sampling frequencies: 12 KHz, 11.025 KHz, 8 KHz
- Supports constant bitrate, variable bitrate, and free bitrate modes
- Supports mono and all stereo modes (normal stereo, joint stereo, dual-mono)
- Option to use Intel® IPP performance libraries (if available)
  - Easy to link in either IPP libraries or Helix code



### Technical Specifications



#### Average CPU Usage

| Sample Rate | Channels      | Bit Rate | Processor Model (1) |            |        |        |        |        |
| ----------- | ------------- | -------- | ------------------- | ---------- | ------ | ------ | ------ | ------ |
| ARM7TDMI    | ARM9TDMI-REV2 | ARM920T  | ARM9E               | StrongARM1 | XScale |        |        |        |
| 48.0 KHz    | 2             | 320 Kbps | 30 MHz              | 24 MHz     | 27 MHz | 20 MHz | 20 MHz | 20 MHz |
| 44.1 KHz    | 2             | 128 Kbps | 26 MHz              | 21 MHz     | 24 MHz | 17 MHz | 17 MHz | 17 MHz |

(1) Tested with ARMulator, simulated zero-wait-state memory



#### Memory Usage

- ROM = 13446 Bytes (const globals)
- RAM = 23816 Bytes (heap)
  - *Total Data Memory = **37262 Bytes***
- Code Size = 21000 Bytes (approximately - depends on compiler)



### Frequently Asked Questions



#### Where is the code in CVS?

- See the Helix Datatype project page: http://datatype.helixcommunity.org
  The CVS root is /cvsroot/datatype, and the module name (path) is mp3/codec/fixpt



#### Where does the build system put the code in my local source tree?

- datatype/mp3/codec/fixpt



#### How does the build system decide whether to build the fixed-point or floating-point MP3 decoder?

- If HELIX_CONFIG_FIXEDPOINT is defined in your profile, it will build the fixed-point decoder. Otherwise it will build the floating-point version.





整理自 [https://www.helixcommunity.org/projects/datatype/Mp3dec](https://www.helixcommunity.org/projects/datatype/Mp3dec)

整理者 [liuduanfei](https://github.com/liuduanfei)