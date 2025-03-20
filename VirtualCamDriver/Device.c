/*++

Module Name:

    device.c - Device handling events for virtual camera driver.

Abstract:

   This file contains the device entry points and callbacks.

Environment:

    Kernel-mode Driver Framework

--*/

#include "driver.h"
#include "Trace.h"
#include "device.h"

// Define the GUID here (only once)
const GUID GUID_DEVINTERFACE_VirtualCamDriver =
{ 0x12345678, 0x1234, 0x1234, {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0} };

#ifdef ALLOC_PRAGMA
#pragma alloc_text (PAGE, VirtualCamDriverCreateDevice)
#endif

NTSTATUS
VirtualCamDriverCreateDevice(
    _Inout_ PWDFDEVICE_INIT DeviceInit
)
/*++

Routine Description:

    Worker routine called to create a device and its software resources.

Arguments:

    DeviceInit - Pointer to an opaque init structure. Memory for this
                    structure will be freed by the framework when the WdfDeviceCreate
                    succeeds. So don't access the structure after that point.

Return Value:

    NTSTATUS

--*/
{
    WDF_OBJECT_ATTRIBUTES deviceAttributes;
    PDEVICE_CONTEXT deviceContext;
    WDFDEVICE device;
    NTSTATUS status;

    PAGED_CODE();

    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&deviceAttributes, DEVICE_CONTEXT);

    status = WdfDeviceCreate(&DeviceInit, &deviceAttributes, &device);

    if (NT_SUCCESS(status)) {
        // Get a pointer to the device context structure that we just associated
        // with the device object.
        deviceContext = DeviceGetContext(device);

        // Initialize the context.
        deviceContext->PrivateDeviceData = 0;

        // Create a device interface for the virtual camera.
        status = WdfDeviceCreateDeviceInterface(
            device,
            &GUID_DEVINTERFACE_VirtualCamDriver,
            NULL // ReferenceString
        );

        if (NT_SUCCESS(status)) {
            // Initialize the I/O Package and any Queues
            status = VirtualCamDriverQueueInitialize(device);
        }
    }

    return status;
}
