# Virtual Camera Driver

## Overview

This project is a **Windows Kernel Mode Driver** that emulates a virtual camera device. It integrates **Windows Portable Devices (WPD)** and supports **Windows Hello** for facial recognition spoofing using a machine learning model.

## Features

- **Virtual Camera Emulation**: Presents itself as a real camera to Windows.
- **Custom ML Integration**: Allows face detection using an AI model.
- **Trace Logging**: Uses **WPP (Windows Software Trace Preprocessor)** for debugging and logging.
- **Windows Hello Compatibility**: Spoofs an **IR sensor** for Windows authentication.
- **Kernel-Mode Development**: Developed using **Windows Driver Kit (WDK)** and **Visual Studio 2022**.

## Prerequisites

### **Development Environment**

- **Windows 10/11 (x64)**
- **Visual Studio 2022** (with Windows Driver Kit installed)
- **Windows Driver Kit (WDK)**
- **Windows Software Development Kit (SDK)**
- **Windows Debugging Tools**

### **Build Tools**

- **MSBuild** (for compiling the driver)
- **TraceView** (for debugging WPP logs)
- **WinDbg** (for kernel debugging)

## Installation

### **1. Build the Driver**

Use the Developer Command Prompt for VS 2022:

```sh
msbuild VirtualCamDriver.sln /t:Build /p:Configuration=Release
```

### **2. Sign the Driver (If Required)**

If deploying outside of test mode, sign the driver:

```sh
signtool sign /v /fd SHA256 /a /f your_cert.pfx VirtualCamDriver.sys
```

### **3. Install the Driver**

- Copy the compiled `.sys` file to the target system.
- Use `devcon.exe` to install:
  ```sh
  devcon install VirtualCamDriver.inf Root\VirtualCam
  ```

### **4. Enable Test Signing (For Unsigned Drivers)**

If running an unsigned driver, enable test mode:

```sh
bcdedit /set testsigning on
shutdown /r /t 0
```

## Debugging

To capture debug logs using WPP:

```sh
traceview.exe -start VirtualCamDriverTrace -guid trace.guid
```

To view Windows Hello compatibility logs, check:

```sh
type %SystemRoot%\INF\setupapi.dev.log
```

## Contributing

Feel free to fork, improve, and submit pull requests. Contributions are welcome!

## License

MIT License
