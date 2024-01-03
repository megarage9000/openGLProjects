#pragma once

#ifndef GARAGE_LOG
#define GARAGE_LOG

#include <time.h>
#include <stdarg.h>
#include <stdio.h>
#define GL_LOG_FILE "gl.log"

bool restart_gl_log();
bool gl_log(const char* message, ...);
bool gl_log_err(const char* message, ...);

#endif // !GARAGE_LOG
