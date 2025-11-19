FROM randomdude/gcc-cross-x86_64-elf

RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y --no-install-recommends \
      nasm xorriso grub-pc-bin grub-common qemu-kvm virt-manager bridge-utils libgtk-3-dev && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

VOLUME /root/env
WORKDIR /root/env