using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CH.Alika.POS.Hardware
{
    public enum ScanSourceEventType {
        DATA_EVENT,
        ERROR_EVENT,
        DEVICE_EVENT
    }

    // Events such as when the device is connected and discconnected, reading errors, or data read
    public class ScanSourceEvent : EventArgs
    {
        
        public MMM.Readers.FullPage.EventCode EventCode { get; private set; }
        public MMM.Readers.ErrorCode ErrorCode { get; private set; }
        public String ErrorMessage { get; private set; }

        private object SwipeData { get; set; }
        private MMM.Readers.Modules.Swipe.SwipeItem SwipeItem { get; set; }

        public ScanSourceEventType EventType { get; private set; }

        public ScanSourceEvent(MMM.Readers.Modules.Swipe.SwipeItem swipeItem, object swipeData)
        {
            EventType = ScanSourceEventType.DATA_EVENT;
            SwipeItem = swipeItem;
            SwipeData = swipeData;
        }

        public ScanSourceEvent(MMM.Readers.FullPage.EventCode eventCode)
        {
            EventType = ScanSourceEventType.DEVICE_EVENT;
            EventCode = eventCode;
            ErrorMessage = null;
        }

        public ScanSourceEvent(MMM.Readers.ErrorCode errorCode, string errorMessage)
        {
            EventType = ScanSourceEventType.ERROR_EVENT;
            ErrorCode = errorCode;
            ErrorMessage = errorMessage == null ? "no error message given" : errorMessage;
        }

        public bool IsError
        {
            get
            {
                return EventType != ScanSourceEventType.ERROR_EVENT;
            }
        }

        public bool IsDeviceEvent
        {
            get
            {
                return EventType != ScanSourceEventType.DEVICE_EVENT;
            }
        }

        public bool IsDataEvent
        {
            get
            {
                return EventType != ScanSourceEventType.DATA_EVENT;
            }
        }

        public override String ToString()
        {
            String msg;
            switch (EventType) {
                case ScanSourceEventType.DATA_EVENT:
                    msg = String.Format("ScanSourceEvent DATA SwipeItem [{0}] SwipeData [{1}]", SwipeItem, SwipeData);
                    break;
                case ScanSourceEventType.DEVICE_EVENT:
                    msg = String.Format("ScanSourceEvent EVENT EventCode [{0}]", EventCode);
                    break;
                case ScanSourceEventType.ERROR_EVENT:
                    msg = String.Format("ScanSourceEvent ERROR ErrorCode [{0}] ErrorMessage [{1}]", ErrorCode, ErrorMessage);
                    break;
                default:
                     msg = "ScanSourceEvent unknown";
                     break;
            }
            return msg;
        }
    }
}
