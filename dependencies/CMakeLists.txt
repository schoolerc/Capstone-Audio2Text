INCLUDE(ExternalProject)

externalproject_add(FFMPEG
        GIT_REPOSITORY https://github.com/FFmpeg/FFmpeg.git
        GIT_TAG n3.0
        CONFIGURE_COMMAND "./configure --prefix=${CMAKE_CURRENT_SOURCE_DIR}/ffmpeg_install"
        BUILD_COMMAND "make"
        INSTALL_COMMAND "make install"
        )

externalproject_add(SPHINXBASE
        GIT_REPOSITORY https://github.com/cmusphinx/sphinxbase.git
        CONFIGURE_COMMAND "./autogen.sh && ./configure"
        BUILD_COMMAND "make"
        INSTALL_COMMAND "make install"
        )

externalproject_get_property(SPHINXBASE install_dir)

externalproject_add(POCKETSPHINX DEPENDS SPHINXBASE
        GIT_REPOSITORY https://github.com/cmusphinx/pocketsphinx.git
        CONFIGURE_COMMAND "./autogen.sh && ./configure --with-sphinxbase=${install_dir}"
        BUILD_COMMAND "make"
        INSTALL_COMMAND "make_install"
        )