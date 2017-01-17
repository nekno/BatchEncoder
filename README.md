﻿# BatchEncoder (Audio Conversion GUI)

[![Gitter](https://badges.gitter.im/wieslawsoltes/BatchEncoder.svg)](https://gitter.im/wieslawsoltes/BatchEncoder?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge)

[![Build status](https://ci.appveyor.com/api/projects/status/gf82ojfu97e7udx7/branch/master?svg=true)](https://ci.appveyor.com/project/wieslawsoltes/batchencoder/branch/master)

BatchEncoder is an audio files conversion software.

### Support BatchEncoder

[![PayPal - The safer, easier way to pay online!](https://www.paypalobjects.com/en_US/i/btn/btn_donate_SM.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=GFQMG36HPYT6W)

### Download BatchEncoder

| Platforn              | Type        | Version       | Download                                                                                                                               |
|-----------------------|-------------|---------------|----------------------------------------------------------------------------------------------------------------------------------------|
| Windows 32-bit        | Installer   | 1.0           | [BatchEncoder-1.0-Win32.exe](https://github.com/wieslawsoltes/BatchEncoder/releases/download/1.0/BatchEncoder-1.0-Win32.exe)                      |
| Windows 64-bit        | Installer   | 1.0           | [BatchEncoder-1.0-x64.exe](https://github.com/wieslawsoltes/BatchEncoder/releases/download/1.0/BatchEncoder-1.0-x64.exe)          |
| Windows 32-bit        | Portable    | 1.0           | [BatchEncoder-1.0-Win32.zip](https://github.com/wieslawsoltes/BatchEncoder/releases/download/1.0/BatchEncoder-1.0-Win32.zip)                      |
| Windows 64-bit        | Portable    | 1.0           | [BatchEncoder-1.0-x64.zip](https://github.com/wieslawsoltes/BatchEncoder/releases/download/1.0/BatchEncoder-1.0-x64.zip)          |

You can also download bleeding edge builds from the CI server [here](https://ci.appveyor.com/project/wieslawsoltes/batchencoder/build/artifacts).

## About

BatchEncoder is an audio files conversion software. It supports most of popular audio file formats 
including lossy and lossless compression. The program is very simple to use. 
BatchEncoder is basically GUI front-end for command-line tools. All it does is create background 
processes and pass arguments to it with options set in presets/format menu and additionally 
adds input and output file paths. For specific options for each format use help available for command-line tools.

## Features

* Audio conversion GUI.
* Supports popular audio formats.
* Supports file transcoding.
* Easily configurable by new users.
* Advanced options for experienced users.
* Open-source, fast and simple.

## Screenshots

### File list

[![File List](screenshots/FileList.png)](https://github.com/wieslawsoltes/BatchEncoder/blob/master/screenshots/FileList.png)

### Conversion progress

[![File List Converting](screenshots/FileListConverting.png)](https://github.com/wieslawsoltes/BatchEncoder/blob/master/screenshots/FileListConvertingFileList.png)

## Supported formats and tools

### Encoders

| Name            | Type        | File extension  | Version       | Executable                       | Progress function                         |
|-----------------|-------------|-----------------|---------------|----------------------------------|-------------------------------------------|
| SSRC            | Encoder     | WAV             | -.-           | ssrc.exe                         | GetProgress_SsrcEnc.progress              |
| LAME            | Encoder     | MP3             | 3.99.5        | lame.exe                         | GetProgress_LameEnc.progress              |
| MPPENC          | Encoder     | MPC             | 1.30.0        | mppenc.exe                       | GetProgress_MppEnc.progress               |
| FAAC            | Encoder     | AAC             | 1.28-mod      | faac.exe                         | GetProgress_FaacEnc.progress              |
| NEROAACENC      | Encoder     | M4A             | 1.5.4         | neroAacEnc.exe                   | - none -                                  |
| NEROAACENC      | Encoder     | MP4             | 1.5.4         | neroAacEnc.exe                   | - none -                                  |
| OGGENC          | Encoder     | OGG             | 2.88          | oggenc2.exe                      | GetProgress_OggEnc.progress               |
| AFTEN           | Encoder     | AC3             | git-2009122   | aften.exe                        | - none -                                  |
| FLAC            | Encoder     | FLAC            | 1.3.2         | flac.exe                         | GetProgress_FlacEnc.progress              |
| WAVPACK         | Encoder     | WV              | 5.0.0         | wavpack.exe                      | GetProgress_WavPackEnc.progress           |
| MAC             | Encoder     | APE             | 4.18          | mac.exe                          | GetProgress_MacEnc.progress               |
| OFR             | Encoder     | OFR             | 5.100         | ofr.exe                          | GetProgress_OfrEnc.progress               |
| TTAENC          | Encoder     | TTA             | -.-           | ttaenc.exe                       | GetProgress_TtaEnc.progress               |
| TAKC            | Encoder     | TAK             | 2.3.0         | takc.exe                         | - none -                                  |

### Decoders

| Name            | Type        | File extension  | Version       | Executable                       | Progress function                         |
|-----------------|-------------|-----------------|---------------|----------------------------------|-------------------------------------------|
| LAME            | Decoder     | MP1             | -.-           | lame.exe                         | GetProgress_LameDec.progress              |
| LAME            | Decoder     | MP2             | -.-           | lame.exe                         | GetProgress_LameDec.progress              |
| LAME            | Decoder     | MP3             | -.-           | lame.exe                         | GetProgress_LameDec.progress              |
| MPPDEC          | Decoder     | MPC             | -.-           | mppdec.exe                       | GetProgress_MppDec.progress               |
| MPPDEC          | Decoder     | MPP             | -.-           | mppdec.exe                       | GetProgress_MppDec.progress               |
| MPPDEC          | Decoder     | MP+             | -.-           | mppdec.exe                       | GetProgress_MppDec.progress               |
| FAAD            | Decoder     | MP4             | -.-           | faad.exe                         | GetProgress_FaadDec.progress              |
| FAAD            | Decoder     | M4A             | -.-           | faad.exe                         | GetProgress_FaadDec.progress              |
| FAAD            | Decoder     | AAC             | -.-           | faad.exe                         | GetProgress_FaadDec.progress              |
| NEROAACDEC      | Decoder     | MP4             | -.-           | neroAacDec.exe                   | - none -                                  |
| NEROAACDEC      | Decoder     | M4A             | -.-           | neroAacDec.exe                   | - none -                                  |
| OGGDEC          | Decoder     | OGG             | -.-           | oggdec.exe                       | GetProgress_OggDec.progress               |
| AFTEN           | Decoder     | AC3             | -.-           | aften.exe                        | - none -                                  |
| FLAC            | Decoder     | FLAC            | -.-           | flac.exe                         | GetProgress_FlacDec.progress              |
| WVUNPACK        | Decoder     | WV              | -.-           | wvunpack.exe                     | GetProgress_WvUnpackDec.progress          |
| MAC             | Decoder     | APE             | -.-           | mac.exe                          | GetProgress_MacDec.progress               |
| MAC             | Decoder     | MAC             | -.-           | mac.exe                          | GetProgress_MacDec.progress               |
| OFR             | Decoder     | OFR             | -.-           | ofr.exe                          | GetProgress_OfrDec.progress               |
| TTAENC          | Decoder     | TTA             | -.-           | ttaenc.exe                       | GetProgress_TtaDec.progress               |
| TAKC            | Decoder     | TAK             | -.-           | takc.exe                         | - none -                                  |

### Tool downloads

| Name            | Downloads                                                                                          |
|-----------------|----------------------------------------------------------------------------------------------------|
| SSRC            | http://shibatch.sourceforge.net/                                                                   |
| LAME            | http://rarewares.org/mp3-lame-bundle.php                                                           |
| MPPENC          | https://musepack.net/index.php?pg=win                                                              |
| MPPDEC          | https://musepack.net/index.php?pg=win                                                              |
| FAAC            | http://rarewares.org/aac-encoders.php                                                              |
| FAAD            | http://rarewares.org/aac-decoders.php                                                              |
| OGGENC          | http://rarewares.org/ogg-oggenc.php                                                                |
| OGGDEC          | http://rarewares.org/ogg-oggdec.php                                                                |
| AFTEN           | https://github.com/wieslawsoltes/wavtoac3encoder/releases                                          |
| FLAC            | https://xiph.org/flac/download.html                                                                |
| WAVPACK         | http://www.wavpack.com/downloads.html                                                              |
| WVUNPACK        | http://www.wavpack.com/downloads.html                                                              |
| MAC             | http://www.monkeysaudio.com/download.html                                                          |
| OFR             | http://losslessaudio.org/Downloads.php                                                             |
| TTAENC          | https://sourceforge.net/projects/tta/files/tta/ttaenc-win/                                         |
| TAKC            | http://www.thbeck.de/Tak/Tak.html                                                                  |
| NEROAACENC      | http://www.videohelp.com/software/Nero-AAC-Codec                                                   |
| NEROAACDEC      | http://www.videohelp.com/software/Nero-AAC-Codec                                                   |

## System requirements

Minimum support version is Windows 7 SP1 or above. 

Command-line tool dependencies may have different hardware and sofrware requirements.

## Configuration files

Program configuration files are used to store all settings. System registry is not used to store program settings. Only XML text files with UTF-8 encoding are supported.

Configuration files extensions:

    .xml		- All configurations files
    .config		- Main program configuration
    .list		- File-list entry's
    .presets	- Preset names and format options
    .formats	- Encoders and decoders format specification
    .progress   - Compiled progress function DLL

Please use only UTF-8 text editors to edit any configuration files. There is no description of format 
of configuration files. The file format is self explaining and most of time it is automatically 
generated by the program. If you really need to know more check the program sources to find how the things are done.

### Default configuration file

https://github.com/wieslawsoltes/BatchEncoder/tree/master/config

### Default progress functions

https://github.com/wieslawsoltes/BatchEncoder/tree/master/src/progress

## Developers

To build program from sources you will need the following components:

### Microsoft Visual Studio 2015

Build `BatchEncoder.sln` solution with [Visual Studio 2015](https://www.visualstudio.com).

### TinyXml-2 Library

Required [TinyXml-2](https://github.com/leethomason/tinyxml2) sources are included in `src\xml` directory.

## License

BatchEncoder (Audio Conversion GUI) is licensed under the [MIT license](LICENSE.TXT).
