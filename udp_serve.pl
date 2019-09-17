use Warnings;
use IO::Socket::INET;
$MAX_BYTES = 1025;
$udp_port="4321";
$packet_num=1;
my $listenSock = IO::Socket::INET->new('LocalPort'=>$udp_port, 'Proto'=>'udp');
my $readBuff;
while(1) {
$listenSock->recv( $readBuff, $MAX_BYTES );
print "$packet_num\n";
print $readBuff; print "\n";
#print  unpack("C*", $readBuff);
$packet_num++;
}
