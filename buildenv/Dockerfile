FROM randomdude/gcc-cross-x86_64-elf

RUN apt-get update 
RUN apt-get upgrade -y 
RUN apt-get install -y --no-install-recommends nasm 
RUN apt-get install -y --no-install-recommends xorriso 
RUN apt-get install -y --no-install-recommends grub-pc-bin 
RUN apt-get install -y --no-install-recommends grub-common 
RUN apt-get install -y --no-install-recommends qemu-kvm 
RUN apt-get install -y --no-install-recommends virt-manager 
RUN apt-get install -y --no-install-recommends bridge-utils 
RUN apt-get install -y --no-install-recommends libgtk-3-dev 
RUN apt-get clean 
RUN rm -rf /var/lib/apt/lists/*

VOLUME /root/env
WORKDIR /root/env