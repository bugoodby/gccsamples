#!/usr/bin/ruby
# -*- coding: utf-8 -*-
require "optparse"

DiffInfo = Struct.new("DiffInfo", :startLine, :endLine)

class GitDiffFile
	def initialize( file )
		@file = file
		@diffBlocks = []
	end
	
	def parseHunk( hunk )
		line_num = 1
		diff_start = 1
		prev_type = :others
		
		# 先頭の範囲情報を解析
		rangeInfo = hunk.shift
		if ( /^@@ -(\d+),(\d+) \+(\d+),(\d+)/ =~ rangeInfo )
#			printf("!!! %d, %d, %d, %d\n", $1, $2, $3, $4)
			line_num = $3.to_i - 1
		else
			puts "[ERROR] invalid hunk! - " + rangeInfo
		end
		
		# +位置を確認
		hunk.each {|line|
			if ( line[0] == "+" )
				line_num += 1
				diff_start = line_num if ( prev_type == :others )
				prev_type = :add
			else
				if ( prev_type == :add )
					@diffBlocks << DiffInfo.new(diff_start, line_num)
				end
				line_num += 1 if ( line[0] != "-" )
				prev_type = :others
			end
		}
		if ( prev_type == :add )
			@diffBlocks << DiffInfo.new(diff_start, line_num)
		end
	end

	def dumpDiffInfo
		@diffBlocks.each_with_index {|b, i|
			printf("   %d: (%d - %d)\n", i, b.startLine, b.endLine)
		}
	end
	
	def insertComment
		lines = File.open(@file, "r").readlines
		@diffBlocks.reverse_each {|b|
			lines[b.endLine, 0] = "// LCOV_EXCL_START"
			lines[b.startLine - 1, 0] = "// LCOV_EXCL_STOP"
		}
		File.open(@file, "w") {|out|
			lines.each {|l| out.puts l }
		}
	end
end

def insertLcovExclComment( file )
	lines = File.open(file, "r+").readlines
	lines.unshift("// LCOV_EXCL_START")
	lines.push("// LCOV_EXCL_STOP")
	File.open(file, "w") {|out|
		lines.each {|l| out.puts l }
	}
end

def checkExclude( excludes, f )
	ret = false
	excludes.each {|e|
		if ( f.start_with?(e) )
			puts " ! exclude: " + f
			ret = true
			break
		end
	}
	return ret
end

### main ###
params = {}
params[:e] = []

OptionParser.new("Usage: ruby #{File.basename($0)} [options] [<commit>] [<filepath>]") do |opt|
	opt.on('-h', '--help', 'show this message') { puts opt; exit }
	opt.on('-e VALUE', '--exclude VALUE', '除外パスの指定') {|v| params[:e] << v}
	opt.on('-n', '--dry-run', '(debug) 実際のファイル変更は行わない') {|v| params[:n] = v}
	opt.on('-d', '--diff-only', '(debug) ファイル先頭末尾のコメント挿入しない') {|v| params[:d] = v}
	begin
		opt.parse!
	rescue
		puts opt; exit
	end
end

commit = ARGV[0] || "HEAD"

if !File.exist?(".git")
	puts "gitリポジトリ直下で実行してください"
	exit
end

#ソースファイル一覧
srcfiles = []
Dir.glob("**/*") {|f|
	srcfiles << f.chomp if ( /\.(c|cpp|cxx|h|hpp|hxx)$/ =~ f )
}

#差分ファイル一覧
modfiles = []
if ( ARGV[1] )
	modfiles << ARGV[1]
else
	IO.popen("git diff #{commit} -w --name-only --diff-filter=ACMUX").each {|line|
		modfiles << line.chomp
	}
end
#除外パスにマッチするものを除く
if ( params[:e].length > 0 )
	modfiles.delete_if {|item| checkExclude(params[:e], item) }
end

# ソースファイルの差分箇所にlcovコメント挿入
puts "[[[ insert comment to diff points ]]]"
modfiles.each {|f|
	if srcfiles.include?(f)
		puts " +++ #{f}"
		gdfile = GitDiffFile.new(f)
		
		hunk = [];
		IO.popen("git diff #{commit} -w -- #{f}").each {|line|
			if ( hunk.length == 0 )
				hunk << line if ( line[0, 3] == "@@ " )
			else
				if ( line[0, 3] == "@@ " )
					gdfile.parseHunk(hunk)
					hunk = [line]
				else
					hunk << line
				end
			end
		}
		gdfile.parseHunk(hunk)
		
		gdfile.dumpDiffInfo
		gdfile.insertComment unless ( params[:n] )
	else
		puts " +++ #{f} [skip]"
	end
}

# 全ソースファイルの先頭と末尾にlcovコメント挿入
unless ( params[:d] )
	puts "[[[ insert comment to first/last line ]]]"
	srcfiles.each {|f|
		puts " +++ #{f}"
		insertLcovExclComment(f) unless ( params[:n] )
	}
end
