FROM archlinux:base-devel

RUN pacman -Syu --noconfirm && \
    pacman -S --noconfirm openssh git meson cmake qt6-base qt6-webengine layer-shell-qt

RUN pacman -Scc --noconfirm

CMD ["/bin/bash"]