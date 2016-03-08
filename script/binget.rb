#!/usr/bin/ruby
# -*- coding: utf-8 -*-
require 'pathname'

def usage
	puts "usage: #{File.basename($0)} <file>"
end

if ARGV.length == 0
	usage()
	exit
end

infile = ARGV[0]
outfile = infile + ".bin"

File.open(outfile, "wb") {|of|
	IO.popen("od -tx1 -vAn #{infile}").each {|line|
		line.strip!
		of.write( Array(line.delete(' ')).pack("H*") )
	}
}

