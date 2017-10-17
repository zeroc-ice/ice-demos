% **********************************************************************
% Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
% **********************************************************************

function build()
    function r = folders(root)
        function r = exclude(name)
            r = strcmp(name, '.') | strcmp(name, '..');
        end
        all = dir(root);
        all = all([all.isdir]);
        all = all(~cellfun(@exclude, {all.name}));
        r = {all.name};
    end

    components = folders('.');
    for i = 1:length(components)
        demos = folders(components{i});
        for j = 1:length(demos)
            demoDir = fullfile(components{i}, demos{j});
            slices = dir(fullfile(demoDir, '*.ice'));
            outputDir = fullfile(demoDir, 'generated');
            fprintf(1, 'Building %s... ', demoDir);
            if exist(outputDir, 'file') == 0
                mkdir(outputDir);
            end
            slice2matlab(sprintf('-I%s --output-dir %s %s\n', demoDir, outputDir,...
                                 strjoin(strcat(strcat(['"' demoDir '\'], {slices.name}), '"'), ' ')));
            fprintf(1, 'ok\n');
        end
    end
end
