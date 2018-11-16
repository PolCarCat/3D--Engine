#ifndef __GLOBALS_H__
#define __GLOBALS_H__

// Warning disabled ---
#pragma warning( disable : 4577 ) // Warning that exceptions are disabled
#pragma warning( disable : 4530 ) // Warning that exceptions are disabled

#include <windows.h>
#include <stdio.h>
#include <list>
#include <vector>

#define VSLOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HAVE_M_PI


typedef unsigned int uint;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Configuration -----------
#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900
#define SCREEN_SIZE 1
#define WIN_FULLSCREEN false
#define WIN_RESIZABLE true
#define WIN_BORDERLESS false
#define WIN_FULLSCREEN_DESKTOP false
#define VSYNC true
#define TITLE "Mehgine"
// Far Plane Distance
#define FPD 50000.0f



namespace Utils 
{
	template<typename TYPE>
	void ClearList(typename std::list<TYPE>& list) 
	{
		typename std::list<TYPE>::iterator it;
		for (it = list.begin(); it != list.end(); it++) {
			delete *it;
		}
		list.clear();
	}

	template<typename TYPE>
	void ClearVector(typename std::vector<TYPE>& vector) 
	{
		typename std::vector<TYPE>::iterator it;
		for (it = vector.begin(); it != vector.end(); it++) {
			delete *it;
		}
		vector.clear();
	}

	template<typename TYPE>
	size_t FindInList(TYPE ptr, typename std::list<TYPE>& list) 
	{
		size_t pos = 0;
		typename std::list<TYPE>::iterator it;
		for (it = list.begin(); it != list.end(); pos++, it++) {
			if (*it == ptr) break;
		}

		if (it != list.end()) {
			return pos;
		}
		return -1;
	}

	template<typename TYPE>
	size_t FindInVector(TYPE ptr, typename std::vector<TYPE>& vector) 
	{
		size_t pos = 0;
		for (pos = 0; pos < vector.size(); pos++) {
			if (vector[pos] == ptr) return pos;
		}
		return -1;
	}

	template<typename TYPE>
	bool RemoveFromList(TYPE ptr, typename std::list<TYPE>& list)
	{
		typename std::list<TYPE>::iterator it;
		for (it = list.begin(); it != list.end(); it++) {
			if (*it == ptr) break;
		}

		if (it != list.end()) {
			list.remove(*it);
			return true;
		}
		return false;
	}

	template<typename TYPE>
	bool RemoveFromVector(TYPE ptr, typename std::vector<TYPE>& vector)
	{
		typename std::vector<TYPE>::iterator it;
		for (it = vector.begin(); it != vector.end(); it++) {
			if (*it == ptr) break;
		}

		if (it != vector.end()) {
			vector.erase(it);
			return true;
		}
		return false;
	}
};

#endif //__GLOBALS_H__