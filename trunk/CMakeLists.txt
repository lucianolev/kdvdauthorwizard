project(kdvdauthorwizard)

find_package(KDE4 REQUIRED)
include_directories(${KDE4_INCLUDES})

set(kdvdauthorwizard_SRCS
 main.cpp
 dvdauthorwizard.cpp
 dvdvideo.cpp
 movie.cpp
 playlistmodel.cpp
 dvdauthorprocess.cpp
)

kde4_add_ui_files(kdvdauthorwizard_SRCS 
 ui/pagewelcome.ui
 ui/pageintromovie.ui
 ui/pagestoragefolder.ui
 ui/pageplaylist.ui
 ui/pageauthoring.ui
)

kde4_add_executable(kdvdauthorwizard ${kdvdauthorwizard_SRCS})
target_link_libraries(kdvdauthorwizard ${KDE4_KDEUI_LIBS}
	${KDE4_KIO_LIBS})
