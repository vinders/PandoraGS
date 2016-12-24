/*******************************************************************************
PANDORAGS project - PS1 GPU driver test app
------------------------------------------------------------------------
File name :   main.h
Description : test app - flow/unit tests -- entry point and test functions
*******************************************************************************/
#pragma once
#include "resource.h"

///<summary>Plugin test function</summary>
///<param name="hWindow">Main window handle</param>
void ProcessTest(HWND hWindow);

///<summary>Complete plugin unit testing</summary>
///<param name="hWindow">Main window handle</param>
void UnitTest(HWND hWindow);

///<summary>Custom drawing primitives testing</summary>
///<param name="hWindow">Main window handle</param>
void PrimitivesTest(HWND hWindow);
