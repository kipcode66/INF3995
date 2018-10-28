# Image that the container is based on. In this case, it is an OS, but it could be
# Python, MySQL, another container image ...
FROM base/archlinux:2018.10.01

ARG build_script
ARG run_script
ARG source_dir
ARG destination_dir

# Set the working directory inside our container
WORKDIR /elevation

# Copy files to our container's working directory
COPY "${build_script}"  "/elevation/${build_script}"
COPY "${run_script}"    "/elevation/${run_script}"
COPY "${source_dir}"    "/elevation/${destination_dir}"

# Environment variables accessible from the container's bash shell.
ENV build_script ${build_script}
ENV run_script ${run_script}
ENV destination_dir ${destination_dir}

# Prepare image (install dependencies, build the server, ...)
RUN yes | pacman -Syu core/automake core/make core/gcc core/curl extra/boost extra/pulseaudio-alsa extra/pulseaudio extra/tcl extra/wget extra/cmake extra/python3
RUN chmod +x "${build_script}" "${run_script}"
RUN ./"${build_script}" "${destination_dir}"

# Commands to run when we start the container
CMD "./${run_script}"
