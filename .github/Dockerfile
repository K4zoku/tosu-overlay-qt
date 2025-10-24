FROM archlinux:base-devel

RUN pacman -Syu --noconfirm && \
    pacman -S --noconfirm qt6-base qt6-webengine layer-shell-qt meson

RUN pacman -Scc --noconfirm

CMD ["/bin/bash"]