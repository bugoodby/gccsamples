#!/usr/bin/ruby
# -*- coding: utf-8 -*-
require "optparse"

usage_msg = <<EOS
insert debug print statement in functions.

Usage: ruby #{File.basename($0)} [options...] <target dir/file>
EOS

params = {}
OptionParser.new(usage_msg) do |opt|
	opt.on('-h', '--help', 'show this message') { puts opt; exit }
	opt.on('-n', '--dry-run', '(debug) 実際のファイル変更は行わない') {|v| params[:n] = v}
	begin
		opt.parse!
	rescue OptionParser::InvalidOption => e
		puts '[ERROR] ' + e.message
		exit
	end
	if ( ARGV.length == 0 )
		puts opt
		exit
	end
end

target = ARGV[0]

srcfiles = []
if ( File.directory?(target) )
	Dir.glob("#{target}/**/*") {|f|
		srcfiles << f.chomp if ( /\.(c|cpp|cxx|h|hpp|hxx)$/ =~ f )
	}
elsif ( File.exist?(target) )
	srcfiles << target
else
	puts "[ERROR] invalid target: #{target}"
	exit
end

srcfiles.each {|infile|
	begin
		puts " +++ #{infile}"
		lines = File.open(infile, "r:UTF-8").readlines

		ws = File.open(infile, "w") unless ( params[:n] )
		lines.each {|line|
			ws.puts line unless ( params[:n] )
			if ( /\/\/#\[ operation (.+)\(\)/ =~ line )
				puts line
				ws.puts 'printf("%s\n", __func__);' unless ( params[:n] )
			end
		}
		ws.close unless ( params[:n] )
	rescue => e
		puts e
		print "continue? (y/n): "
		response = STDIN.gets.chomp.downcase
		exit if ( response != 'y' )
	end
}
