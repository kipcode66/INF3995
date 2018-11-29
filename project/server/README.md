# Elevation

Remember to **READ THE FINE MANUAL**. It's so useful sometimes.

Setting up
----------

Before running these below, you should first install the Elevation Server's dependencies:

    sudo pacman -Sy core/automake core/make core/gcc core/curl extra/boost extra/pulseaudio-alsa extra/pulseaudio extra/tcl extra/wget extra/git extra/cmake extra/python3

You should then copy the files under `res/config` to the proper location under `/etc` and run :

    pulseaudio -k

or ideally reboot your system.

Finally, you should build the Elevation Server and generate a blank database. See below.

Building
--------

Use the elevation script. Mind you ; this will take about an hour on the FPGA.

Generating a blank database
---------------------

Use the elevation script.

Starting
--------

Use the elevation script.

Starting in development mode
--------

Use the elevation script.

Stopping
--------

Use the elevation script.

Testing
-------

Use the elevation script to execute the unit tests. See `src/common/test/api-tests/READMEYOUFOOL.md` to run the REST API tests.

Regenerating SSL certificates
-----------------------------

Remove all files under `res/ssl` except the `regenerateCertificates`
script, then run `regenerateCertificates` and copy `res/ssl/CA/ca.crt` to `<git_dir>/project/client/app/src/main/res/raw/`.
