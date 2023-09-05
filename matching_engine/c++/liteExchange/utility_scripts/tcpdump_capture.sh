#/bin/bash
TCPDUMP_EXECUTABLE=tcpdump
LITE_EXCHANGE_TCP_PORT_NUMBER=5001
echo "Displaying network interfaces : "
$TCPDUMP_EXECUTABLE -D
echo "Enter number for the network interface ( Pick lo for 127.0.0.1 ) "
read NW_INTERFACE_NUMBER
echo "Enter capture output name : "
read CAPTURE_OUTPUT
echo "Capture starting. Press Ctrl C to quit..."
$TCPDUMP_EXECUTABLE -i $NW_INTERFACE_NUMBER port $LITE_EXCHANGE_TCP_PORT_NUMBER -w $CAPTURE_OUTPUT