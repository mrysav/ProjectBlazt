#!/usr/bin/env ruby

require 'rubygems'
require 'bundler/setup'
require 'nokogiri'

fname = ARGV[0]
lname = ARGV[1]

unless fname && lname
  puts 'Usage: mklevel.rb [IN FILE NAME] [LEVEL NAME]'
  return 1
end

puts "reading #{fname}..."
doc = File.open(fname) { |f| Nokogiri::XML(f) }

map = doc.at_css('map')
height = map['height']
width = map['width']
data = map.at_css('data').content

puts "map is #{width}x#{height}..."

l_u_name = lname.upcase
l_h_name = lname + '.h'
l_c_name = lname + '.c'
l_data = '{' + data.strip.gsub(/,\n/, "},\n{") + '}'

puts "writing #{l_u_name} to #{l_h_name} and #{l_c_name}..."

File.open(l_h_name, 'w') do |file|
  file.write(%(
#ifndef __#{l_u_name}_H__
#define __#{l_u_name}_H__

#include "level.h"

extern const Level #{l_u_name};

#endif
  ).strip)
end

File.open(l_c_name, 'w') do |file|
  file.write(%(
#include "level.h"
#include "#{l_h_name}"

const Level #{l_u_name} = {
  #{width},
  #{height},
    {
      #{l_data}
    }
};
  ).strip)
end
