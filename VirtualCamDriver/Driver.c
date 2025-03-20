/*++

Module Name:

    driver.c

Abstract:

    This file contains the driver entry points and callbacks.

Environment:

    Kernel-mode Driver Framework

--*/

#include "driver.h"

#ifdef ALLOC_PRAGMA
#pragma alloc_text (INIT, DriverEntry)
#pragma alloc_text (PAGE, VirtualCamDriverEvtDeviceAdd)
#pragma alloc_text (PAGE, VirtualCamDriverEvtDriverContextCleanup)
#endif

NTSTATUS
DriverEntry(
    _In_ PDRIVER_OBJECT  DriverObject,
    _In_ PUNICODE_STRING RegistryPath
)
{
    WDF_DRIVER_CONFIG config;
    NTSTATUS status;
    WDF_OBJECT_ATTRIBUTES attributes;

    KdPrint(("[VirtualCamDriver] DriverEntry called\n"));

    WDF_OBJECT_ATTRIBUTES_INIT(&attributes);
    attributes.EvtCleanupCallback = VirtualCamDriverEvtDriverContextCleanup;

    WDF_DRIVER_CONFIG_INIT(&config,
        VirtualCamDriverEvtDeviceAdd
    );

    status = WdfDriverCreate(DriverObject,
        RegistryPath,
        &attributes,
        &config,
        WDF_NO_HANDLE
    );

    if (!NT_SUCCESS(status)) {
        KdPrint(("[VirtualCamDriver] WdfDriverCreate failed: 0x%08X\n", status));
        return status;
    }

    KdPrint(("[VirtualCamDriver] DriverEntry completed successfully\n"));
    return status;
}

NTSTATUS
VirtualCamDriverEvtDeviceAdd(
    _In_    WDFDRIVER       Driver,
    _Inout_ PWDFDEVICE_INIT DeviceInit
)
{
    NTSTATUS status;

    UNREFERENCED_PARAMETER(Driver);

    PAGED_CODE();

    KdPrint(("[VirtualCamDriver] EvtDeviceAdd called\n"));

    status = VirtualCamDriverCreateDevice(DeviceInit);

    KdPrint(("[VirtualCamDriver] EvtDeviceAdd completed with status: 0x%08X\n", status));
    return status;
}

VOID
VirtualCamDriverEvtDriverContextCleanup(
    _In_ WDFOBJECT DriverObject
)
{
    UNREFERENCED_PARAMETER(DriverObject);

    PAGED_CODE();

    KdPrint(("[VirtualCamDriver] DriverContextCleanup called\n"));
}
