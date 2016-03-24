# QtGui-for-Motor-Drivers-and-DSLR-Camera-Control

Using the SDK from Nikon (copyrighted but it is a free download after registering). Wrappers are in the cameracontrol.h and cameracontrol.c (remember to edit Function.c from the SDK).

Controlling 3 motors using windows handle's for COM port access.

There are two different image windows. One is the generic window where you can control the motors (gui_003.ui). The other is a detailed window (InspectionWindow.ui).

Wrapper files must be compiled to a DLL combined wtih the Nikon provided libraries.
