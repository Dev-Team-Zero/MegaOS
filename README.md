# MegaOS Overview

[![Build Docker Image and compile binaries CI](https://github.com/eu888/MegaOS/actions/workflows/docker-image.yml/badge.svg)](https://github.com/eu888/MegaOS/actions/workflows/docker-image.yml)

[![CodeQL Advanced](https://github.com/eu888/MegaOS/actions/workflows/codeql.yml/badge.svg)](https://github.com/eu888/MegaOS/actions/workflows/codeql.yml)

MegaOS is an operating system designed from scratch.

## Installation and build

1. Clone the repository:

    ```bash
    git clone https://github.com/eu888/MegaOS.git
    ```

2. Navigate to the project directory:

    ```bash
    cd MegaOS
    ```

3. Create docker container and install dependencies:

    ```bash
    docker build buildenv -t myos-buildenv
    ```

4. Navigate to the source code folder

    ```bash
    cd kernel
    ```

5. Run the environment build command:

    - For Linux

    ```bash
    docker run --rm -it -v "$(pwd):/env" myos-buildenv
    ```  

    This will open an interactive shell in the container with the kernel source mounted at `/env`.

    - For Windows

    ```bash
    docker run --rm -it -v "$(pwd):/MegaOS" -w /MegaOS myos-buildenv
    ```

    This will open an interactive shell in the container with the kernel source mounted at `/MegaOS`.

6. Inside the container, build the project:

    ```bash
    make build-x86_64
    ```

## Usage

After compilation, you can run the `kernel.iso` found in `dist\x86_64` in:

### VirtualBox

1. Open VirtualBox and create a new virtual machine.
2. Select "Other" as the operating system type.
    Note: that the system must be 64 bits to work
3. Attach the `kernel.iso` file as the bootable disk.
4. Start the virtual machine.

### QEMU

1. Run this command while in the environment:

    ```bash
    make run
    ```

## Contributing

Contributions are welcome! Please follow the [contribution guidelines](CONTRIBUTING.md) to get started.

## License

This project is licensed under the BSD 3-Clause License - see the [LICENSE](LICENSE) file for details.
