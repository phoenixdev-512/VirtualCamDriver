/*++

Module Name:

    queue.c

Abstract:

    This file contains the queue entry points and callbacks.

Environment:

    Kernel-mode Driver Framework

--*/

#include "driver.h"
#include "Trace.h"
#include "queue.h"

#ifdef ALLOC_PRAGMA
#pragma alloc_text (PAGE, VirtualCamDriverQueueInitialize)
#endif

#define FRAME_BUFFER_SIZE 4096

static UCHAR FrameBuffer[FRAME_BUFFER_SIZE];
static size_t FrameBufferLength = 0;

NTSTATUS
VirtualCamDriverQueueInitialize(
    _In_ WDFDEVICE Device
)
/*++

Routine Description:

     The I/O dispatch callbacks for the frameworks device object
     are configured in this function.

     A single default I/O Queue is configured for parallel request
     processing, and a driver context memory allocation is created
     to hold our structure QUEUE_CONTEXT.

Arguments:

    Device - Handle to a framework device object.

Return Value:

    VOID

--*/
{
    WDFQUEUE queue;
    NTSTATUS status;
    WDF_IO_QUEUE_CONFIG queueConfig;

    PAGED_CODE();

    WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(
        &queueConfig,
        WdfIoQueueDispatchParallel
    );

    queueConfig.EvtIoDeviceControl = VirtualCamDriverEvtIoDeviceControl;
    queueConfig.EvtIoRead = VirtualCamDriverEvtIoRead;
    queueConfig.EvtIoWrite = VirtualCamDriverEvtIoWrite;
    queueConfig.EvtIoStop = VirtualCamDriverEvtIoStop;

    status = WdfIoQueueCreate(
        Device,
        &queueConfig,
        WDF_NO_OBJECT_ATTRIBUTES,
        &queue
    );

    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "WdfIoQueueCreate failed %!STATUS!", status);
        return status;
    }

    return status;
}

VOID
VirtualCamDriverEvtIoDeviceControl(
    _In_ WDFQUEUE Queue,
    _In_ WDFREQUEST Request,
    _In_ size_t OutputBufferLength,
    _In_ size_t InputBufferLength,
    _In_ ULONG IoControlCode
)
{
    TraceEvents(TRACE_LEVEL_INFORMATION,
        TRACE_QUEUE,
        "%!FUNC! Queue 0x%p, Request 0x%p OutputBufferLength %d InputBufferLength %d IoControlCode %d",
        Queue, Request, (int)OutputBufferLength, (int)InputBufferLength, IoControlCode);

    WdfRequestComplete(Request, STATUS_SUCCESS);
}

VOID
VirtualCamDriverEvtIoRead(
    _In_ WDFQUEUE Queue,
    _In_ WDFREQUEST Request,
    _In_ size_t Length
)
{
    NTSTATUS status;
    WDFMEMORY memory;
    size_t copyLength = min(Length, FrameBufferLength);

    status = WdfRequestRetrieveOutputMemory(Request, &memory);
    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "Failed to retrieve output memory: %!STATUS!", status);
        WdfRequestComplete(Request, status);
        return;
    }

    status = WdfMemoryCopyFromBuffer(memory, 0, FrameBuffer, copyLength);
    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "Failed to copy frame data: %!STATUS!", status);
    }

    WdfRequestCompleteWithInformation(Request, status, copyLength);
}

VOID
VirtualCamDriverEvtIoWrite(
    _In_ WDFQUEUE Queue,
    _In_ WDFREQUEST Request,
    _In_ size_t Length
)
{
    NTSTATUS status;
    WDFMEMORY memory;

    if (Length > FRAME_BUFFER_SIZE) {
        WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
        return;
    }

    status = WdfRequestRetrieveInputMemory(Request, &memory);
    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "Failed to retrieve input memory: %!STATUS!", status);
        WdfRequestComplete(Request, status);
        return;
    }

    status = WdfMemoryCopyToBuffer(memory, 0, FrameBuffer, Length);
    if (NT_SUCCESS(status)) {
        FrameBufferLength = Length;
    }
    else {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "Failed to copy data to frame buffer: %!STATUS!", status);
    }

    WdfRequestComplete(Request, status);
}

VOID
VirtualCamDriverEvtIoStop(
    _In_ WDFQUEUE Queue,
    _In_ WDFREQUEST Request,
    _In_ ULONG ActionFlags
)
{
    TraceEvents(TRACE_LEVEL_INFORMATION,
        TRACE_QUEUE,
        "%!FUNC! Queue 0x%p, Request 0x%p ActionFlags %d",
        Queue, Request, ActionFlags);
    return;
}
