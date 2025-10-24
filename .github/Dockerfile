FROM archlinux:base-devel

# install dependencies
RUN pacman -Syu --noconfirm && \
    pacman -S --noconfirm openssh git meson cmake qt6-base qt6-webengine layer-shell-qt

# add github user for github actions
RUN useradd -m -d /github/home github

# allow sudo
RUN echo 'github ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers

USER github

# install yay
WORKDIR /tmp
RUN git clone https://aur.archlinux.org/yay-bin.git yay && \
    cd yay && \
    makepkg -cis --noconfirm

# install tosu
RUN yay -S --noconfirm tosu

# cleanup
RUN yay -Scc --noconfirm

CMD ["/bin/bash"]