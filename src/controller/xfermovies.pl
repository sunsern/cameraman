#! /usr/bin/perl -w

$dir = "/Users/cameraman/Documents/Quicktime_Recordings/";
open(FID, $dir . "log.txt");
@lines = <FID>;
close(FID);
unlink($dir . "log.txt");

foreach $line (@lines) {
    chomp($line);
    ($num, $time_stamp) = split(/ /, $line);
    $filename = "";
    if($num == 0) {
	$filename = $dir . "Movie.mov"; 
    }
    else {
	$filename = $dir . "Movie " . $num . ".mov";
    }
    if($time_stamp == 0) {
	unlink($filename);
    }
    else {
	rename($filename, $dir . "send_me/". $time_stamp . ".mov");
	rename($dir . $time_stamp . ".log", $dir . "send_me/". $time_stamp . ".log");
    }
}


