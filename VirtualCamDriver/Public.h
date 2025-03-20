/*++

Module Name:

    public.h

Abstract:

    This module contains the common declarations shared by driver
    and user applications.

Environment:

    user and kernel

--*/

//
// Define an Interface Guid so that apps can find the device and talk to it.
//

DEFINE_GUID (GUID_DEVINTERFACE_VirtualCamDriver,
    0x51453196,0x65c6,0x44a9,0x9f,0xe0,0x15,0xbd,0xfc,0xc3,0xd2,0xda);
// {51453196-65c6-44a9-9fe0-15bdfcc3d2da}
