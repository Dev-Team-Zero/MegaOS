FROM randomdude/gcc-cross-x86_64-elf

RUN apt-get upgrade 
RUN apt-get update 
RUN apt-get install -y nasm
RUN apt-get install -y xorriso
RUN apt-get install -y grub-pc-bin
RUN apt-get install -y grub-common
RUN apt-get install -y qemu-kvm 
RUN apt-get install -y virt-manager 
RUN apt-get install -y bridge-utils
RUN apt-get install -y libgtk-3-dev
RUN apt-get install -y qemu-system-gui
RUN rm -rf /var/lib/apt/lists/*

WORKDIR /env