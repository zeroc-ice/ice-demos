function clean()
    demos = {'Ice/gui' 'Ice/latency' 'Ice/throughput'};
    for i = 1:length(demos)
        fprintf(1, 'Cleaning %s... ', demos{i});
        generated = fullfile(demos{i}, 'generated');
        if exist(generated, 'dir') == 7
            rmdir(generated, 's');
        end
        fprintf(1, 'ok\n');
    end
end
