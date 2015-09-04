#include "cLib.h"
#include "Windows.h"
#include "Winuser.h"
#include <vector>

std::vector<char*> names;

BOOL CALLBACK winEnum(HWND hwnd, LPARAM lPram) {

    int length = GetWindowTextLength(hwnd);

    char* name = new char[length + 1];

    GetWindowTextA(hwnd, name, length + 1);

    names.push_back(name);

    return true;
}

JNIEXPORT jobjectArray JNICALL Java_app_JNI_CLibrary_enumWindows
(JNIEnv *env, jobject obj) {

	EnumWindows(winEnum, NULL);
	unsigned int nameSize = names.size();

	jobjectArray nameList = (jobjectArray)env->NewObjectArray(nameSize, env->FindClass("java/lang/String"), NULL);

	for (unsigned int i = 0; i < nameSize; i++) {
		char* temp = names[0];
		env->SetObjectArrayElement(nameList, i, env->NewStringUTF(temp));

		names.erase(names.begin());
		delete temp;
	}

	return nameList;
}