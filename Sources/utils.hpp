#ifndef __UTILS_HPP__
#define __UTILS_HPP__
#include <string>
#define CHECK_ERRORS
void checkGLError(int line);
void drawString(float x, float y, void* font, const std::string & str);

#endif
