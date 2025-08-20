#include "app/display_list.h"

void displayHappyFace() {
	faceDisplay->Expression.GoTo_Awe();
	faceDisplay->LookFront();
	faceDisplay->Update();
}