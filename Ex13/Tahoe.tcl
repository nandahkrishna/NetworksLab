set ns [new Simulator]
set nr [open outt.tr w]
$ns trace-all $nr
set nf [open outt.nam w]
$ns namtrace-all $nf
$ns color 1 Blue
$ns color 2 Red

proc finish {} {
global ns nf
$ns flush-trace
close $nf
exec nam outt.nam &
exit 0
}

set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]

$ns duplex-link $n0 $n1 10Mb 10ms DropTail
$ns duplex-link $n1 $n2 2Mb 10ms DropTail

$ns duplex-link-op $n0 $n1 orient left
$ns duplex-link-op $n1 $n2 orient left

$ns queue-limit $n0 $n1 10
$ns queue-limit $n1 $n2 10

set tcp [new Agent/TCP]
$ns attach-agent $n0 $tcp
set sink [new Agent/TCPSink/DelAck]
$ns attach-agent $n2 $sink
$ns connect $tcp $sink
$tcp set fid_ 1
$tcp set window_ 8000
$tcp set packetSize_ 552

set ftp [new Application/FTP]
$ftp attach-agent $tcp
$ftp set type_ FTP

$ns at 0.1 "$ftp start"
$ns at 4.5 "$ftp stop"

$ns at 5.0 "finish"
$ns run