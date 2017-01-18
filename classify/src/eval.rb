#!/usr/bin/env ruby

$KCODE = 'u';

require 'getopts';
getopts('a:','b:');

# ----- ----- ----- ----- -----

fpa = open( $OPT_a );
fpb = open( $OPT_b );

cc = 0; cm = 0; mc = 0; mm = 0; z = 0;

all = 0;
fpa.each{ |label_ans|
  label_ans.chomp!;

  label_out = fpb.readline(); label_out.chomp!;

  if label_ans == 'cleaner' && label_out == 'cleaner';
    cc += 1;
  elsif label_ans == 'cleaner' && label_out == 'mp3player';
    cm += 1;
  elsif label_ans == 'mp3player' && label_out == 'cleaner';
    mc += 1;
  elsif label_ans == 'mp3player' && label_out == 'mp3player';
    mm += 1;
  else
    z += 1;
  end

  all += 1;
}

puts ( cc + mm ).to_f / all.to_f;
puts 'cc:' + cc.to_s + ' cm:' + cm.to_s + ' mc:' + mc.to_s + ' mm:' + mm.to_s;

# ----- ----- ----- ----- -----
