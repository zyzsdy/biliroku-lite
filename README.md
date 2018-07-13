# BILIROKU-LITE

This project was created because I needed to use a small tool to download bilibili live on a Ubuntu Server and my router (Arch: MIPS).

It is a continuation of the previous project ([Biliroku](https://github.com/zyzsdy/biliroku)), but it is much less functional than Biliroku. For example, there is no GUI, and there is no function of saving the comment.

However, this project was designed to work on weaker devices, so the project will strictly control CPU and memory usage. I hope this will bring a good experience.

源码分发，编译运行。我就不提供二进制你来打我呀。

## Dependencies

* CMake (Version 3.4 or newer) 
* RapidJSON (https://github.com/Tencent/rapidjson/releases)
* CURL (7.60.0 or newer)
* cmdline (https://github.com/tanakh/cmdline)

## Compilation

1. Download the CURL source and compile it;
2. Download the RapidJSON source and save it.

Cmdline has been released with the source code of this program, so you don't need to prepare it yourself.

The compilation steps are as follows:

```bash
mkdir build
cd build
cmake -DRAPID_JSON_DIR <path_to_rapidJSON> ../
make
make install
```

## Usage

Run `biliroku -h` to display help.

Simple use example:

**Download the live stream of roomid = 1 and save it to aust_{time}.flv:**

```bash
biliroku -n 1 -o aust_{time}.flv
```

{time} will be automatically replaced with a string of the form YYYYMMDDHHMMSS. The video file you finally got might look like as: aust_20180713150201.flv

**Download the live stream of roomid = 1, use the socks5 proxy 127.0.0.1 at port 1080:**

```bash
biliroku -n 1 --proxy socks5://127.0.0.1:1080
```

## LICENSE

GPLv3