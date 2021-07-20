# mbedTLS

[中文页](README_ZH.md) | English

## 1 Introduction 

**mbedTLS** (formerly PolarSSL) is an SSL/TLS algorithm library open sourced and maintained by ARM. It uses the C programming language to implement the SSL/TLS function and various encryption algorithms with the smallest code footprint, which is easy to understand, use, integrate and extend, and it is convenient for developers to easily use the SSL/TLS function in embedded products.

The [mbedtls](https://github.com/RT-Thread-packages/mbedtls) package is **RT-Thread** based on [ARMmbed/mbedtls](https://github.com/ARMmbed/mbedtls/ ) Porting of the open source library [v2.6.0 version](https://github.com/ARMmbed/mbedtls/tree/72ea31b026e1fc61b01662474aa5125817b968bc). For more information about mbedTLS, please refer to [Package Details](docs/introduction.md ).

### 1.1 Directory structure

| Name | Description |
| ---- | ---- |
| certs | CA certificate storage directory |
| docs | Document directory |
| mbedtls | ARM mbedtls source code |
| ports | Migration file directory |
| samples | Sample file directory |
| LICENSE | License File |
| README.md | Package Instructions |
| SConscript | RT-Thread default build script |

### 1.2 License

Apache License Version 2.0 agreement license.

## 2. Get the software package

Under the BSP directory used, use the ENV tool to open menuconfig to obtain the software package.

- Configure software packages and enable examples

```c
RT-Thread online packages --->
    security packages --->
            Select Root Certificate ---> # Select certificate file
        [*] mbedtls: An portable and flexible SSL/TLS library --- # Open the mbedtls package
        [*] Store the AES tables in ROM # Store the AES tables in ROM to optimize memory usage
        (2) Maximum window size used # The maximum "window" size used for dot multiplication (2-7, the smaller the value, the smaller the memory usage)
        (3584) Maxium fragment length in bytes # Configure the data frame size (0x7200 error can try to increase the size)
        [*] Enable a mbedtls client example # Open mbedtls test routine
        [] Enable Debug log output # Enable debug log output
              version (latest) ---> # Select the package version, the default is the latest version
```

- Select certificate file

```c
RT-Thread online packages --->
    security packages --->
        [*] mbedtls: An portable and flexible SSL/TLS library --- # Open the mbedtls package
                Select Root Certificate ---> # Select certificate file
                    [] Using all default CA(Use preset CA certificates. Take up more memory)
                    [] Using user CA(copy your Root CA file to mbedtls package "certs" directory)
                    [*] Using Digital Signature Trust Root CA # The certificate used by the test routine
```

- The `Using all default CA` configuration option will add all the preset certificates in the `certs/default` directory to the compilation, which will take up a lot of memory
- The `Using user CA` configuration option allows users to add the certificate files they need to the compilation, requiring the user to copy the certificate files to the root directory of `certs`
- For more instructions, please refer to [Detailed Description of Package](docs/introduction.md)
- Use the `pkgs --update` command to download the package

## 3. Use mbedtls

- How to use from scratch, please refer to [User Manual](docs/user-guide.md)
- For complete API documentation, please refer to [API Manual](docs/api.md)
- For detailed sample introduction, please refer to [Sample Document](docs/samples.md)
- For the working principle of mbedtls protocol, please refer to [Working Principle](docs/principle.md)
- mbedtls RAM and ROM optimization, please read [Resource Occupation Optimization Guide](docs/footprint-optimization-guide.md)
- More **Detailed introduction documents** are located in the [`/docs`](/docs) folder, **Please check before using the package for development**

## 4. Reference materials

- mbedTLS official website: https://tls.mbed.org/
- ARMmbed GitHub: [mbedtls](https://github.com/ARMmbed/mbedtls/tree/72ea31b026e1fc61b01662474aa5125817b968bc)

## 5. Contact & Thanks

- Maintenance: RT-Thread development team
- Homepage: https://github.com/RT-Thread-packages/mbedtls
