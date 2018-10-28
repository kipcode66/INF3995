# IMPORTANT NOTE : This Dockerfile is expected to be built from <git-repo>/project/server.
# In practice, though, this should always be the case because we will not call docker
# directly, but rather through a script.

# Image that the container is based on. In this case, it is an OS, but it could be
# Python, MySQL, another container image ...
FROM base/archlinux:2018.10.01

# Set the working directory inside our container
WORKDIR /elevation

# Copy files to our container's working directory
COPY build-image        /elevation
COPY run-container      /elevation
COPY elevation-git      /elevation/elevation-git

# Prepare image (install dependencies, build the server, ...)
RUN yes | pacman -Syu core/automake core/make core/gcc core/curl extra/boost extra/pulseaudio-alsa extra/pulseaudio extra/tcl extra/wget extra/cmake extra/git extra/python3
RUN chmod +x build-image run-container
RUN ./build-image

# Commands to run when we start the container
CMD ["./run-container"]
