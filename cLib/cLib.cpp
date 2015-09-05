#include "cLib.h"
#include "Windows.h"
#include "Winuser.h"

struct EnumData {
	jobjectArray *nameList;
	JNIEnv *env;
	int* index;
};

BOOL CALLBACK winEnum(HWND hwnd, LPARAM lPram) {

	EnumData *enumData = (EnumData*)lPram;

	jobjectArray *nameList = enumData->nameList;
	JNIEnv *env = enumData->env;
	int *index = enumData->index;

	int length = GetWindowTextLength(hwnd) + 1;
	char* name = new char[length];

	GetWindowTextA(hwnd, name, length);
	env->SetObjectArrayElement(*nameList, *index, env->NewStringUTF(name));

	(*index)++;
	delete name;

	return true;
}

BOOL CALLBACK winNum(HWND hwnd, LPARAM lPram) {

	int *winCount = (int*)lPram;
	(*winCount)++;

	return true;
}

JNIEXPORT jobjectArray JNICALL Java_app_JNI_CLibrary_enumWindows
(JNIEnv *env, jobject obj) {

	int winCount = 0;
	EnumWindows(winNum, (LPARAM)&winCount);
	
	jobjectArray nameList = (jobjectArray)env->NewObjectArray(winCount, env->FindClass("java/lang/String"), NULL);
	int index = 0;

	EnumData enumData;
	enumData.env = env;
	enumData.nameList = &nameList;
	enumData.index = &index;

	EnumWindows(winEnum, (LPARAM) &enumData);

	return nameList;
}