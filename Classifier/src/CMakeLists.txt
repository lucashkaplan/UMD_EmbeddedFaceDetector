IF(EXISTS ${PROJECT_SOURCE_DIR}/test)
    SET(source_list
		Actors/file_sink.cpp
		Graph/face_detector.cpp
		Actors/classifier.cpp
		Actors/file_sink.cpp
		Actors/ImgRead.cpp
        )
ELSE()
    SET(source_list
		Actors/file_sink.cpp
		Graph/face_detector.cpp
		Actors/classifier.cpp
		Actors/file_sink.cpp
		Actors/ImgRead.cpp
		)
ENDIF(EXISTS ${PROJECT_SOURCE_DIR}/test)

ADD_LIBRARY(classifier_src ${source_list})
INSTALL(TARGETS classifier_src DESTINATION .)

