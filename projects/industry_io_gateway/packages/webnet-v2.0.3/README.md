# WebNet

[中文页](README_ZH.md) | English

## 1 Introduction

The WebNet software package is independently developed by RT-Thread and is based on the HTTP protocol web server. It not only provides the basic functions of communication between the device and the HTTP Client, but also supports a variety of module function extensions, with less resource usage, strong tailorability, and sufficient Meet the functional requirements of developers for embedded device servers.

The features of the WebNet software package are as follows:

- Support HTTP 1.0/1.1
- Support AUTH basic authentication function
- Support CGI function
- Support ASP variable substitution function
- Support SSI file embedding function
- Support INDEX directory file display function
- Support ALIAS alias access function
- Support file upload function
- Support pre-compression function
- Support cache function
- Support breakpoint resume function

For more information about the features of the software package, please see [Detailed Introduction](docs/introduction.md).

### 1.1 Directory structure

| Name | Description |
| ---------- | ------------------------ |
| docs | Document directory |
| inc | Header file directory |
| src | Source file directory |
| module | Function module file directory |
| samples | Sample file directory |
| LICENSE | License File |
| README.md | Package Instructions |
| SConscript | RT-Thread default build script |

### 1.2 License

The WebNet software package follows the GPL2+ commercial dual license. The software package can use the General Public License according to the GNU standard, see the LICENSE file for details. If it is used in commercial applications, you can contact us to obtain a commercial license via email <business@rt-thread.com>.

### 1.3 Dependency

- RT-Thread 3.0+
- DFS file system

## 2. Get the software package

To use the WebNet software package, you need to select it in the RT-Thread package management. The specific path is as follows:

```c
RT-Thread online packages
    IoT-internet of things --->
    [*] WebNet: A HTTP Server for RT-Thread
            (80) Server listen port
            (16) Maximum number of server connections
            (/webnet) Server root directory
                  Select supported modules --->
                     [] LOG: Enanle output log support
                     [] AUTH: Enanle basic HTTP authentication support
                     [] CGI: Enanle Common Gateway Interface support
                     [] ASP: Enanle Active Server Pages support
                     [] SSI: Enanle Server Side Includes support
                     [] INDEX: Enanle list all the file in the directory support
                     [] ALIAS: Enanle alias support
                     [] DAV: Enanle Web-based Distributed Authoring and Versioning support
                     [] UPLOAD: Enanle upload file support
                     [] GZIP: Enable compressed file support by GZIP
                     (0) CACHE: Configure cache level
            [] Enable webnet samples
            Version (latest) --->
```

**Server listen port**: Configure the server listening port number;

**Maximum number of server connections**: Configure the maximum number of server connections;

**Server root directory**: Configure the server root directory path;

**Select supported modules**: select the function modules supported by the server;

**Enable webnet samples**: configure and add server sample files;

**Version**: Configure the software package version.

After the configuration is complete, let the RT-Thread package manager automatically update, or use the pkgs --update command to update the package to the BSP.

## 3. Use WebNet software package

- For detailed description of the software package, please refer to [Package Introduction](docs/introduction.md)
- For detailed sample introduction, please refer to [Sample Document](docs/samples.md)
- How to use from scratch, please refer to [User Guide](docs/user-guide.md)
- For complete API documentation, please refer to [API Manual](docs/api.md)
- For the working principle of the package, please refer to [Working Principle](docs/principle.md)
- More **Detailed introduction documents** are located under the `/docs` folder, **Please check before using the package for development**.

## 4. Matters needing attention

- The use of the WebNet software package requires file system support, and it is necessary to ensure that the file system can be used on the running device.
- The WebNet software package does not enable any module function support by default. You need to enable the required functions in Env according to [Package Introduction](docs/introduction.md).

## 5. Contact & Thanks

- Maintenance: RT-Thread development team
- Homepage: <https://github.com/RT-Thread-packages/webnet>
