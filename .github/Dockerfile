FROM archlinux:base-devel

# install dependencies
RUN pacman -Syu --noconfirm && \
    pacman -S --noconfirm openssh git meson cmake qt6-base qt6-webengine layer-shell-qt

# add github user for github actions
RUN useradd -m -d /github/home github

# allow sudo
RUN echo 'github ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers

USER github

# add ssh config
WORKDIR /github/home
RUN mkdir -p /github/home/.ssh
RUN printf "Host aur.archlinux.org\n\tHostName aur.archlinux.org\n\tUser aur\n\tIdentityFile /github/home/.ssh/aur\n\tStrictHostKeyChecking no\n\tUserKnownHostsFile /dev/null" > /github/home/.ssh/config
RUN chmod 644 /github/home/.ssh/config

# install yay
WORKDIR /tmp
RUN git clone https://aur.archlinux.org/yay-bin.git yay && \
    cd yay && \
    makepkg -si --noconfirm

# install tosu
RUN yay -S --noconfirm tosu

# cleanup
RUN yay -Scc --noconfirm

CMD ["/bin/bash"]