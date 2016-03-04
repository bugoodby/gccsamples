#!/usr/bin/ruby
# -*- coding: utf-8 -*-

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
		
		hunk.each {|line|
			if ( /^@@ -(\d+),(\d+) \+(\d+),(\d+)/ =~ line )
#				printf("!!! %d, %d, %d, %d\n", $1, $2, $3, $4)
				line_num = $3.to_i - 1
				prev_type = :others
			else
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
			end
		}
		if ( prev_type == :add )
			@diffBlocks << DiffInfo.new(diff_start, line_num)
		end
	end

	def dumpDiffInfo
		@diffBlocks.each_with_index {|b, i|
			printf("   %d : (%d, %d)\n", i, b.startLine, b.endLine)
		}
	end
	
	def insertComment
		lines = File.open(@file, "r").readlines
		@diffBlocks.reverse_each {|b|
			lines[b.endLine, 0] = "// LCOV_EXCL_START"
			lines[b.startLine - 1, 0] = "// LCOV_EXCL_STOP"
		}
		File.open(@file, "w") {|out|
			lines.each {|l|
				out.puts l
			}
		}
	end
end


commit = ARGV[0] || "HEAD"

#ソースファイル一覧
srcfiles = []
Dir.glob("**/*") {|f|
	srcfiles << f.chomp if ( /\.\(c|cpp|cxx|h|hpp\)$/ =~ f )
}
p srcfiles

#git diffで差分のあるファイル一覧（削除ファイル除く）
modfiles = []
IO.popen("git diff #{commit} --name-only --diff-filter=ACMUX").each {|line|
	modfiles << line.chomp
}
p modfiles

# ソースファイルの差分箇所にlcovコメント挿入
modfiles.each {|f|
	if srcfiles.include?(f)
		puts "+++ #{f}"
		gdfile = GitDiffFile.new(f)
		
		hunk = [];
		IO.popen("git diff #{commit} -- #{f}").each {|line|
			if ( hunk.length == 0 )
				hunk << line if ( /^@@ / =~ line )
			else
				if ( /^@@ / =~ line )
					gdfile.parseHunk(hunk)
					hunk = [line]
				else
					hunk << line
				end
			end
		}
		gdfile.parseHunk(hunk)
		
		gdfile.dumpDiffInfo
#		gdfile.insertComment
	else
		puts "+++ #{f} [skip]"
	end
}

# 全ソースファイルの先頭と末尾にlcovコメント挿入
srcfiles.each {|f|
	`sed -i -e "1i // LCOV_EXCL_START" #{f}`
	`echo "// LCOV_EXCL_STOP" >> #{f}`
}

