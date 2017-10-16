function build()
  demos = { {'Ice/latency' 'Ice/latency/Latency.ice'}
            {'Ice/throughput' 'Ice/latency/Latency.ice'}
            {'Ice/gui' 'Ice/hello/Hello.ice'}
          };
  for i = 1:length(demos)
    demo = demos{i};
    fprintf(1, 'Building %s...', demo{1});
    generated = fullfile(demo{1}, 'generated');
    if exist(generated, 'file') == 0
      mkdir(generated);
    end
    status = slice2matlab(sprintf('%s --output-dir %s', demo{2}, generated));
    if status == 0
      fprintf(1, 'ok\n');
    end
end
