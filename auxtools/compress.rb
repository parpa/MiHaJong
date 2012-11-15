#!/usr/bin/env ruby
# -*- coding: UTF-8 -*-

def is_win?
	if (RUBY_PLATFORM.downcase =~ /mswin(?!ce)|mingw|cygwin|bccwin/) then
		return true
	else
		return false
	end
end

require "digest/sha2"; require "pathname"

# If ruby-lzma is available, use it.
# If not, give an alternative.
ruby_lzma_available = nil
begin
	require "lzma"
	ruby_lzma_available = true
rescue LoadError
	ruby_lzma_available = false
end

if ARGV.size < 1 then
	$stderr.print("Usage: #{$0} src-file\n")
	exit(1)
end

srcfile = Pathname.new(ARGV[0])
targetfile = Pathname.new((srcfile.to_s) + ".lzma")
symbolname = srcfile.basename.to_s.upcase.gsub(/\W/, '_')
headerfile = Pathname.new((srcfile.to_s) + ".h")
cppfile = Pathname.new((srcfile.to_s) + ".cpp")

compressed = nil
digest = nil
File.open(srcfile, "rb") {|source|
	source_data = source.read()
	digest = Digest::SHA256.hexdigest(source_data)
	if ruby_lzma_available then
		compressed = LZMA.compress(source_data)
		File.open(targetfile, "wb") {|target|
			target.print(compressed)
		}
	else
		command = is_win?() ?
			"python #{File.expand_path(File.dirname(__FILE__)).gsub(/\//, "\\")}\\compress.py" :
			"python #{File.expand_path(File.dirname(__FILE__))}/compress.py"
		if not system("#{command} #{srcfile.to_s}") then
			throw("Failure in #{command}")
		end
	end
}
File.open(headerfile, "wt") {|target|
	target.print(<<"FINIS")
// This file was automatically generated by #{(Pathname.new($0)).basename.to_s}. Do not edit.

#ifndef DATA_#{symbolname}_H
#define DATA_#{symbolname}_H

#include "../decomp.h"

namespace Compressed {

class file_#{symbolname.downcase} : public Data {
private:
	static LPCTSTR Description;
	static const int FileID = FILEID_#{symbolname};
	static const uint8_t expectedDigest[32];
};

}

#endif
FINIS
}
File.open(cppfile, "wt") {|target|
	hexarray = ""
	digest.to_s.unpack("a2"* (digest.to_s.length / 2)).each do |i|
		hexarray += "0x%2s, " % [i]
	end
	hexarray_arranged = []
	hexarray.to_s.unpack("a47x"* (hexarray.to_s.length / 48)).each do |i|
		hexarray_arranged += ["\t%s" % [i]]
	end
	target.print(<<"FINIS")
// This file was automatically generated by #{(Pathname.new($0)).basename.to_s}. Do not edit.

#include "#{headerfile.basename.to_s}"

namespace Compressed {

LPCTSTR file_#{symbolname.downcase}::Description = FILEDESC_#{symbolname};
const uint8_t file_#{symbolname.downcase}::expectedDigest[32] = {
#{hexarray_arranged.join("\n")}
};

}
FINIS
}
