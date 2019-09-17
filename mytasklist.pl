use warnings;
use strict;

my @procs = `tasklist`;

#Find position of PID based on the ===== ====== line in the header
my $pid_pos;
if ($procs[2] =~ /^=+/)
{
    $pid_pos = $+[0]+1;
}
else
{
    die "Unexpected format!";   
}

my %pids;
for (@procs[3 .. $#procs])
{
    #Get process name and strip whitespace
    my $name = substr $_,0,$pid_pos;
    $name =~s/^\s+|\s+$//g;

    #Get PID
    if (substr($_,$pid_pos) =~ /^\s*(\d+)/)
    {
        $pids{$1} = $name;  
    }
}


print  %pids;