/**
 * @file    main.cpp
 * @author  Lukas Piwowarski
 * @date    2021 November
 */

#ifndef CONFIG_H
#define CONFIG_H

#ifndef CMAKE_SOURCE_DIR
#define SOURCE_DIR "."
#endif

#ifdef CMAKE_SOURCE_DIR
#define SOURCE_DIR CMAKE_SOURCE_DIR
#endif

#endif